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

#ifndef VIEWERSETTINGSWIDGET_H
#define VIEWERSETTINGSWIDGET_H

#include <QWidget>
#include <QButtonGroup>
#include <QSettings>

#include "QsKineticScroller.h"

class Plot3DViewerPlugin;

namespace Ui
{
class PlotSettingsWidget;
}

class PlotSettingsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PlotSettingsWidget(QWidget* parent = 0);
    ~PlotSettingsWidget();
    void setTarget(Plot3DViewerPlugin* viewer_);
    void saveState(QSettings &settings);
    void restoreState(QSettings &settings);

protected:
    void changeEvent(QEvent* e);

private:
    void setupConnections();
    void setupColors();
    void updateTarget();

    Ui::PlotSettingsWidget* ui;
    Plot3DViewerPlugin* m_target;

    QVector<QColor> distanceEchoLineColor;
    QVector<QColor> distanceEchoPointColor;
    QVector<QColor> intensityEchoLineColor;
    QVector<QColor> intensityEchoPointColor;

    QColor distanceEchoLineColorDefault;
    QColor distanceEchoPointColorDefault;
    QColor intensityEchoLineColorDefault;
    QColor intensityEchoPointColorDefault;

    QButtonGroup* distanceEchoLineColorGroup;
    QButtonGroup* distanceEchoPointColorGroup;
    QButtonGroup* distanceEchoShowGroup;
    QButtonGroup* intensityEchoLineColorGroup;
    QButtonGroup* intensityEchoPointColorGroup;
    QButtonGroup* intensityEchoShowGroup;

    QsKineticScroller m_scroller;

    int maxEchoes;

private slots:
    void startStepChanged(int step);
    void eEndStepChanged(int step);
    void minDistanceChanged(int dist);
    void maxDistanceChanged(int dist);
    void pointModeChanged(bool mode);
    void lineModeChanged(bool mode);
    void polygonModeChanged(bool mode);

    void distanceEchoLineColorChanged(int index);
    void distanceEchoPointColorChanged(int index);
    void distanceEchoShowChanged(int index);
    void distanceLineSizeChanged(int size);
    void distancePointSizeChanged(int size);

    void intensityEchoLineColorChanged(int index);
    void intensityEchoPointColorChanged(int index);
    void intensityEchoShowChanged(int index);
    void intensityLineSizeChanged(int size);
    void intensityPointSizeChanged(int size);

    void shadowSpinBoxChanged(int value);
    void refreshSpinBoxChanged(int val);


    void viewDataBoxChanged(bool state);
    void viewIntensityBoxChanged(bool state);
    void viewItensityOnTopBoxChanged(bool state);

    void rotationSpinBoxChanged(int rotation);
    void rotationDialChanged(int rotation);

    void invertedCheckBoxChanged(bool state);
    void intensityRatioChanged(int val);

    void echoSelectionChanged(int index);

    void colorModeChanged(int index);
    void colorSchemeChanged(int index);
};

#endif // VIEWERSETTINGSWIDGET_H

