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

#include "PlotSettingsWidget.h"
#include "ui_PlotSettingsWidget.h"

#include <QDebug>
#include <QColorDialog>
#include <QSettings>
#include <iostream>
#include <QStandardPaths>
#include "Plot3DViewerPlugin.h"
#include "PixmapDelegate.h"
#include "ColorScheme.h"

PlotSettingsWidget::PlotSettingsWidget(QWidget* parent) :
    QWidget(parent),
    m_scroller(this),
    ui(new Ui::PlotSettingsWidget),

    distanceEchoLineColorDefault(Qt::green),
    distanceEchoPointColorDefault(Qt::red),
    intensityEchoLineColorDefault(Qt::magenta),
    intensityEchoPointColorDefault(Qt::red),
    distanceEchoLineColorGroup(new QButtonGroup(this)),
    distanceEchoPointColorGroup(new QButtonGroup(this)),
    distanceEchoShowGroup(new QButtonGroup(this)),
    intensityEchoLineColorGroup(new QButtonGroup(this)),
    intensityEchoPointColorGroup(new QButtonGroup(this)),
    intensityEchoShowGroup(new QButtonGroup(this))
{
    ui->setupUi(this);
    m_target = NULL;

    maxEchoes = 5;

//    m_scroller.enableKineticScrollFor(ui->scrollArea);
    setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);

    setupConnections();

    setupColors();
}

