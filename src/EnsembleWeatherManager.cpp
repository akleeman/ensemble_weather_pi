#include <iostream>
#include "EnsembleWeatherManager.h"

void EnsembleWeatherManager::Reset(){ }

void EnsembleWeatherManager::Render(wrDC &wrdc, PlugIn_ViewPort &vp){

  if(!wrdc.GetDC()) {
      glPushAttrib(GL_LINE_BIT | GL_ENABLE_BIT | GL_HINT_BIT ); //Save state
      glEnable( GL_LINE_SMOOTH );
      glEnable( GL_BLEND );
      glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
      glHint( GL_LINE_SMOOTH_HINT, GL_NICEST );
  }

  m_wind_circle_factory.Render(wrdc, vp, m_fcst);

  if(!wrdc.GetDC())
      glPopAttrib();
}

void EnsembleWeatherManager::OnPaintSpot( wxPaintEvent& event ) {
  wxWindow *window = dynamic_cast<wxWindow*>(event.GetEventObject());
  if(!window)
      return;

  wxPaintDC dc(window);
  dc.SetBackgroundMode(wxTRANSPARENT);

  wrDC wrdc(dc);

  wxColour test_color(255, 255, 255, 210);
  wxBrush brush(test_color, wxALPHA_OPAQUE);
  wrdc.SetBrush(brush);

  wrdc.DrawCircle(10, 10, 10);
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
