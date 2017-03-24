#include <iostream>
#include "EnsembleWeatherManager.h"


void EnsembleWeatherManager::Reset(){
  m_time_index = 0;
  m_lon_index = 0;
  m_lat_index = 0;
  m_need_to_process_double_click = false;
  m_spot_plot = SpotPlot();
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

void EnsembleWeatherManager::Render(wrDC &wrdc, PlugIn_ViewPort &vp){
    if (!m_fcst.is_empty()) {
        if(!wrdc.GetDC()) {
            glPushAttrib(GL_LINE_BIT | GL_ENABLE_BIT | GL_HINT_BIT ); //Save state
            glEnable( GL_LINE_SMOOTH );
            glEnable( GL_BLEND );
            glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
            glHint( GL_LINE_SMOOTH_HINT, GL_NICEST );
        }

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

        if(!wrdc.GetDC())
            glPopAttrib();

        if (m_spot_plot.get_window()) {
            RequestRefresh(m_spot_plot.get_window());
        }
    }
}


void EnsembleWeatherManager::OnChartDoubleClick(wxMouseEvent &event) {
    m_recent_double_click_point = event.GetPosition();
    m_need_to_process_double_click = true;
    Refresh();
}


void EnsembleWeatherManager::OnSpotDoubleClick(wxMouseEvent &event) {
    int new_index = m_spot_plot.get_time_index(event.GetPosition());
    if (new_index >= 0 && new_index < (int) m_fcst.get_times().size()) {
        m_time_index = new_index;
    }
    Refresh();
}


void EnsembleWeatherManager::OnPaintSpot( wxPaintEvent& event ) {
    wxWindow *window = dynamic_cast<wxWindow*>(event.GetEventObject());
    if (!window)
        return;
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


void EnsembleWeatherManager::OnPrevTimeClick(wxCommandEvent& event) {
    m_time_index = (m_time_index - 1) % ((int) m_fcst.get_times().size());
    if (m_time_index < 0) {
      m_time_index += (int) m_fcst.get_times().size();
    }
    Refresh();
}


void EnsembleWeatherManager::OnNextTimeClick(wxCommandEvent& event) {
    m_time_index = (m_time_index + 1) % ((int) m_fcst.get_times().size());
    Refresh();
}