void PlotSettingsWidget::setupColors()
{
    //---------------init QVectors;
    for (int i = 0; i < maxEchoes; i++) {
        distanceEchoLineColor.push_back(distanceEchoLineColorDefault);
        distanceEchoPointColor.push_back(distanceEchoPointColorDefault);
        intensityEchoLineColor.push_back(intensityEchoLineColorDefault);
        intensityEchoPointColor.push_back(intensityEchoPointColorDefault);
    }

    //----------------------label init
    ui->distanceEcho1LineColor->setText("1");
    ui->distanceEcho2LineColor->setText("2");
    ui->distanceEcho3LineColor->setText("3");
    ui->distanceEcho4LineColor->setText("4");
    ui->distanceEcho5LineColor->setText("5");

    ui->distanceEcho1PointColor->setText("1");
    ui->distanceEcho2PointColor->setText("2");
    ui->distanceEcho3PointColor->setText("3");
    ui->distanceEcho4PointColor->setText("4");
    ui->distanceEcho5PointColor->setText("5");

    ui->intensityEcho1LineColor->setText("1");
    ui->intensityEcho2LineColor->setText("2");
    ui->intensityEcho3LineColor->setText("3");
    ui->intensityEcho4LineColor->setText("4");
    ui->intensityEcho5LineColor->setText("5");

    ui->intensityEcho1PointColor->setText("1");
    ui->intensityEcho2PointColor->setText("2");
    ui->intensityEcho3PointColor->setText("3");
    ui->intensityEcho4PointColor->setText("4");
    ui->intensityEcho5PointColor->setText("5");

    //-------------------color init

    ui->distanceEcho1LineColor->setColor(distanceEchoLineColor[0]);
    ui->distanceEcho2LineColor->setColor(distanceEchoLineColor[1]);
    ui->distanceEcho3LineColor->setColor(distanceEchoLineColor[2]);
    ui->distanceEcho4LineColor->setColor(distanceEchoLineColor[3]);
    ui->distanceEcho5LineColor->setColor(distanceEchoLineColor[4]);

    ui->distanceEcho1PointColor->setColor(distanceEchoPointColor[0]);
    ui->distanceEcho2PointColor->setColor(distanceEchoPointColor[1]);
    ui->distanceEcho3PointColor->setColor(distanceEchoPointColor[2]);
    ui->distanceEcho4PointColor->setColor(distanceEchoPointColor[3]);
    ui->distanceEcho5PointColor->setColor(distanceEchoPointColor[4]);

    ui->intensityEcho1LineColor->setColor(intensityEchoLineColor[0]);
    ui->intensityEcho2LineColor->setColor(intensityEchoLineColor[1]);
    ui->intensityEcho3LineColor->setColor(intensityEchoLineColor[2]);
    ui->intensityEcho4LineColor->setColor(intensityEchoLineColor[3]);
    ui->intensityEcho5LineColor->setColor(intensityEchoLineColor[4]);

    ui->intensityEcho1PointColor->setColor(intensityEchoPointColor[0]);
    ui->intensityEcho2PointColor->setColor(intensityEchoPointColor[1]);
    ui->intensityEcho3PointColor->setColor(intensityEchoPointColor[2]);
    ui->intensityEcho4PointColor->setColor(intensityEchoPointColor[3]);
    ui->intensityEcho5PointColor->setColor(intensityEchoPointColor[4]);

    //----------------------goupping

    distanceEchoLineColorGroup->setExclusive(false);
    distanceEchoLineColorGroup->addButton(ui->distanceEcho1LineColor, 0);
    distanceEchoLineColorGroup->addButton(ui->distanceEcho2LineColor, 1);
    distanceEchoLineColorGroup->addButton(ui->distanceEcho3LineColor, 2);
    distanceEchoLineColorGroup->addButton(ui->distanceEcho4LineColor, 3);
    distanceEchoLineColorGroup->addButton(ui->distanceEcho5LineColor, 4);

    distanceEchoPointColorGroup->setExclusive(false);
    distanceEchoPointColorGroup->addButton(ui->distanceEcho1PointColor, 0);
    distanceEchoPointColorGroup->addButton(ui->distanceEcho2PointColor, 1);
    distanceEchoPointColorGroup->addButton(ui->distanceEcho3PointColor, 2);
    distanceEchoPointColorGroup->addButton(ui->distanceEcho4PointColor, 3);
    distanceEchoPointColorGroup->addButton(ui->distanceEcho5PointColor, 4);

    intensityEchoLineColorGroup->setExclusive(false);
    intensityEchoLineColorGroup->addButton(ui->intensityEcho1LineColor, 0);
    intensityEchoLineColorGroup->addButton(ui->intensityEcho2LineColor, 1);
    intensityEchoLineColorGroup->addButton(ui->intensityEcho3LineColor, 2);
    intensityEchoLineColorGroup->addButton(ui->intensityEcho4LineColor, 3);
    intensityEchoLineColorGroup->addButton(ui->intensityEcho5LineColor, 4);

    intensityEchoPointColorGroup->setExclusive(false);
    intensityEchoPointColorGroup->addButton(ui->intensityEcho1PointColor, 0);
    intensityEchoPointColorGroup->addButton(ui->intensityEcho2PointColor, 1);
    intensityEchoPointColorGroup->addButton(ui->intensityEcho3PointColor, 2);
    intensityEchoPointColorGroup->addButton(ui->intensityEcho4PointColor, 3);
    intensityEchoPointColorGroup->addButton(ui->intensityEcho5PointColor, 4);

    //----------------------visibility groupping

    distanceEchoShowGroup->setExclusive(false);
    distanceEchoShowGroup->addButton(ui->distanceEcho1Show, 0);
    distanceEchoShowGroup->addButton(ui->distanceEcho2Show, 1);
    distanceEchoShowGroup->addButton(ui->distanceEcho3Show, 2);
    distanceEchoShowGroup->addButton(ui->distanceEcho4Show, 3);
    distanceEchoShowGroup->addButton(ui->distanceEcho5Show, 4);

    intensityEchoShowGroup->setExclusive(false);
    intensityEchoShowGroup->addButton(ui->intensityEcho1Show, 0);
    intensityEchoShowGroup->addButton(ui->intensityEcho2Show, 1);
    intensityEchoShowGroup->addButton(ui->intensityEcho3Show, 2);
    intensityEchoShowGroup->addButton(ui->intensityEcho4Show, 3);
    intensityEchoShowGroup->addButton(ui->intensityEcho5Show, 4);

    //-----------------------color schemes
    ui->colorscheme->setItemDelegate(new PixmapDelegate(ui->colorscheme));
    for (int i = 0; i < 7; ++i) {
        QPixmap pixmap(QString(":/Plot3DViewerPlugin/scheme%1").arg(i+1));
        pixmap = pixmap.scaled(QSize(131,20),Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        QString name = "";
        switch (i) {
            case ColorScheme::VULCAN_SCHEME:
                name = "Vulcan";
                break;
            case ColorScheme::HOT_SCHEME:
                name = "Hot";
                break;
            case ColorScheme::HSV_SCHEME:
                name = "HSV";
                break;
            case ColorScheme::OCEAN_SCHEME:
                name = "Ocean";
                break;
            case ColorScheme::RAINBOW_SCHEME:
                name = "Rainbow";
                break;
            case ColorScheme::TWILIGHT_SCHEME:
                name = "Twilight";
                break;
            case ColorScheme::GRAY_SCHEME:
                name = "Gray";
                break;
        }
        ui->colorscheme->addItem(name, pixmap);
    }
}

PlotSettingsWidget::~PlotSettingsWidget()
{
    qDebug() << "PlotSettingsWidget::~PlotSettingsWidget";
    delete ui;
}

void PlotSettingsWidget::setTarget(Plot3DViewerPlugin *viewer_)
{
    m_target = viewer_;

    updateTarget();
}

void PlotSettingsWidget::updateTarget()
{
    if(m_target){
        m_target->setDistanceLineColor(0, ui->distanceEcho1LineColor->color());
        m_target->setDistancePointColor(0,  ui->distanceEcho1PointColor->color());
        m_target->setDistanceShow(0, ui->distanceEcho1Show->isChecked());
        m_target->setIntensityLineColor(0, ui->intensityEcho1LineColor->color());
        m_target->setIntensityPointColor(0, ui->intensityEcho1PointColor->color());
        m_target->setIntensityShow(0, ui->intensityEcho1Show->isChecked());

        m_target->setDistanceLineColor(1, ui->distanceEcho2LineColor->color());
        m_target->setDistancePointColor(1,  ui->distanceEcho2PointColor->color());
        m_target->setDistanceShow(1, ui->distanceEcho2Show->isChecked());
        m_target->setIntensityLineColor(1, ui->intensityEcho2LineColor->color());
        m_target->setIntensityPointColor(1, ui->intensityEcho2PointColor->color());
        m_target->setIntensityShow(1, ui->intensityEcho2Show->isChecked());

        m_target->setDistanceLineColor(2, ui->distanceEcho3LineColor->color());
        m_target->setDistancePointColor(2,  ui->distanceEcho3PointColor->color());
        m_target->setDistanceShow(2, ui->distanceEcho3Show->isChecked());
        m_target->setIntensityLineColor(2, ui->intensityEcho3LineColor->color());
        m_target->setIntensityPointColor(2, ui->intensityEcho3PointColor->color());
        m_target->setIntensityShow(2, ui->intensityEcho3Show->isChecked());

        m_target->setDistanceLineColor(3, ui->distanceEcho4LineColor->color());
        m_target->setDistancePointColor(3,  ui->distanceEcho4PointColor->color());
        m_target->setDistanceShow(3, ui->distanceEcho4Show->isChecked());
        m_target->setIntensityLineColor(3, ui->intensityEcho4LineColor->color());
        m_target->setIntensityPointColor(3, ui->intensityEcho4PointColor->color());
        m_target->setIntensityShow(3, ui->intensityEcho4Show->isChecked());

        m_target->setDistanceLineColor(4, ui->distanceEcho5LineColor->color());
        m_target->setDistancePointColor(4,  ui->distanceEcho5PointColor->color());
        m_target->setDistanceShow(4, ui->distanceEcho5Show->isChecked());
        m_target->setIntensityLineColor(4, ui->intensityEcho5LineColor->color());
        m_target->setIntensityPointColor(4, ui->intensityEcho5PointColor->color());
        m_target->setIntensityShow(4, ui->intensityEcho5Show->isChecked());

        m_target->showDistance(ui->distanceGroupBox->isChecked());
        m_target->ShowIntensity(ui->intensityGroupBox->isChecked());
        m_target->intensityOnTop(!ui->viewItensityOnTopBox->isChecked());
        m_target->setRotationOffset(ui->rotationSpinBox->value());

        if (ui->lineModeRadio->isChecked()) {
            m_target->setDrawingMode(Plot3DViewerPlugin::Lines);
        }
        if (ui->pointModeRadio->isChecked()) {
            m_target->setDrawingMode(Plot3DViewerPlugin::Points);
        }
        if (ui->polygonModeRadio->isChecked()) {
            m_target->setDrawingMode(Plot3DViewerPlugin::Polygon);
        }

        m_target->setDistanceLineSize(ui->distanceLineSizeBox->value());
        m_target->setDistancePointSize(ui->distancePointSizeBox->value());

        m_target->setIntensityLineSize(ui->intensityLineSizeBox->value());
        m_target->setIntensityPointSize(ui->intensityPointSizeBox->value());
        m_target->setIntensityRatio(ui->intensityRatio->value());

        m_target->setRefreshRate(ui->refreshSpinBox->value());
        m_target->setDrawPeriod(ui->shadowSpinBox->value());

        m_target->setEchoSelection(ui->echoSelection->currentIndex());

        m_target->setColorScheme(ui->colormode->currentIndex() > 0, ui->colorscheme->currentIndex());

        m_target->refresh();
    }
}

void PlotSettingsWidget::saveState(QSettings &settings)
{
    settings.setValue("distanceEcho1LineColor", ui->distanceEcho1LineColor->color());
    settings.setValue("distanceEcho2LineColor", ui->distanceEcho2LineColor->color());
    settings.setValue("distanceEcho3LineColor", ui->distanceEcho3LineColor->color());
    settings.setValue("distanceEcho4LineColor", ui->distanceEcho4LineColor->color());
    settings.setValue("distanceEcho5LineColor", ui->distanceEcho5LineColor->color());

    settings.setValue("distanceEcho1PointColor", ui->distanceEcho1PointColor->color());
    settings.setValue("distanceEcho2PointColor", ui->distanceEcho2PointColor->color());
    settings.setValue("distanceEcho3PointColor", ui->distanceEcho3PointColor->color());
    settings.setValue("distanceEcho4PointColor", ui->distanceEcho4PointColor->color());
    settings.setValue("distanceEcho5PointColor", ui->distanceEcho5PointColor->color());

    settings.setValue("distanceEcho1Show", ui->distanceEcho1Show->isChecked());
    settings.setValue("distanceEcho2Show", ui->distanceEcho2Show->isChecked());
    settings.setValue("distanceEcho3Show", ui->distanceEcho3Show->isChecked());
    settings.setValue("distanceEcho4Show", ui->distanceEcho4Show->isChecked());
    settings.setValue("distanceEcho5Show", ui->distanceEcho5Show->isChecked());

    settings.setValue("intensityEcho1LineColor", ui->intensityEcho1LineColor->color());
    settings.setValue("intensityEcho2LineColor", ui->intensityEcho2LineColor->color());
    settings.setValue("intensityEcho3LineColor", ui->intensityEcho3LineColor->color());
    settings.setValue("intensityEcho4LineColor", ui->intensityEcho4LineColor->color());
    settings.setValue("intensityEcho5LineColor", ui->intensityEcho5LineColor->color());

    settings.setValue("intensityEcho1PointColor", ui->intensityEcho1PointColor->color());
    settings.setValue("intensityEcho2PointColor", ui->intensityEcho2PointColor->color());
    settings.setValue("intensityEcho3PointColor", ui->intensityEcho3PointColor->color());
    settings.setValue("intensityEcho4PointColor", ui->intensityEcho4PointColor->color());
    settings.setValue("intensityEcho5PointColor", ui->intensityEcho5PointColor->color());

    settings.setValue("intensityEcho1Show", ui->intensityEcho1Show->isChecked());
    settings.setValue("intensityEcho2Show", ui->intensityEcho2Show->isChecked());
    settings.setValue("intensityEcho3Show", ui->intensityEcho3Show->isChecked());
    settings.setValue("intensityEcho4Show", ui->intensityEcho4Show->isChecked());
    settings.setValue("intensityEcho5Show", ui->intensityEcho5Show->isChecked());

    settings.setValue("distanceShow", ui->distanceGroupBox->isChecked());
    settings.setValue("intensityShow", ui->intensityGroupBox->isChecked());

    settings.setValue("distanceLineSize", ui->distanceLineSizeBox->value());
    settings.setValue("distancePointSize", ui->distancePointSizeBox->value());
    settings.setValue("intensityLineSize", ui->intensityLineSizeBox->value());
    settings.setValue("intensityPointSize", ui->intensityPointSizeBox->value());
    settings.setValue("intensityRatio", ui->intensityRatio->value());

    settings.setValue("lineModeRadio", ui->lineModeRadio->isChecked());
    settings.setValue("pointModeRadio", ui->pointModeRadio->isChecked());
    settings.setValue("polygonModeRadio", ui->polygonModeRadio->isChecked());

    settings.setValue("shadowRate", ui->shadowSpinBox->value());
    settings.setValue("refreshRate", ui->refreshSpinBox->value());

    settings.setValue("viewItensityOnTopBox", ui->viewItensityOnTopBox->isChecked());
    settings.setValue("invertedCheckBox", ui->invertedCheckBox->isChecked());

    settings.setValue("rotationSpinBox", ui->rotationSpinBox->value());

    settings.setValue("echoSelection", ui->echoSelection->currentIndex());

    settings.setValue("colorMode", ui->colormode->currentIndex());
    settings.setValue("colorScheme", ui->colorscheme->currentIndex());
}

void PlotSettingsWidget::restoreState(QSettings &settings)
{
    ui->distanceEcho1LineColor->setColor(settings.value("distanceEcho1LineColor", distanceEchoLineColorDefault).value<QColor>());
    ui->distanceEcho2LineColor->setColor(settings.value("distanceEcho2LineColor", distanceEchoLineColorDefault).value<QColor>());
    ui->distanceEcho3LineColor->setColor(settings.value("distanceEcho3LineColor", distanceEchoLineColorDefault).value<QColor>());
    ui->distanceEcho4LineColor->setColor(settings.value("distanceEcho4LineColor", distanceEchoLineColorDefault).value<QColor>());
    ui->distanceEcho5LineColor->setColor(settings.value("distanceEcho5LineColor", distanceEchoLineColorDefault).value<QColor>());

    ui->distanceEcho1PointColor->setColor(settings.value("distanceEcho1PointColor", distanceEchoPointColorDefault).value<QColor>());
    ui->distanceEcho2PointColor->setColor(settings.value("distanceEcho2PointColor", distanceEchoPointColorDefault).value<QColor>());
    ui->distanceEcho3PointColor->setColor(settings.value("distanceEcho3PointColor", distanceEchoPointColorDefault).value<QColor>());
    ui->distanceEcho4PointColor->setColor(settings.value("distanceEcho4PointColor", distanceEchoPointColorDefault).value<QColor>());
    ui->distanceEcho5PointColor->setColor(settings.value("distanceEcho5PointColor", distanceEchoPointColorDefault).value<QColor>());

    ui->distanceEcho1Show->setChecked(settings.value("distanceEcho1Show", true).toBool());
    ui->distanceEcho2Show->setChecked(settings.value("distanceEcho2Show", true).toBool());
    ui->distanceEcho3Show->setChecked(settings.value("distanceEcho3Show", true).toBool());
    ui->distanceEcho4Show->setChecked(settings.value("distanceEcho4Show", true).toBool());
    ui->distanceEcho5Show->setChecked(settings.value("distanceEcho5Show", true).toBool());

    ui->intensityEcho1LineColor->setColor(settings.value("intensityEcho1LineColor", intensityEchoLineColorDefault).value<QColor>());
    ui->intensityEcho2LineColor->setColor(settings.value("intensityEcho2LineColor", intensityEchoLineColorDefault).value<QColor>());
    ui->intensityEcho3LineColor->setColor(settings.value("intensityEcho3LineColor", intensityEchoLineColorDefault).value<QColor>());
    ui->intensityEcho4LineColor->setColor(settings.value("intensityEcho4LineColor", intensityEchoLineColorDefault).value<QColor>());
    ui->intensityEcho5LineColor->setColor(settings.value("intensityEcho5LineColor", intensityEchoLineColorDefault).value<QColor>());

    ui->intensityEcho1PointColor->setColor(settings.value("intensityEcho1PointColor", intensityEchoPointColorDefault).value<QColor>());
    ui->intensityEcho2PointColor->setColor(settings.value("intensityEcho2PointColor", intensityEchoPointColorDefault).value<QColor>());
    ui->intensityEcho3PointColor->setColor(settings.value("intensityEcho3PointColor", intensityEchoPointColorDefault).value<QColor>());
    ui->intensityEcho4PointColor->setColor(settings.value("intensityEcho4PointColor", intensityEchoPointColorDefault).value<QColor>());
    ui->intensityEcho5PointColor->setColor(settings.value("intensityEcho5PointColor", intensityEchoPointColorDefault).value<QColor>());

    ui->intensityEcho1Show->setChecked(settings.value("intensityEcho1Show", true).toBool());
    ui->intensityEcho2Show->setChecked(settings.value("intensityEcho2Show", true).toBool());
    ui->intensityEcho3Show->setChecked(settings.value("intensityEcho3Show", true).toBool());
    ui->intensityEcho4Show->setChecked(settings.value("intensityEcho4Show", true).toBool());
    ui->intensityEcho5Show->setChecked(settings.value("intensityEcho5Show", true).toBool());

    ui->distanceGroupBox->setChecked(settings.value("distanceShow", true).toBool());
    ui->intensityGroupBox->setChecked(settings.value("intensityShow", true).toBool());

    ui->distanceLineSizeBox->setValue(settings.value("distanceLineSize", 1).toInt());
    ui->distancePointSizeBox->setValue(settings.value("distancePointSize", 1).toInt());
    ui->intensityLineSizeBox->setValue(settings.value("intensityLineSize", 1).toInt());
    ui->intensityPointSizeBox->setValue(settings.value("intensityPointSize", 1).toInt());
    ui->intensityRatio->setValue(settings.value("intensityRatio", 100).toInt());

    ui->lineModeRadio->setChecked(settings.value("lineModeRadio", true).toBool());
    ui->pointModeRadio->setChecked(settings.value("pointModeRadio", false).toBool());
    ui->polygonModeRadio->setChecked(settings.value("polygonModeRadio", false).toBool());

    ui->shadowSpinBox->setValue(settings.value("shadowRate", 0).toInt());
    ui->refreshSpinBox->setValue(settings.value("refreshRate", 0).toInt());

    ui->viewItensityOnTopBox->setChecked(settings.value("viewItensityOnTopBox", false).toBool());
    ui->invertedCheckBox->setChecked(settings.value("invertedCheckBox", false).toBool());

    ui->rotationSpinBox->setValue(settings.value("rotationSpinBox", 0).toInt());

    ui->echoSelection->setCurrentIndex(settings.value("echoSelection", 0).toInt());

    ui->colormode->setCurrentIndex(settings.value("colorMode", 0).toInt());
    ui->colorscheme->setCurrentIndex(settings.value("colorScheme", 0).toInt());

    qInfo() << "colorMode on restore: "<< ui->colormode->currentIndex() << ", and on file: " << settings.value("colorMode", 0).toInt();
    updateTarget();
}

void PlotSettingsWidget::changeEvent(QEvent* e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        if (ui) {
            int index = ui->colormode->currentIndex();
            ui->colormode->blockSignals(true);
            ui->retranslateUi(this);
            ui->colormode->setCurrentIndex(index);
            ui->colormode->blockSignals(false);
        }
        break;
    default:
        break;
    }
}

