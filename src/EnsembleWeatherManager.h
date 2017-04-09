/******************************************************************************
 *
 * Project:  OpenCPN
 * Purpose:  Ensemble Weather Plugin
 * Author:   Alex Kleeman
 *
 ***************************************************************************
 *   Copyright (C) 2017 by Alex Kleeman                                 *
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
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************
 *
 */

#include <iostream>
#include <wx/wx.h>

#include "wrdc.h"
#include "ocpn_plugin.h"
#include "Slocum.h"
#include "SpotPlot.h"
#include "EnsembleWeatherUI.h"


class EnsembleWeatherManager : public EnsembleWeatherUI
{
  public:

    EnsembleWeatherManager(wxWindow* parent,
                           wxWindowID id = wxID_ANY,
                           const wxString& title = wxT("Ensemble Weather"),
                           const wxPoint& pos = wxDefaultPosition,
                           const wxSize& size = wxSize( 389,62 ),
                           long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL)
    : EnsembleWeatherUI(parent, id, title, pos, size, style) {
        Reset();
        LoadFile("/home/kleeman/Downloads/gridded_gefs_2017-04-03_1204.fcst");
//        LoadFile("/home/kleeman/Desktop/gridded_gefs_2017-03-22_1203.fcst");
    };

    void LoadFile(std::string filename);

    void OnClose(wxCloseEvent& event);

    void OnOpenFile(wxCommandEvent& event);

    void OnPrevTimeClick(wxCommandEvent& event);

    void OnNextTimeClick(wxCommandEvent& event);

    void OnPaintSpot(wxPaintEvent& event);

    void OnChartDoubleClick(wxMouseEvent &event);

    void OnSpotDoubleClick(wxMouseEvent &event);

    void Reset();

    void Refresh();

    void Render(wrDC &wrdc, PlugIn_ViewPort &vp);

    int m_time_index;
    int m_lon_index;
    int m_lat_index;
    EnsembleForecast m_fcst;

  private:

    SpotPlot m_spot_plot;
    wxPoint m_recent_double_click_point;
    bool m_need_to_process_double_click;
    WindCircleFactory m_wind_circle_factory;
};
