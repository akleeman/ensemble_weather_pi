/***************************************************************************
 *
 * Project:  OpenCPN Weather Routing plugin
 * Author:   Sean D'Epagnier
 *
 ***************************************************************************
 *   Copyright (C) 2016 by Sean D'Epagnier                                 *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301,  USA.         *
 ***************************************************************************
 */

#include <wx/wx.h>
#include <wx/stdpaths.h>
#include <wx/treectrl.h>
#include <wx/fileconf.h>

#include "Utilities.h"
#include "Boat.h"
#include "RouteMapOverlay.h"
#include "WeatherRouting.h"
#include "ensemble_weather_pi.h"

// Define minimum and maximum versions of the grib plugin supported
#define GRIB_MAX_MAJOR 4
#define GRIB_MAX_MINOR 1
#define GRIB_MIN_MAJOR 4
#define GRIB_MIN_MINOR 1

//Define minimum and maximum versions of the climatology plugin supported
#define CLIMATOLOGY_MAX_MAJOR 1
#define CLIMATOLOGY_MAX_MINOR 3
#define CLIMATOLOGY_MIN_MAJOR 0
#define CLIMATOLOGY_MIN_MINOR 10

static wxJSONValue g_ReceivedODVersionJSONMsg;
static bool ODVersionNewerThan(int major, int minor, int patch)
{
    wxJSONValue jMsg;
    wxJSONWriter writer;
    wxString    MsgString;
    jMsg[wxS("Source")] = wxS("ENSEMBLE_WEATHER_PI");
    jMsg[wxT("Type")] = wxT("Request");
    jMsg[wxT("Msg")] = wxS("Version");
    jMsg[wxT("MsgId")] = wxS("version");
    writer.Write( jMsg, MsgString );
    SendPluginMessage( wxS("OCPN_DRAW_PI"), MsgString );

    if(g_ReceivedODVersionJSONMsg.Size() <= 0)
        return false;
    if(g_ReceivedODVersionJSONMsg[wxS("Major")].AsInt() > major) return true;
    if(g_ReceivedODVersionJSONMsg[wxS("Major")].AsInt() == major &&
        g_ReceivedODVersionJSONMsg[wxS("Minor")].AsInt() > minor) return true;
    if(g_ReceivedODVersionJSONMsg[wxS("Major")].AsInt() == major &&
        g_ReceivedODVersionJSONMsg[wxS("Minor")].AsInt() == minor &&
        g_ReceivedODVersionJSONMsg[wxS("Patch")].AsInt() >= patch) return true;
    return false;
}


extern "C" DECL_EXP opencpn_plugin* create_pi(void *ppimgr)
{
    return new ensemble_weather_pi(ppimgr);
}

extern "C" DECL_EXP void destroy_pi(opencpn_plugin* p)
{
    delete p;
}

#include "icons.h"

ensemble_weather_pi::ensemble_weather_pi(void *ppimgr)
      :opencpn_plugin_110(ppimgr)
{
      // Create the PlugIn icons
      initialize_images();

      m_tCursorLatLon.Connect(wxEVT_TIMER, wxTimerEventHandler
                              ( ensemble_weather_pi::OnCursorLatLonTimer ), NULL, this);
}

ensemble_weather_pi::~ensemble_weather_pi(void)
{
      delete _img_WeatherRouting;
}

int ensemble_weather_pi::Init(void)
{

      AddLocaleCatalog( _T("opencpn-ensemble_weather_pi") );

      //    Get a pointer to the opencpn configuration object
      m_pconfig = GetOCPNConfigObject();

      // Get a pointer to the opencpn display canvas, to use as a parent for the WEATHER_ROUTING dialog
      m_parent_window = GetOCPNCanvasWindow();

      m_pWeather_Routing = NULL;

      m_leftclick_tool_id  = InsertPlugInTool
          (_T(""), _img_WeatherRouting, _img_WeatherRouting, wxITEM_CHECK,
           _("Ensemble_Weather"), _T(""), NULL,
           ENSEMBLE_WEATHER_TOOL_POSITION, 0, this);

      wxMenu dummy_menu;
      m_position_menu_id = AddCanvasContextMenuItem
          (new wxMenuItem(&dummy_menu, -1, _("Ensemble Weather Position")), this );
      SetCanvasContextMenuItemViz(m_position_menu_id, false);

      //    And load the configuration items
      LoadConfig();

      return (WANTS_OVERLAY_CALLBACK |
              WANTS_OPENGL_OVERLAY_CALLBACK |
              WANTS_TOOLBAR_CALLBACK    |
              WANTS_CONFIG              |
              WANTS_CURSOR_LATLON       |
              WANTS_NMEA_EVENTS         |
              WANTS_PLUGIN_MESSAGING
            );
}