void PlotSettingsWidget::setupConnections()
{
    //    connect(ui->startStep, SIGNAL(valueChanged(int)),
    //            this, SLOT(startStepChanged(int)));

    //    connect(ui->endStep, SIGNAL(valueChanged(int)),
    //            this, SLOT(eEndStepChanged(int)));
    //    connect(ui->minDistance, SIGNAL(valueChanged(int)),
    //            this, SLOT(minDistanceChanged(int)));
    //    connect(ui->maxDistance, SIGNAL(valueChanged(int)),
    //            this, SLOT(maxDistanceChanged(int)));

    connect(ui->pointModeRadio, &QAbstractButton::toggled,
            this, &PlotSettingsWidget::pointModeChanged);
    connect(ui->lineModeRadio, &QAbstractButton::toggled,
            this, &PlotSettingsWidget::lineModeChanged);
    connect(ui->polygonModeRadio, &QAbstractButton::toggled,
            this, &PlotSettingsWidget::polygonModeChanged);

    connect(ui->shadowSpinBox, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            this, &PlotSettingsWidget::shadowSpinBoxChanged);
    connect(ui->refreshSpinBox, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            this, &PlotSettingsWidget::refreshSpinBoxChanged);


    connect(ui->distanceGroupBox, &QGroupBox::toggled,
            this, &PlotSettingsWidget::viewDataBoxChanged);
    connect(ui->intensityGroupBox, &QGroupBox::toggled,
            this, &PlotSettingsWidget::viewIntensityBoxChanged);
    connect(ui->viewItensityOnTopBox, &QAbstractButton::toggled,
            this, &PlotSettingsWidget::viewItensityOnTopBoxChanged);

    connect(ui->rotationSpinBox, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            this, &PlotSettingsWidget::rotationSpinBoxChanged);
    connect(ui->rotationDial, &QAbstractSlider::sliderMoved,
            this, &PlotSettingsWidget::rotationDialChanged);
    connect(ui->invertedCheckBox, &QAbstractButton::toggled,
            this, &PlotSettingsWidget::invertedCheckBoxChanged);

    //--------------colors
    connect(distanceEchoLineColorGroup, static_cast<void (QButtonGroup::*)(int)>(&QButtonGroup::buttonClicked),
            this, &PlotSettingsWidget::distanceEchoLineColorChanged);
    connect(distanceEchoPointColorGroup, static_cast<void (QButtonGroup::*)(int)>(&QButtonGroup::buttonClicked),
            this, &PlotSettingsWidget::distanceEchoPointColorChanged);
    connect(distanceEchoShowGroup, static_cast<void (QButtonGroup::*)(int)>(&QButtonGroup::buttonClicked),
            this, &PlotSettingsWidget::distanceEchoShowChanged);

    connect(intensityEchoLineColorGroup, static_cast<void (QButtonGroup::*)(int)>(&QButtonGroup::buttonClicked),
            this, &PlotSettingsWidget::intensityEchoLineColorChanged);
    connect(intensityEchoPointColorGroup, static_cast<void (QButtonGroup::*)(int)>(&QButtonGroup::buttonClicked),
            this, &PlotSettingsWidget::intensityEchoPointColorChanged);
    connect(intensityEchoShowGroup, static_cast<void (QButtonGroup::*)(int)>(&QButtonGroup::buttonClicked),
            this, &PlotSettingsWidget::intensityEchoShowChanged);

    //------------------
    connect(ui->distanceLineSizeBox, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            this, &PlotSettingsWidget::distanceLineSizeChanged);
    connect(ui->distancePointSizeBox, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            this, &PlotSettingsWidget::distancePointSizeChanged);
    connect(ui->intensityLineSizeBox, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            this, &PlotSettingsWidget::intensityLineSizeChanged);
    connect(ui->intensityPointSizeBox, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            this, &PlotSettingsWidget::intensityPointSizeChanged);

    //------------------------
    connect(ui->intensityRatio, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            this, &PlotSettingsWidget::intensityRatioChanged);


    connect(ui->echoSelection, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this, &PlotSettingsWidget::echoSelectionChanged);

    //------------------------
    //color mode and scheme
    connect(ui->colormode, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this, &PlotSettingsWidget::colorModeChanged);
    connect(ui->colorscheme, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this, &PlotSettingsWidget::colorSchemeChanged);
}

