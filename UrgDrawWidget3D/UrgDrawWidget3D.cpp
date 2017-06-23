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

#include "Axis.h"
#include "GridPlaneXY.h"
#include "Text.h"
#include "PointCloud.h"

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

UrgDrawWidget3D::UrgDrawWidget3D(QWidget* parent)
    : QGLWidget(parent)
    , m_clicpRange(30000)
{

    //    Viewport *vp = new Viewport();
    //    vp->setRect(QRectF(0.0, 0.0, 0.5, 0.5));
    //    vp->setName("Camera");
    //    vp->camera()->setViewMode(Camera::CameraViewMode);
    //    m_scene.addViewport(vp, "Camera");

    //    vp = new Viewport();
    //    vp->setRect(QRectF(0.0, 0.5, 0.5, 0.5));
    //    vp->setName("Front");
    //    vp->camera()->setViewMode(Camera::FrontViewMode);
    //    m_scene.addViewport(vp, "Front");

    //    vp = new Viewport();
    //    vp->setRect(QRectF(0.5, 0.0, 0.5, 0.5));
    //    vp->setName("Left");
    //    vp->camera()->setViewMode(Camera::LeftViewMode);
    //    m_scene.addViewport(vp, "Left");

    //    vp = new Viewport();
    //    vp->setRect(QRectF(0.5, 0.5, 0.5, 0.5));
    //    vp->setName("Top");
    //    vp->camera()->setViewMode(Camera::TopViewMode);
    //    m_scene.addViewport(vp, "Top");

    Viewport *vp = new Viewport();
    vp->setRect(QRectF(0.0, 0.0, 1.0, 1.0));
    vp->camera()->setViewMode(Camera::CameraViewMode);
    m_scene.addViewport(vp, "Main");

    vp = new Viewport();
    vp->setRect(QRectF(0.75, 0.75, 0.25, 0.25));
    vp->camera()->setViewMode(Camera::TopViewMode);
    m_scene.addViewport(vp, "View");

    m_scene.addItem(new Axis(Q_NULLPTR, -MaxAxisRange, -MaxAxisRange, -MaxAxisRange
                             , MaxAxisRange, MaxAxisRange, MaxAxisRange, 1, 2, false)
                    , "Axis");
    m_scene.addItem(new GridPlaneXY(Q_NULLPTR, -MaxAxisRange, MaxAxisRange, -MaxAxisRange, MaxAxisRange, 0, 1),
                    "Grid");

    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setMinimumSize(MinimumWidth, MinimumHeight);
}



UrgDrawWidget3D::~UrgDrawWidget3D(void)
{
    qDebug() << "UrgDrawWidget3D::~UrgDrawWidget3D";
}


void UrgDrawWidget3D::redraw(void)
{
    update();
}

void UrgDrawWidget3D::setColorScheme(int scheme)
{
    m_colorModel.setColorScheme(scheme);
}

void UrgDrawWidget3D::setColoringMode(int mode)
{
    m_colorModel.setColoringMode(mode);
}

void UrgDrawWidget3D::addMeasurementData(const PluginDataStructure &data)
{
    if(data.converter){
        PointCloud *pc = new PointCloud();
        QVector<QVector<QVector3D> > rawPoints = data.converter->getPoints(data.ranges);
        QVector<QVector4D> points;
        points.reserve(data.ranges.size() *3);
        std::for_each(rawPoints.begin(), rawPoints.end(), [&points](const QVector<QVector3D> &inPoints){
            std::for_each(inPoints.begin(), inPoints.end(), [&points](const QVector3D &point){
                points << QVector4D(point, 0);
            });
        });
        if(data.levels.size() > 0){
            int pos = 0;
            std::for_each(data.levels.begin(), data.levels.end(), [&points, &pos](const QVector<long> &levels){
                std::for_each(levels.begin(), levels.end(), [&points, &pos](long level){
                    if(pos < points.size()){
                        points[pos].setW(level);
                    }
                    pos++;
                });
            });
        }

        int pos = 0;
        float clipRange = m_clicpRange;
        std::for_each(data.ranges.begin(), data.ranges.end(), [&points, &pos, clipRange](const QVector<long> &ranges){
            std::for_each(ranges.begin(), ranges.end(), [&points, &pos, clipRange](long range){
                if(pos < points.size()){
                    if(range > clipRange){
                        points[pos].setX(0);
                        points[pos].setY(0);
                        points[pos].setZ(0);
                    }
                }
                pos++;
            });
        });
        m_colorModel.setMinMaxCoordinates(points);
        pc->setColorModel(&m_colorModel);
        pc->setPoints(points);
        pc->setPointSize(m_pointSize);
        m_scene.addItem(pc, "Scan");
        update();
    }
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
    update();
}

float UrgDrawWidget3D::clicpRange() const
{
    return m_clicpRange;
}

void UrgDrawWidget3D::setClicpRange(float clicpRange)
{
    m_clicpRange = clicpRange;
}

float UrgDrawWidget3D::pointSize() const
{
    return m_pointSize;
}

void UrgDrawWidget3D::setPointSize(float pointSize)
{
    m_pointSize = pointSize;
}


void UrgDrawWidget3D::paintGL(void)
{
    m_scene.render();
}

void UrgDrawWidget3D::mousePressEvent(QMouseEvent* event)
{
    m_scene.mousePressEvent(event);
    update();
}


void UrgDrawWidget3D::mouseMoveEvent(QMouseEvent* event)
{
    m_scene.mouseMoveEvent(event);
    update();
}

void UrgDrawWidget3D::wheelEvent(QWheelEvent* event)
{
    m_scene.wheelEvent(event);
    update();
}


void UrgDrawWidget3D::mouseReleaseEvent(QMouseEvent* event)
{
    m_scene.mouseReleaseEvent(event);
    update();
}

void UrgDrawWidget3D::smallerZoom(void)
{
    m_scene.zoomOut();
    update();
}


void UrgDrawWidget3D::largerZoom(void)
{
    m_scene.zoomIn();
    update();
}
