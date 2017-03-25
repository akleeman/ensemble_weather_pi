#include <iostream>
#include "EnsembleWeatherManager.h"


void EnsembleWeatherManager::Reset(){
  m_time_index = 0;
  m_lon_index = 0;
  m_lat_index = 0;
  m_need_to_process_double_click = false;
  m_spot_plot = SpotPlot();
  m_fcst = EnsembleForecast();
}


void EnsembleWeatherManager::Refresh() {
  if (m_spot_plot.get_window()) {
      RequestRefresh(m_spot_plot.get_window());
  }
  RequestRefresh(GetParent());
}


void EnsembleWeatherManager::LoadFile(std::string filename) {
    m_fcst = read_slocum_forecast(filename);
    wxSize s = GetSize();
    s.x = m_fcst.get_times().size() * 35;
    s.y = 350;
    SetSize(s);
}


/*
 * Here we render the gridded forecast overlay.
 */
void EnsembleWeatherManager::Render(wrDC &wrdc, PlugIn_ViewPort &vp){
    if (!m_fcst.is_empty()) {
        // If we're using GL we need to intialize before drawing and
        // everything get's computed in a buffer, then popped to the
        // screen.
        if(!wrdc.GetDC()) {
            glPushAttrib(GL_LINE_BIT | GL_ENABLE_BIT | GL_HINT_BIT ); //Save state
            glEnable( GL_LINE_SMOOTH );
            glEnable( GL_BLEND );
            glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
            glHint( GL_LINE_SMOOTH_HINT, GL_NICEST );
        }

        // Deal with the (possible) case where the user double clicked
        // on the gridded forecast.
        if (m_need_to_process_double_click) {

          // find out where the user clicked.
          double lon, lat;
          GetCanvasLLPix(&vp, m_recent_double_click_point, &lat, &lon);

          // this looks up the indices into a set of lons / lats grids
          // that are closest to the provided lon / lat.
          nearest(m_fcst.get_lons(), m_fcst.get_lats(),
                  lon, lat,
                  &m_lon_index, &m_lat_index);

          m_need_to_process_double_click = false;
        }

        m_wind_circle_factory.RenderGriddedForecast(wrdc, vp, &m_fcst,
                                                    m_time_index,
                                                    m_lon_index,
                                                    m_lat_index);

        // If we're using GL we then pop the buffered display to the screen.
        if(!wrdc.GetDC())
            glPopAttrib();

        if (m_spot_plot.get_window()) {
            RequestRefresh(m_spot_plot.get_window());
        }
    }
}


/*
 * When the user double clicks on the chart the grid nearest to their
 * click is made active and used in the spot forecast.
 */
void EnsembleWeatherManager::OnChartDoubleClick(wxMouseEvent &event) {
    // In order to determine where the user clicked we need to have the
    // plugin viewport around, which we don't at this point!  Instead
    // we set a flag, tell everything to refresh and wait until the
    // Render method is called to process the click.
    m_recent_double_click_point = event.GetPosition();
    m_need_to_process_double_click = true;
    Refresh();
}


/*
 * This handles the case when the user double clicks on the spot forecast,
 * if they double clicked on a specific time it updates both the spot
 * forecast and the gridded forecast plots.
 */
void EnsembleWeatherManager::OnSpotDoubleClick(wxMouseEvent &event) {
    if (!m_fcst.is_empty()) {
        int new_index = m_spot_plot.get_time_index(event.GetPosition());
        if (new_index >= 0 && new_index < (int) m_fcst.get_times().size()) {
            m_time_index = new_index;
        }
        Refresh();
    }
}


/*
 * This gets called whenever the spot forecast plot gets refreshed.
 */
void EnsembleWeatherManager::OnPaintSpot( wxPaintEvent& event ) {
    wxWindow *window = dynamic_cast<wxWindow*>(event.GetEventObject());
    if (!window)
        return;
    if (!m_fcst.is_empty()) {
        SpotForecast spot = m_fcst.get_spot(m_lon_index, m_lat_index);
        m_spot_plot.plot(window, &spot, m_time_index);

        char s[100];
        wxDateTime fcst_time;
        fcst_time = wxDateTime((*(spot.get_times()))[m_time_index]);
        wxString time_string = fcst_time.FormatISOCombined();
        std::string time_std_string = (std::string) time_string;

        sprintf(s, "Forecast for %3.1f, %4.1f at %s", spot.get_lat(), spot.get_lon(), time_std_string.c_str());
        m_time_text->SetLabel(s);
    }
}


/*
 * Open a file dialog and load the resulting forecast file.
 */
void EnsembleWeatherManager::OnOpenFile(wxCommandEvent& event) {
    wxString error;
    wxFileDialog openDialog(this,
                            _( "Select Forecast File" ),
                            _T(""), wxT ( "" ),
                            wxT ( "Forecast files (*.fcst)|*.fcst;*.fcst|All files (*.*)|*.*" ),
                            wxFD_OPEN);
    if(openDialog.ShowModal() == wxID_OK) {
        wxString filename = openDialog.GetPath();
        LoadFile((std::string) filename);
    }
}


/*
 * Shift to the previous forecast.
 */
void EnsembleWeatherManager::OnPrevTimeClick(wxCommandEvent& event) {
    if (!m_fcst.is_empty()) {
        m_time_index = (m_time_index - 1) % ((int) m_fcst.get_times().size());
        if (m_time_index < 0) {
          m_time_index += (int) m_fcst.get_times().size();
        }
        Refresh();
    }
}


/*
 * Shift to the next forecast.
 */
void EnsembleWeatherManager::OnNextTimeClick(wxCommandEvent& event) {
    if (!m_fcst.is_empty()) {
        m_time_index = (m_time_index + 1) % ((int) m_fcst.get_times().size());
        Refresh();
    }
}