void PlotSettingsWidget::startStepChanged(int step)
{
    Q_UNUSED(step);
}

void PlotSettingsWidget::eEndStepChanged(int step)
{
    Q_UNUSED(step);
}

void PlotSettingsWidget::minDistanceChanged(int dist)
{
    Q_UNUSED(dist);
}

void PlotSettingsWidget::maxDistanceChanged(int dist)
{
    Q_UNUSED(dist);
}

void PlotSettingsWidget::pointModeChanged(bool mode)
{
    if (m_target && mode) {
        m_target->setDrawingMode(Plot3DViewerPlugin::Points);
        m_target->refresh();
    }
}

void PlotSettingsWidget::lineModeChanged(bool mode)
{
    if (m_target && mode) {
        m_target->setDrawingMode(Plot3DViewerPlugin::Lines);
        m_target->refresh();
    }
}

void PlotSettingsWidget::polygonModeChanged(bool mode)
{
    if (m_target && mode) {
        m_target->setDrawingMode(Plot3DViewerPlugin::Polygon);
        m_target->refresh();
    }
}

void PlotSettingsWidget::distanceEchoLineColorChanged(int index)
{
    if (m_target) {
        ColorButton* button = (ColorButton*)distanceEchoLineColorGroup->button(index);
        m_target->setDistanceLineColor(index, button->color());
        m_target->refresh();
    }
}