bool ensemble_weather_pi::DeInit(void)
{
    if(m_pWeather_Routing)
        m_pWeather_Routing->Close();
    WeatherRouting *wr = m_pWeather_Routing;
    m_pWeather_Routing = NULL; /* needed first as destructor may call event loop */
    delete wr;

    return true;
}

int ensemble_weather_pi::GetAPIVersionMajor()
{
      return MY_API_VERSION_MAJOR;
}

int ensemble_weather_pi::GetAPIVersionMinor()
{
      return MY_API_VERSION_MINOR;
}

int ensemble_weather_pi::GetPlugInVersionMajor()
{
      return PLUGIN_VERSION_MAJOR;
}

int ensemble_weather_pi::GetPlugInVersionMinor()
{
      return PLUGIN_VERSION_MINOR;
}

wxBitmap *ensemble_weather_pi::GetPlugInBitmap()
{
    return new wxBitmap(_img_WeatherRouting->ConvertToImage().Copy());
}

wxString ensemble_weather_pi::GetCommonName()
{
      return _("Ensemble Weather");
}

wxString ensemble_weather_pi::GetShortDescription()
{
    return _("Display multiple forecasts simultaneously.");
}

wxString ensemble_weather_pi::GetLongDescription()
{
    return _("\
Ensemble Weather proivdes a way of displaying multiple forecasts,\n\
often from the same provider which gives a sense of how much you can\n\
trust any given one.\
");
}

void ensemble_weather_pi::SetDefaults(void)
{
}

int ensemble_weather_pi::GetToolbarToolCount(void)
{
      return 1;
}

void ensemble_weather_pi::SetCursorLatLon(double lat, double lon)
{
    if(m_pWeather_Routing && m_pWeather_Routing->FirstCurrentRouteMap() && !m_tCursorLatLon.IsRunning())
        m_tCursorLatLon.Start(50, true);

    m_cursor_lat = lat;
    m_cursor_lon = lon;
}

void ensemble_weather_pi::SetPluginMessage(wxString &message_id, wxString &message_body)
{
    if(message_id == wxS("ENSEMBLE_WEATHER_PI")) {
        // construct the JSON root object
        wxJSONValue  root;
        // construct a JSON parser
        wxJSONReader reader;
        // now read the JSON text and store it in the 'root' structure
        // check for errors before retreiving values...
        int numErrors = reader.Parse( message_body, &root );
        if ( numErrors > 0 ) {
            wxLogMessage(_T("ensemble_weather_pi: Error parsing JSON message - "));
            const wxArrayString& errors = reader.GetErrors();
            for(int i = 0; i < (int)errors.GetCount(); i++) {
                wxLogMessage( errors.Item( i ) );
                return;
            }
        }
        
        if(root[wxS("Type")].AsString() == wxS("Response") && root[wxS("Source")].AsString() == wxS("OCPN_DRAW_PI")) {
            if(root[wxS("Msg")].AsString() == wxS("Version") ) {
                if(root[wxS("MsgId")].AsString() == wxS("version"))
                    g_ReceivedODVersionJSONMsg = root;
            } else
            if(root[wxS("Msg")].AsString() == wxS("GetAPIAddresses") ) {
                wxString sptr = root[_T("OD_FindClosestBoundaryLineCrossing")].AsString();
                sscanf(sptr.To8BitData().data(), "%p", &RouteMap::ODFindClosestBoundaryLineCrossing);
            }
        }
    }
}

void ensemble_weather_pi::SetPositionFixEx(PlugIn_Position_Fix_Ex &pfix)
{
    m_boat_lat = pfix.Lat;
    m_boat_lon = pfix.Lon;
}

void ensemble_weather_pi::ShowPreferencesDialog( wxWindow* parent )
{
}

void ensemble_weather_pi::OnToolbarToolCallback(int id)
{
    if(!m_pWeather_Routing) {
        m_pWeather_Routing = new WeatherRouting(m_parent_window, *this);
        wxPoint p = m_pWeather_Routing->GetPosition();
        m_pWeather_Routing->Move(0,0);        // workaround for gtk autocentre dialog behavior
        m_pWeather_Routing->Move(p);

        SendPluginMessage(wxString(_T("GRIB_TIMELINE_REQUEST")), _T(""));
        SendPluginMessage(wxString(_T("CLIMATOLOGY_REQUEST")), _T(""));

        if(ODVersionNewerThan( 1, 1, 15)) {
            wxJSONValue jMsg;
            wxJSONWriter writer;
            wxString MsgString;
            jMsg[wxT("Source")] = wxT("ENSEMBLE_WEATHER_PI");
            jMsg[wxT("Type")] = wxT("Request");
            jMsg[wxT("Msg")] = wxS("GetAPIAddresses");
            jMsg[wxT("MsgId")] = wxS("GetAPIAddresses");
            writer.Write( jMsg, MsgString );
            SendPluginMessage( wxS("OCPN_DRAW_PI"), MsgString );
        }
        
        m_pWeather_Routing->Reset();
    }

    m_pWeather_Routing->Show(!m_pWeather_Routing->IsShown());
}

