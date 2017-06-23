/*
	This file is part of the UrgBenri application.

	Copyright (c) 2016 Mehrez Kristou.
	All rights reserved.

	Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

	1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

	2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

	3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

	Please contact kristou@hokuyo-aut.jp for more details.

*/

#include "ColorScheme.h"
#include <math.h>
#include <QDebug>

#ifndef M_PI
#define M_PI       3.14159265358979323846
#endif

typedef float (*colorFunc_t)(float x);

static QVector<std::function<float(float)>> colormap_functors;
static QMap<ColorScheme::color_scheme_t, QList<int>> color_schemes;

static void populateColorSchemes()
{
    static bool first_run = true;

    //if already populated, do nothing
    if (!first_run) {
        return;
    }
    first_run = false;

    //RGBFormulae from GNUPLOT
    //    0: 0               1: 0.5             2: 1
    //    3: x               4: x^2             5: x^3
    //    6: x^4             7: sqrt(x)         8: sqrt(sqrt(x))
    //    9: sin(90x)       10: cos(90x)       11: |x-0.5|
    //    12: (2x-1)^2       13: sin(180x)      14: |cos(180x)|
    //    15: sin(360x)      16: cos(360x)      17: |sin(360x)|
    //    18: |cos(360x)|    19: |sin(720x)|    20: |cos(720x)|
    //    21: 3x             22: 3x-1           23: 3x-2
    //    24: |3x-1|         25: |3x-2|         26: (3x-1)/2
    //    27: (3x-2)/2       28: |(3x-1)/2|     29: |(3x-2)/2|
    //    30: x/0.32-0.78125 31: 2*x-0.84       32: 4x;1;-2x+1.84;x/0.08-11.5
    //    33: |2*x - 0.5|    34: 2*x            35: 2*x - 0.5
    //    36: 2*x - 1
    colormap_functors.clear();
    colormap_functors.push_back([](float /*x*/) { return 0.0f; }); //0
    colormap_functors.push_back([](float /*x*/) { return 0.5f; }); //1
    colormap_functors.push_back([](float /*x*/) { return 1.0f; }); //2
    colormap_functors.push_back([](float x) { return x; }); //3
    colormap_functors.push_back([](float x) { return x*x; });  //4
    colormap_functors.push_back([](float x) { return x*x*x; }); //5
    colormap_functors.push_back([](float x) { return x*x*x*x; }); //6
    colormap_functors.push_back([](float x) { return sqrt(x); }); //7
    colormap_functors.push_back([](float x) { return sqrt(sqrt(x)); }); //8
    colormap_functors.push_back([](float x) { return sin(0.5*M_PI*x); }); //9
    colormap_functors.push_back([](float x) { return cos(0.5*M_PI*x); }); //10
    colormap_functors.push_back([](float x) { return fabs(x - 0.5); }); //11
    colormap_functors.push_back([](float x) { return (2.0*x - 1.0)*(2.0*x - 1.0); }); //12
    colormap_functors.push_back([](float x) { return sin(M_PI*x); }); //13
    colormap_functors.push_back([](float x) { return fabs(cos(M_PI*x)); }); //14
    colormap_functors.push_back([](float x) { return sin(2.0*M_PI*x); }); //15
    colormap_functors.push_back([](float x) { return cos(2.0*M_PI*x); }); //16
    colormap_functors.push_back([](float x) { return fabs(sin(2.0*M_PI*x)); }); //17
    colormap_functors.push_back([](float x) { return fabs(cos(2.0*M_PI*x)); }); //18
    colormap_functors.push_back([](float x) { return fabs(sin(4.0*M_PI*x)); }); //19
    colormap_functors.push_back([](float x) { return fabs(cos(4.0*M_PI*x)); }); //20
    colormap_functors.push_back([](float x) { return 3.0*x; }); //21
    colormap_functors.push_back([](float x) { return 3.0*x - 1.0; }); //22
    colormap_functors.push_back([](float x) { return 3.0*x - 2.0; }); //23
    colormap_functors.push_back([](float x) { return fabs(3.0*x - 1.0); }); //24
    colormap_functors.push_back([](float x) { return fabs(3.0*x - 2.0); }); //25
    colormap_functors.push_back([](float x) { return (3.0*x - 1.0)/2.0; }); //26
    colormap_functors.push_back([](float x) { return (3.0*x - 2.0)/2.0; }); //27
    colormap_functors.push_back([](float x) { return fabs(3.0*x - 1.0)/2.0; }); //28
    colormap_functors.push_back([](float x) { return fabs(3.0*x - 2.0)/2.0; }); //29
    colormap_functors.push_back([](float x) { return x/0.32 - 0.78125; }); //30
    colormap_functors.push_back([](float x) { return 2.0*x - 0.84; }); //31
    colormap_functors.push_back([](float x) { return ((x<0.25) ? (4.0*x) : ((x<0.42) ? (1.0) : ((x<0.92) ? (-2.0*x+1.84) : (x/0.08-11.5)))); }); //32
    colormap_functors.push_back([](float x) { return fabs(2.0*x - 0.5); }); //33
    colormap_functors.push_back([](float x) { return 2.0*x; }); //34
    colormap_functors.push_back([](float x) { return 2.0*x - 0.5; }); //35
    colormap_functors.push_back([](float x) { return 2.0*x - 1.0; }); //36

    //Color schemes
    QList<int> list;
    list.clear();
    list << 7 << 5 << 15;
    color_schemes[ColorScheme::VULCAN_SCHEME] = list;

    list.clear();
    list << 21 << 22 << 23;
    color_schemes[ColorScheme::HOT_SCHEME] = list;

    list.clear();
    list << 3 << 2 << 2;
    color_schemes[ColorScheme::HSV_SCHEME] = list;

    list.clear();
    list << 23 << 28 << 3;
    color_schemes[ColorScheme::OCEAN_SCHEME] = list;

    list.clear();
    list << 33 << 13 << 10;
    color_schemes[ColorScheme::RAINBOW_SCHEME] = list;

    list.clear();
    list << 30 << 31 << 32;
    color_schemes[ColorScheme::TWILIGHT_SCHEME] = list;

    list.clear();
    list << 3 << 3 << 3;
    color_schemes[ColorScheme::GRAY_SCHEME] = list;
}