void PlotSettingsWidget::distanceEchoPointColorChanged(int index)
{
    if (m_target) {
        ColorButton* button = (ColorButton*)distanceEchoPointColorGroup->button(index);
        m_target->setDistancePointColor(index, button->color());
        m_target->refresh();
    }
}

void PlotSettingsWidget::distanceEchoShowChanged(int index)
{
    if (m_target) {
        QCheckBox* button = (QCheckBox*)distanceEchoShowGroup->button(index);
        m_target->setDistanceShow(index, button->isChecked());
        m_target->refresh();
    }
}

void PlotSettingsWidget::distanceLineSizeChanged(int size)
{
    if (m_target) {
        m_target->setDistanceLineSize(size);
        m_target->refresh();
    }
}

void PlotSettingsWidget::distancePointSizeChanged(int size)
{
    if (m_target) {
        m_target->setDistancePointSize(size);
        m_target->refresh();
    }
}

void PlotSettingsWidget::intensityEchoLineColorChanged(int index)
{
    if (m_target) {
        ColorButton* button = (ColorButton*)intensityEchoLineColorGroup->button(index);
        m_target->setIntensityLineColor(index, button->color());
        m_target->refresh();
    }
}

void PlotSettingsWidget::intensityEchoPointColorChanged(int index)
{
    if (m_target) {
        ColorButton* button = (ColorButton*)intensityEchoPointColorGroup->button(index);
        m_target->setIntensityPointColor(index, button->color());
        m_target->refresh();
    }
}

