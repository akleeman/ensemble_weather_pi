#include <iostream>
#include <wx/wx.h>

#include "wrdc.h"
#include "ocpn_plugin.h"
#include "WindCircle.h"
#include "EnsembleWeatherUI.h"
//#include "SlocumReader.h"


class EnsembleWeatherManager : public EnsembleWeatherUI
{
  public:

    EnsembleWeatherManager(wxWindow* parent,
                           wxWindowID id = wxID_ANY,
                           const wxString& title = wxT("Ensemble Weather"),
                           const wxPoint& pos = wxDefaultPosition,
                           const wxSize& size = wxSize( 389,62 ),
                           long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL)
    : EnsembleWeatherUI(parent, id, title, pos, size, style) {};

    void OnOpenFile(wxCommandEvent& event);

    void Reset();

    void Render(wrDC &wrdc, PlugIn_ViewPort &vp);

  private:
    WindCircleFactory m_circle_factory;
};
