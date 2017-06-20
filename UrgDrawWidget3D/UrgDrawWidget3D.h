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

#ifndef QRK_URG_DRAW_WIDGET_H
#define QRK_URG_DRAW_WIDGET_H

//#include "Color.h"
#include <QGLWidget>
#include <memory>

#include <QPointF>
#include <QTimer>
#include <QVector>
#include <QTime>
#include <QReadWriteLock>
#include "PluginDataStructure.h"
#include "ColorScheme.h"

#include "Scene.h"

class UrgDrawWidget3D : public QGLWidget
{
    Q_OBJECT

public:
    UrgDrawWidget3D(QWidget* parent = 0);
    virtual ~UrgDrawWidget3D(void);

public slots:
    void smallerZoom();
    void largerZoom();
    void redraw(void);

    void addMeasurementData(const PluginDataStructure &data);

signals:
    void positionUpdated(bool active, long x_mm, long y_mm, int step);
    void positionClicked(bool active, long x_mm, long y_mm, int step);

    void error(const QString &header, const QString &message);
    void warning(const QString &header, const QString &message);
    void information(const QString &header, const QString &message);

protected:
    void initializeGL(void);
    void resizeGL(int width, int height);
    void paintGL(void);
    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void wheelEvent(QWheelEvent* event);
    void resizeEvent(QResizeEvent* event);

private:
    UrgDrawWidget3D(const UrgDrawWidget3D &rhs);
    UrgDrawWidget3D &operator = (const UrgDrawWidget3D &rhs);

    Scene m_scene;

};

#endif /* !QRK_URG_DRAW_WIDGET_H */