ColorScheme::ColorScheme():
    min_limit(0.0),
    max_limit(1.0),
    scheme(PLAIN_SCHEME)
{
    populateColorSchemes();
}

ColorScheme::~ColorScheme()
{

}

void ColorScheme::setScheme(ColorScheme::color_scheme_t scheme)
{
    this->scheme = scheme;
}

void ColorScheme::setLimits(float min_val, float max_val)
{
    min_limit = qMin(min_val, max_val);
    max_limit = qMax(min_val, max_val);
}

QColor ColorScheme::getColor(float value) const
{
    QColor color;

    //if no color scheme selected, return some plain color
    if (scheme == PLAIN_SCHEME) {
        return QColor(255,0,0,0);
    }

    //convert value in the range 0..1
    value = (value - min_limit)/(max_limit - min_limit);
    value = qBound(0.0f, value, 1.0f);
    //generate the color according to the color scheme and color functors
    if (scheme == HSV_SCHEME) {
        color = QColor::fromHsvF(qBound(0.0f, colormap_functors[color_schemes[scheme].at(0)](value), 0.833f), //extreme HUEs in HSV are very similar, reduce range to 83.3%
                                 qBound(0.0f, colormap_functors[color_schemes[scheme].at(1)](value), 1.0f),
                                 qBound(0.0f, colormap_functors[color_schemes[scheme].at(2)](value), 1.0f),
                                 1.0);
    } else {
        color = QColor::fromRgbF(qBound(0.0f, colormap_functors[color_schemes[scheme].at(0)](value), 1.0f),
                                 qBound(0.0f, colormap_functors[color_schemes[scheme].at(1)](value), 1.0f),
                                 qBound(0.0f, colormap_functors[color_schemes[scheme].at(2)](value), 1.0f),
                                 1.0);
    }
    return color;
}

QColor ColorScheme::getColor(float value, float min_val, float max_val)
{
    setLimits(min_val, max_val);

    return getColor(value);
}

QColor ColorScheme::getColor(float value, float min_val, float max_val, color_scheme_t scheme)
{
    QColor color;

    //if no color scheme selected, return some plain color
    if (scheme == PLAIN_SCHEME) {
        return QColor(255,0,0,0);
    }

    //populate color schemes when necessary
    populateColorSchemes();

    //convert value in the range 0..1
    value = (value - min_val)/(max_val - min_val);
    value = qBound(0.0f, value, 1.0f);
    //generate the color according to the color scheme and color functors
    if (scheme == HSV_SCHEME) {
        color = QColor::fromHsvF(qBound(0.0f, colormap_functors[color_schemes[scheme].at(0)](value), 0.833f), //extreme HUEs in HSV are very similar, reduce range to 83.3%
                                 qBound(0.0f, colormap_functors[color_schemes[scheme].at(1)](value), 1.0f),
                                 qBound(0.0f, colormap_functors[color_schemes[scheme].at(2)](value), 1.0f),
                                 1.0);
    } else {
        color = QColor::fromRgbF(qBound(0.0f, colormap_functors[color_schemes[scheme].at(0)](value), 1.0f),
                                 qBound(0.0f, colormap_functors[color_schemes[scheme].at(1)](value), 1.0f),
                                 qBound(0.0f, colormap_functors[color_schemes[scheme].at(2)](value), 1.0f),
                                 1.0);
    }
    return color;
}