void PlotSettingsWidget::intensityEchoShowChanged(int index)
{
    if (m_target) {
        QCheckBox* button = (QCheckBox*)intensityEchoShowGroup->button(index);
        m_target->setIntensityShow(index, button->isChecked());
        m_target->refresh();
    }
}

void PlotSettingsWidget::intensityLineSizeChanged(int size)
{
    if (m_target) {
        m_target->setIntensityLineSize(size);
        m_target->refresh();
    }
}

void PlotSettingsWidget::intensityPointSizeChanged(int size)
{
    if (m_target) {
        m_target->setIntensityPointSize(size);
        m_target->refresh();
    }
}

void PlotSettingsWidget::shadowSpinBoxChanged(int value)
{
    if (m_target) {
        m_target->setDrawPeriod(value);
    }
}

void PlotSettingsWidget::refreshSpinBoxChanged(int val)
{

    if (m_target) {
        m_target->setRefreshRate(val);
    }
}

void PlotSettingsWidget::viewDataBoxChanged(bool state)
{
    if (m_target) {
        m_target->showDistance(state);
        m_target->refresh();
    }

    //    ui->distanceGroupBox->layout()->setEnabled(state);
    //    ui->distanceGroupBox->setEnabled(state);
}

void PlotSettingsWidget::viewIntensityBoxChanged(bool state)
{
    if (m_target) {
        m_target->ShowIntensity(state);
        m_target->refresh();
    }

    //    ui->intensityGroupBox->layout()->setEnabled(state);
    //    ui->intensityGroupBox->setEnabled(state);
}

