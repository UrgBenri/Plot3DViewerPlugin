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

#ifndef COLORSCHEME_H
#define COLORSCHEME_H

#include <QColor>
#include <QVector>
#include <QMap>
#include <QList>
#include <functional>   // std::function

class ColorScheme
{
public:
    ColorScheme();
    ~ColorScheme();

    typedef enum {
        PLAIN_SCHEME = -1,
        VULCAN_SCHEME = 0,
        HOT_SCHEME,
        HSV_SCHEME,
        OCEAN_SCHEME,
        RAINBOW_SCHEME,
        TWILIGHT_SCHEME,
        GRAY_SCHEME,
    } color_scheme_t;

    void setScheme(color_scheme_t scheme);
    inline color_scheme_t getScheme() { return scheme; }
    void setLimits(float min_val, float max_val);
    inline float getLimitMin() { return min_limit; }
    inline float getLimitMax() { return max_limit; }
    QColor getColor(float value);
    QColor getColor(float value, float min_val, float max_val);
    static QColor getColor(float value, float min_val, float max_val, color_scheme_t scheme);

private:
    float min_limit;
    float max_limit;
    color_scheme_t scheme;
};

#endif // COLORSCHEME_H

