#include <iostream>
#include "WindCircle.h"


WindCircleFactory::WindCircleFactory() {

}



//bool WindCircleFactory::render_gl_grib_overlay(wxGLContext *pcontext, PlugIn_ViewPort *vp)
//{
//    // inform lower layers that this is OpenGL render
//    m_pdc = NULL;
//    return DoRenderGribOverlay( vp );
//}

//bool WindCircleFactory::render_grib_overlay(wxDC &dc, PlugIn_ViewPort *vp)
//{
//#if wxUSE_GRAPHICS_CONTEXT
//    wxMemoryDC *pmdc;
//    pmdc = wxDynamicCast(&dc, wxMemoryDC);
//    wxGraphicsContext *pgc = wxGraphicsContext::Create( *pmdc );
//    m_gdc = pgc;
//#endif
//    m_pdc = &dc;
//    return DoRenderGribOverlay(dc, vp);
//}


bool WindCircleFactory::Render(wrDC &wrdc, PlugIn_ViewPort &vp){

    wxPoint pl;
    GetCanvasPixLL(&vp, &pl, 0., 0.);

    //set default colour
    wxColour colour;
    GetGlobalColor( _T ( "DILG3" ), &colour );

    int arrowWidth = 2;
    glColor3ub(colour.Red(), colour.Green(), colour.Blue());
    glLineWidth(arrowWidth);

    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

    int r = 10.;
    double s = 2 * M_PI / 10.;
    for(double a = 0; a < 2 * M_PI; a += s) {
        m_circle.pushLine(r*sin(a) + pl.x, r*cos(a) + pl.y,
                          r*sin(a+s) + pl.x, r*cos(a+s) + pl.y);
    }
    m_circle.Finalize();

    if(wrdc.GetDC()) {
        wrdc.GetDC()->SetPen( wxPen( colour, 2 ) );
        wrdc.GetDC()->SetBrush( *wxTRANSPARENT_BRUSH);
    }
#ifdef ocpnUSE_GL
    else {
        glPushMatrix();
//        glTranslated(point.x, point.y, 0);
//        glRotated(vp.rotation*180/M_PI, 0, 0, 1);

        glColor3ub(colour.Red(), colour.Green(), colour.Blue());
        //      Enable anti-aliased lines, at best quality
        glEnable( GL_BLEND );
        glLineWidth( 2 );
        glEnableClientState(GL_VERTEX_ARRAY);
    }
#endif

    m_circle.draw(wrdc.GetDC());

#ifdef ocpnUSE_GL
    if(wrdc.GetDC()) {
        glDisableClientState(GL_VERTEX_ARRAY);
        glPopMatrix();
    }
#endif

    return true;
}
