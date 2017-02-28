#include <wx/wx.h>

#include "wrdc.h"
#include "ocpn_plugin.h"
#include "GribUIDialogBase.h"


class EnsembleWeatherManager : public GRIBUICtrlBarBase
{
  public:

    EnsembleWeatherManager(wxWindow *parent);
    ~EnsembleWeatherManager();

    void Reset();

    void Render(wrDC &dc, PlugIn_ViewPort &vp);

};
