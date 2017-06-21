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

#include <QMouseEvent>
#include <limits>
#include <QShortcut>
#include <QApplication>
#include <QTime>
#include <QDebug>

#include "UrgDrawWidget3D.h"
#include "MathUtils.h"

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <Axis.h>
#include "GridPlaneXY.h"

namespace
{
const qreal MaxAxisRange = 100;
const qreal RenderUnit = 0.001;

enum {
    MinimumWidth = 100,
    MinimumHeight = 100,

    PointLength = 60 * 1000,     // [mm]
    MinimumRedrawInterval = 25,
    SnapMaxCount = 2,
};
}

UrgDrawWidget3D::UrgDrawWidget3D(QWidget* parent):
    QGLWidget(parent)
{

//    Viewport *vp = new Viewport(0.0, 0.0, 0.5, 0.5);
//    vp->setName("Camera");
//    vp->camera()->setViewMode(Camera::CameraViewMode);
//    m_scene.addViewport(vp);

//    vp = new Viewport(0.0, 0.5, 0.5, 0.5);
//    vp->setName("Front");
//    vp->camera()->setViewMode(Camera::FrontViewMode);
//    m_scene.addViewport(vp);

//    vp = new Viewport(0.5, 0.0, 0.5, 0.5);
//    vp->setName("Left");
//    vp->camera()->setViewMode(Camera::LeftViewMode);
//    m_scene.addViewport(vp);

//    vp = new Viewport(0.5, 0.5, 0.5, 0.5);
//    vp->setName("Top");
//    vp->camera()->setViewMode(Camera::TopViewMode);
//    m_scene.addViewport(vp);

    Viewport *vp = new Viewport(0.0, 0.0, 1.0, 1.0);
    vp->camera()->setViewMode(Camera::CameraViewMode);
    m_scene.addViewport(vp);

    m_scene.addItem(new Axis(-MaxAxisRange, MaxAxisRange
                             , -MaxAxisRange, MaxAxisRange
                             , -MaxAxisRange, MaxAxisRange, 1, 2));
    m_scene.addItem(new GridPlaneXY(-MaxAxisRange, MaxAxisRange, -MaxAxisRange, MaxAxisRange, 0, 1));

    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setMinimumSize(MinimumWidth, MinimumHeight);

    (void) new QShortcut(Qt::Key_Less, this, SLOT(smallerZoom()));
    (void) new QShortcut(Qt::Key_Comma, this, SLOT(smallerZoom()));
    (void) new QShortcut(Qt::Key_Greater, this, SLOT(largerZoom()));
    (void) new QShortcut(Qt::Key_Period, this, SLOT(largerZoom()));

    setMouseTracking(true);

    setAttribute(Qt::WA_AcceptTouchEvents);
}



UrgDrawWidget3D::~UrgDrawWidget3D(void)
{
    qDebug() << "UrgDrawWidget3D::~UrgDrawWidget3D";
}


void UrgDrawWidget3D::redraw(void)
{
    //    m_drawDirty = true;
    update();
}



void UrgDrawWidget3D::addMeasurementData(const PluginDataStructure &data)
{
    redraw();
}

void UrgDrawWidget3D::initializeGL(void)
{
    m_scene.init();
}


void UrgDrawWidget3D::resizeGL(int width, int height)
{
    m_scene.setSize(QSize(width, height));
}

void UrgDrawWidget3D::resizeEvent(QResizeEvent* event)
{
    QGLWidget::resizeEvent(event);

    resizeGL(event->size().width(),event->size().height());
    redraw();
}


void UrgDrawWidget3D::paintGL(void)
{
    m_scene.render();
}

void UrgDrawWidget3D::mousePressEvent(QMouseEvent* event)
{
    //    qDebug() << "UrgDrawWidget3D::mousePressEvent";
    m_scene.mousePressEvent(event);
    redraw();
}


void UrgDrawWidget3D::mouseMoveEvent(QMouseEvent* event)
{
    m_scene.mouseMoveEvent(event);
    redraw();
}

void UrgDrawWidget3D::wheelEvent(QWheelEvent* event)
{
    m_scene.wheelEvent(event);
    redraw();
}


void UrgDrawWidget3D::mouseReleaseEvent(QMouseEvent* event)
{
    //    qDebug() << "UrgDrawWidget3D::mouseReleaseEvent";
    m_scene.mouseReleaseEvent(event);
    redraw();
}

void UrgDrawWidget3D::smallerZoom(void)
{
    m_scene.zoomOut();
    redraw();
}


void UrgDrawWidget3D::largerZoom(void)
{
    m_scene.zoomIn();
    redraw();
}