void ensemble_weather_pi::OnContextMenuItemCallback(int id)
{
    if(!m_pWeather_Routing)
        return;

    if(id == m_position_menu_id)
        m_pWeather_Routing->AddPosition(m_cursor_lat, m_cursor_lon);

    m_pWeather_Routing->Reset();
}

bool ensemble_weather_pi::RenderOverlay(wxDC &dc, PlugIn_ViewPort *vp)
{
    if(m_pWeather_Routing && m_pWeather_Routing->IsShown()) {
        wrDC wrdc(dc);
        m_pWeather_Routing->Render(wrdc, *vp);
        return true;
    }
    return false;
}

bool ensemble_weather_pi::RenderGLOverlay(wxGLContext *pcontext, PlugIn_ViewPort *vp)
{
    if(m_pWeather_Routing && m_pWeather_Routing->IsShown()) {
        wrDC wrdc;
        m_pWeather_Routing->Render(wrdc, *vp);
        return true;
    }
    return false;
}

void ensemble_weather_pi::OnCursorLatLonTimer( wxTimerEvent & )
{
    std::list<RouteMapOverlay *>routemapoverlays = m_pWeather_Routing->CurrentRouteMaps();
    bool refresh = false;
    for(std::list<RouteMapOverlay *>::iterator it = routemapoverlays.begin();
        it != routemapoverlays.end(); it++)
        if((*it)->SetCursorLatLon(m_cursor_lat, m_cursor_lon))
            refresh = true;

    m_pWeather_Routing->UpdateCursorPositionDialog();

    if(refresh) {
        RequestRefresh(m_parent_window);
        m_pWeather_Routing->CursorRouteChanged();
    }
}

bool ensemble_weather_pi::LoadConfig(void)
{
      wxFileConfig *pConf = (wxFileConfig *)m_pconfig;

      if(!pConf)
          return false;

      pConf->SetPath ( _T( "/PlugIns/WeatherRouting" ) );
      return true;
}

bool ensemble_weather_pi::SaveConfig(void)
{
      wxFileConfig *pConf = (wxFileConfig *)m_pconfig;

      if(!pConf)
          return false;

      pConf->SetPath ( _T ( "/PlugIns/EnsembleWeather" ) );
      return true;
}

void ensemble_weather_pi::SetColorScheme(PI_ColorScheme cs)
{
      DimeWindow(m_pWeather_Routing);
}

wxString ensemble_weather_pi::StandardPath()
{
    wxStandardPathsBase& std_path = wxStandardPathsBase::Get();
    wxString s = wxFileName::GetPathSeparator();
#if defined(__WXMSW__)
    wxString stdPath  = std_path.GetConfigDir();
#elif defined(__WXGTK__) || defined(__WXQT__)
    wxString stdPath  = std_path.GetUserDataDir();
#elif defined(__WXOSX__)
    wxString stdPath  = (std_path.GetUserConfigDir() + s + _T("opencpn"));   // should be ~/Library/Preferences/opencpn
#endif

    return stdPath + wxFileName::GetPathSeparator() +
        _T("plugins") + wxFileName::GetPathSeparator() +
        _T("ensemble_weather") +  wxFileName::GetPathSeparator();

    stdPath += s + _T("plugins");
    if (!wxDirExists(stdPath))
      wxMkdir(stdPath);

    stdPath += s + _T("ensemble_weather");

#ifdef __WXOSX__
    // Compatibility with pre-OCPN-4.2; move config dir to
    // ~/Library/Preferences/opencpn if it exists
    wxString oldPath = (std_path.GetUserConfigDir() + s + _T("plugins") + s + _T("weather_routing"));
    if (wxDirExists(oldPath) && !wxDirExists(stdPath)) {
		wxLogMessage("ensemble_weather_pi: moving config dir %s to %s", oldPath, stdPath);
		wxRenameFile(oldPath, stdPath);
    }
#endif

    if (!wxDirExists(stdPath))
      wxMkdir(stdPath);

    stdPath += s;
    return stdPath;
}

void ensemble_weather_pi::ShowMenuItems(bool show)
{
    SetToolbarItemState( m_leftclick_tool_id, show );
    SetCanvasContextMenuItemViz(m_position_menu_id, show);
}
