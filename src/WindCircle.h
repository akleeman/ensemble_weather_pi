/******************************************************************************
 *
 * Project:  OpenCPN
 * Purpose:  Ensemble Weather Plugin
 * Author:   Alex Kleeman
 *
 ***************************************************************************
 *   Copyright (C) 2017 by Alex Kleeman                                 *
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
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************
 *
 */

#include "wx/wxprec.h"

#ifndef  WX_PRECOMP
  #include "wx/wx.h"
#endif //precompiled headers

#include <wx/glcanvas.h>
#include <wx/graphics.h>
#include <wx/progdlg.h>

#include <cmath>
#include "wrdc.h"
#include "ocpn_plugin.h"
#include "GriddedData.h"
#include "VectorUtilities.h"
#include "Slocum.h"

class PlugIn_ViewPort;
class EnsembleForecast;


class WindCircleFactory {
public:

    WindCircleFactory();

    bool RenderGriddedForecast(wrDC &wrdc, PlugIn_ViewPort &vp,
                               EnsembleForecast *fcst, int time_idx,
                               int lon_idx, int lat_idx);

    void RenderWindTriangle(wxPoint center,
                            double speed,
                            double direction,
                            double radius,
                            wrDC &wrdc);

    void RenderBackgroundCircle(wxPoint center,
                                double radius,
                                wrDC &wrdc);

    void RenderPoint(wrDC &wrdc,
                     wxPoint center,
                     std::vector<double> speeds,
                     std::vector<double> directions,
                     double radius);

    void Reset() {};

    void ClearCached() {};

private:

    wxColour m_circle_fill;
};
