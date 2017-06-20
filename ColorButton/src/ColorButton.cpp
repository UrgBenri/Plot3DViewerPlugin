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

#include <ColorButton.h>
#include <QColorDialog>
#include <QPainter>
#include <QImage>
#include <QPixmap>
#include <QtDebug>

#include <stdio.h>

ColorButton::ColorButton(QWidget* parent) :
    QPushButton(parent)
{
    m_color = QColor(255, 255, 255);
    setAutoDefault(false);

    connect(this, &QAbstractButton::clicked, this, &ColorButton::clickedSLOT);
}

ColorButton::ColorButton(const QString &text, QWidget* parent) :
    QPushButton(text, parent)
{
    m_color = QColor(255, 255, 255);
    setAutoDefault(false);

    connect(this, &QAbstractButton::clicked, this, &ColorButton::clickedSLOT);
}

ColorButton::~ColorButton()
{
    qDebug() << "ColorButton::~ColorButton";
}

QColor
ColorButton::color() const
{
    return m_color;
}

void
ColorButton::setColor(const QColor &c)
{
    m_color = c;

    QImage img(16, 12, QImage::Format_ARGB32);
    img.fill(m_color.rgba());
    for (int i = 0; i < 16; ++i) {
        ((QRgb*)img.scanLine(0))[i] = 0;
        ((QRgb*)img.scanLine(11))[i] = 0;
    }

    for (int i = 0; i < 12; ++i) {
        ((QRgb*)img.scanLine(i))[0] = 0;
        ((QRgb*)img.scanLine(i))[15] = 0;
    }

    QPixmap pix;
    pix.convertFromImage(img);
    //  setPixmap( pix );
    QIcon icon(pix);
    QSize iconSize(pix.rect().size());
    setIconSize(iconSize);
    setIcon(icon);
}

void
ColorButton::clickedSLOT()
{
    QColor c = QColorDialog::getColor(color(), this , tr("Select Color"), QColorDialog::ShowAlphaChannel);

    if (c.isValid()) {
        setColor(c);

        emit valueChanged();
        emit valueChanged(c);
    }
}

/*
void
ColorButton::drawButtonLabel( QPainter *p )
{
  p->setBrush( color() );
  p->drawRoundRect( 6, 6, width()-12, height()-12, 30, 30 );
}
*/

