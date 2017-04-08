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

#include <iostream>
#include <cmath>
#include <vector>

/*
 * Returns the index into a sorted array of values which
 * indexes where in the array the `bisection` value should
 * be placed to preserve order.
 */
int bisect(std::vector<double> values, double bisection);

/*
 * Returns the index in values that holds the minimum value
 * of the vector
 */
int argmin(std::vector<double> values);

/*
 * Returns the indices that would sort values.
 */
std::vector<int> argsort(std::vector<double> values);

/*
 * Returns the minimu value ignoring any nans.
 */
double nanmin(std::vector<double> values);

/*
 * Returns the maximum value ignoring any nans.
 */
double nanmax(std::vector<double> values);
