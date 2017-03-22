#include <iostream>
#include "EnsembleWeatherManager.h"


void EnsembleWeatherManager::Reset(){
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
          double lon, lat;
          GetCanvasLLPix(&vp, m_recent_double_click_point, &lon, &lat);

          auto lons = m_fcst.get_lats();
          auto lats = m_fcst.get_lons();
          double min_dist = 1.e10;
          for (int i = 0; i <= lons->shape()[0]; i++){
              for (int j = 0; j <= lons->shape()[0]; j++){
                  double dist = (pow(lons->get(i, j) - lon, 2.) +
                                 pow(lats->get(i, j) - lat, 2.));
                  if (dist < min_dist) {
                      min_dist = dist;
                      m_lon_index = i;
                      m_lat_index = j;
                  }
              }
          }
          m_need_to_process_double_click = false;
        }

        m_wind_circle_factory.RenderGriddedForecast(wrdc, vp, &m_fcst,
                                                    m_time_index,
                                                    m_lon_index,
                                                    m_lat_index);

        tm *timeinfo = localtime(&m_fcst.get_times()[m_time_index]);
        m_time_text->SetLabel(asctime(timeinfo));

        if(!wrdc.GetDC())
            glPopAttrib();
    }
}


void EnsembleWeatherManager::OnChartDoubleClick(wxMouseEvent &event) {
  m_recent_double_click_point = event.GetPosition();
  m_need_to_process_double_click = true;
}


void EnsembleWeatherManager::OnPaintSpot( wxPaintEvent& event ) {
    wxWindow *window = dynamic_cast<wxWindow*>(event.GetEventObject());
    if(!window)
        return;

    wxPaintDC dc(window);
    dc.SetBackgroundMode(wxTRANSPARENT);

    wrDC wrdc(dc);
    SpotPlot spot_plotter;
    SpotForecast spot = m_fcst.get_spot(m_lon_index, m_lat_index);
    spot_plotter.plot(wrdc, &spot);
}


void EnsembleWeatherManager::OnOpenFile(wxCommandEvent& event) {
    wxString error;
    wxFileDialog openDialog(this,
                            _( "Select Forecast File" ),
                            _T(""), wxT ( "" ),
                            wxT ( "Forecast files (*.fcst)|*.fcst;*.fcst|All files (*.*)|*.*" ),
                            wxFD_OPEN);
    if(openDialog.ShowModal() == wxID_OK) {
      // Call slocum reader.
    }
}


void EnsembleWeatherManager::OnPrevTimeClick(wxCommandEvent& event) {
    m_time_index = (m_time_index - 1) % ((int) m_fcst.get_times().size());
    if (m_time_index < 0) {
      m_time_index += (int) m_fcst.get_times().size();
    }
    RequestRefresh(GetParent());
}


void EnsembleWeatherManager::OnNextTimeClick(wxCommandEvent& event) {
    m_time_index = (m_time_index + 1) % ((int) m_fcst.get_times().size());
    RequestRefresh(GetParent());
}
