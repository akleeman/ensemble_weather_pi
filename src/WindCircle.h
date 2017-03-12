#include "wx/wxprec.h"

#ifndef  WX_PRECOMP
  #include "wx/wx.h"
#endif //precompiled headers

#include <wx/glcanvas.h>
#include <wx/graphics.h>
#include <wx/progdlg.h>

#include "wrdc.h"
#include "ocpn_plugin.h"
#include "LineBufferOverlay.h"

class PlugIn_ViewPort;


class WindCircleFactory {
public:
    WindCircleFactory();

    bool Render(wrDC &wrdc, PlugIn_ViewPort &vp);
    void Reset() {};
    void ClearCached() {};

private:

//    bool render_gl_grib_overlay(wxGLContext *pcontext, PlugIn_ViewPort *vp);
//    bool render_grib_overlay(wxDC &dc, PlugIn_ViewPort *vp);
//    void render_circles();

    LineBuffer m_circle;
};
