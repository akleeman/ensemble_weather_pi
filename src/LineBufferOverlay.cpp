/***************************************************************************
 *
 * Project:  OpenCPN Weather Routing plugin
 * Author:   Sean D'Epagnier
 *
 ***************************************************************************
 *   Copyright (C) 2015 by Sean D'Epagnier                                 *
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
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301,  USA.         *
 ***************************************************************************
 */
#include <iostream>
#include <wx/wx.h>
#include "ocpn_plugin.h"

#include "LineBufferOverlay.h"

void LineBuffer::pushLine( float x0, float y0, float x1, float y1 )
{
    buffer.push_back(x0);
    buffer.push_back(y0);
    buffer.push_back(x1);
    buffer.push_back(y1);
}

void LineBuffer::Finalize()
{
    count = buffer.size() / 4;
    delete [] lines;
    lines = new float[buffer.size()];
    int i = 0;
    for(std::list <float>::iterator it = buffer.begin(); it != buffer.end(); it++)
        lines[i++] = *it;

    buffer.clear();
}

void LineBuffer::pushTransformedBuffer(LineBuffer &buffer, int x, int y, double ang, bool south)
{
    // transform vertexes by angle
    float sa = sinf( ang ), ca = cosf( ang );

    float m[2][2] = {{ ca, -sa},
                     { sa,  ca}};

    if(south)
        m[0][0] = -m[0][0], m[1][0] = -m[1][0];

    for(int i=0; i < 2*buffer.count; i+=2) {
        float *k = buffer.lines + 2*i;
        pushLine(k[0]*m[0][0] + k[1]*m[0][1] + x, k[0]*m[1][0] + k[1]*m[1][1] + y,
                 k[2]*m[0][0] + k[3]*m[0][1] + x, k[2]*m[1][0] + k[3]*m[1][1] + y);
    }
}

void LineBuffer::draw(wxDC *dc)
{
    if( dc ) {
        for(int i=0; i < count; i++) {
            float *l = lines + 4*i;
#if wxUSE_GRAPHICS_CONTEXT && 0
            if( m_hiDefGraphics && m_gdc )
                m_gdc->StrokeLine( l[0], l[1], l[2], l[3] );
            else
#endif
                dc->DrawLine( l[0], l[1], l[2], l[3] );
        }
    } else {                       // OpenGL mode
#ifdef ocpnUSE_GL
        glVertexPointer(2, GL_FLOAT, 2*sizeof(float), lines);
        glDrawArrays(GL_LINES, 0, 2*count);
#endif
    }
}

//
//void LineBuffer::drawTransformed(LineBuffer &buffer, int x, int y, double ang, bool south)
//{
//    // transform vertexes by angle
//    float six = sinf( ang ), cox = cosf( ang ), siy, coy;
//    if(south)
//        siy = -six, coy = -cox;
//    else
//        siy = six, coy = cox;
//
//    float vertexes[40];
//
//    wxASSERT(sizeof vertexes / sizeof *vertexes >= (unsigned)buffer.count*4);
//    for(int i=0; i < 2*buffer.count; i++) {
//        float *k = buffer.lines + 2*i;
//        vertexes[2*i+0] = k[0]*cox + k[1]*siy + x;
//        vertexes[2*i+1] = k[0]*six - k[1]*coy + y;
//    }
//
//    if( m_pdc ) {
//        for(int i=0; i < buffer.count; i++) {
//            float *l = vertexes + 4*i;
//#if wxUSE_GRAPHICS_CONTEXT
//            if( m_hiDefGraphics && m_gdc )
//                m_gdc->StrokeLine( l[0], l[1], l[2], l[3] );
//            else
//#endif
//                m_pdc->DrawLine( l[0], l[1], l[2], l[3] );
//        }
//    } else {                       // OpenGL mode
//#ifdef ocpnUSE_GL
//        glVertexPointer(2, GL_FLOAT, 2*sizeof(float), vertexes);
//        glDrawArrays(GL_LINES, 0, 2*buffer.count);
//#endif
//    }
//}