void PlotSettingsWidget::viewItensityOnTopBoxChanged(bool state)
{
    if (m_target) {
        m_target->intensityOnTop(!state);
        m_target->refresh();
    }
}

void PlotSettingsWidget::rotationSpinBoxChanged(int rotation)
{
    if (m_target) {
        m_target->setRotationOffset(rotation);
        int rot = rotation + 180;
        if (rot > 359) {
            rot = rot - 360;
        }
        ui->rotationDial->setValue(rot);
        m_target->refresh();
    }
}

void PlotSettingsWidget::rotationDialChanged(int rotation)
{
    int value = 0;
    if (rotation < 180) {
        value = rotation + 180;
    }
    else {
        value = rotation - 180;
    }
    ui->rotationSpinBox->setValue(value);
}

void PlotSettingsWidget::invertedCheckBoxChanged(bool state)
{
    if (m_target) {
        m_target->setInvertedView(state);
    }
}

void PlotSettingsWidget::intensityRatioChanged(int val)
{
    if (m_target) {
        m_target->setIntensityRatio(val);
    }
}

void PlotSettingsWidget::echoSelectionChanged(int index)
{
    if (m_target) {
        m_target->setEchoSelection(index);
    }
}

void PlotSettingsWidget::colorModeChanged(int index)
{
    qInfo() << "colorMode's colorModeChanged -> " << index;
    //for plain color, disable the color scheme combobox
    if (index == 0) {
        ui->colorscheme->setEnabled(false);
        if (m_target) {
            //m_target->setColorScheme(false, -1); //no color scheme
            m_target->setColoringMode(-1);
            m_target->setColorScheme(false, ui->colorscheme->currentIndex());
        }
    } else { //disable the plain color selection
        ui->colorscheme->setEnabled(true);
        if (m_target) {
            m_target->setColoringMode(index-1);
            m_target->setColorScheme(true, ui->colorscheme->currentIndex());
        }
    }
}

void PlotSettingsWidget::colorSchemeChanged(int index)
{
    if (m_target) {
        m_target->setColorScheme(ui->colormode->currentIndex() > 0, index);
    }
}

