#include "EnsembleWeatherManager.h"


void EnsembleWeatherManager::Reset(){ }

void EnsembleWeatherManager::Render(wrDC &dc, PlugIn_ViewPort &vp){ }

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
