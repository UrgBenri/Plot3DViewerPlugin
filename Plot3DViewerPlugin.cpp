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

#include "Plot3DViewerPlugin.h"
#include "ui_Plot3DViewerPlugin.h"

#include <algorithm>
#include <QFileDialog>
#include <QFileInfo>
#include <QDir>
#include <QDebug>
#include <QShortcut>
#include <QDesktopWidget>

#define ECHO_COUNT 3

Plot3DViewerPlugin::Plot3DViewerPlugin(QWidget *parent)
    : ViewerPluginInterface(parent)
    , ui(new Ui::Plot3DViewerPlugin)
    , currentScan(0)
    , refreshRate(100)
    , refreshTimerAvg(40, 25)
    , m_settings(Q_NULLPTR)
{
    initialize();
}

void Plot3DViewerPlugin::initSettings()
{
    for(int i = 0; i < 5; ++i){
        setDistanceLineColor(i, QColor(Qt::green));
        setDistancePointColor(i,  QColor(Qt::red));
        setDistanceShow(i, true);
        setIntensityLineColor(i, QColor(Qt::magenta));
        setIntensityPointColor(i, QColor(Qt::magenta));
        setIntensityShow(i, true);
    }

    showDistance(true);
    ShowIntensity(true);
    intensityOnTop(true);
    setRotationOffset(0);

    setDrawingMode(Plot3DViewerPlugin::Lines);

    setDistanceLineSize(1);
    setDistancePointSize(1);

    setIntensityLineSize(1);
    setIntensityPointSize(1);
    setIntensityRatio(100);

    setRefreshRate(0);
    setDrawPeriod(25);
}

void Plot3DViewerPlugin::reszeValuesTable()
{
    ui->stepInfo->setText("");
}

Plot3DViewerPlugin::~Plot3DViewerPlugin()
{
    //    qDebug() << "PlotWidget::~PlotWidget";
    if(m_settings) delete m_settings;
    delete ui;
}

void Plot3DViewerPlugin::setIntensityRatio(int val)
{
    double ratio = (double)val / 100.0;
//    ui->_3DDrawWidget->setIntensityRatio(ratio);
}

void Plot3DViewerPlugin::changeEvent(QEvent* e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
        case QEvent::LanguageChange:
            if(ui) ui->retranslateUi(this);
            break;
        default:
            break;
    }
}

void Plot3DViewerPlugin::hideEvent(QHideEvent *)
{
    m_settings->close();
}

void Plot3DViewerPlugin::addMeasurementData(const QString &id, const PluginDataStructure &data)
{
    //TODO: investigate this, memory leak!!!!
    localData = data;
    localData.ranges = invertData(localData.ranges, invertedView);
    localData.levels = invertData(localData.levels, invertedView);


    if (refreshTimer.elapsed() > refreshRate) {
        refreshTimerAvg.push_back(refreshTimer.restart());
        refresh();
    }
}

void Plot3DViewerPlugin::setRotationOffset(int rotation)
{
//    ui->_3DDrawWidget->setRotationOffset(rotation);
}

//void PlotWidget::setRangeSensor(const RangeSensor* sensor)
//{
//    localSensor = sensor;
//    ui->_3DDrawWidget->setConvertor(localSensor);
//}

void Plot3DViewerPlugin::orthoMouseMove(bool state, long x, long y, int step)
{
    if (state) {
        emit mousePositionChanged(x, y, step);
    }
}

void Plot3DViewerPlugin::orthoMouseClicked(bool state, long x, long y, int step)
{
    if (state) {
        emit mousePositionClicked(x, y, step);
        setSelectedStep(step);
        emit selectedStepChanged(step);
    }
}

void Plot3DViewerPlugin::refresh()
{
    ui->_3DDrawWidget->addMeasurementData(localData);
//    ui->_3DDrawWidget->setUpdateRate(tr("Refresh rate: ") + QString::number(1000 / refreshTimerAvg.average()) + " fps");
    updateValuesTable(selectedStep);
}

void Plot3DViewerPlugin::orthoSnapshot()
{
//    QPixmap pixmap = ui->_3DDrawWidget->renderPixmap(0, 0, true);
//    QString defaulName = QString("/") +
//            QDateTime::currentDateTime().toString("yyyy_MM_dd_HH_mm_ss_zzz") +
//            ".png";
//    QString filename = QFileDialog::getSaveFileName(
//                this,
//                tr("Save snapshot"),
//                QDir::currentPath() + defaulName,
//                tr("PNG file (*.png)"));
//    if (!filename.isNull()) {
//        QFileInfo fi(filename);
//        QDir::setCurrent(fi.absolutePath());

//        pixmap.save(filename);
//    }
}

void Plot3DViewerPlugin::setSelectedStep(int step){
    selectedStep = step;
//    ui->_3DDrawWidget->setSelectedStep(step);
    updateValuesTable(selectedStep);
}

void Plot3DViewerPlugin::onLoad(PluginManagerInterface *manager)
{

}

void Plot3DViewerPlugin::onUnload()
{

}

void Plot3DViewerPlugin::toggleGrid()
{
//    ui->_3DDrawWidget->showAxis(!ui->_3DDrawWidget->isAxisShowed());
}

void Plot3DViewerPlugin::updateValuesTable(int step)
{
    if(localData.converter){
        int fdata = localData.converter.data()->step2Index(step);
        if (fdata >= 0 && fdata < localData.ranges.size()) {
            QString stepInfo = tr("Step: <b>%1</b>")
                    .arg(QString::number(step), 4, '0');
            for(int i = 0; i < ECHO_COUNT; ++i){
                if(localData.ranges[fdata].size() > i){
                    stepInfo += ", " + tr("Echo%1: <b>%2</b>")
                            .arg(i)
                            .arg(QString::number(localData.ranges[fdata][i]), 5, '0');
                }
            }

            ui->stepInfo->setText(stepInfo);
            return;
        }
    }

    ui->stepInfo->setText("");
}

void Plot3DViewerPlugin::setupConnections()
{
    connect(ui->_3DDrawWidget, &UrgDrawWidget3D::positionUpdated,
            this, &Plot3DViewerPlugin::orthoMouseMove);
    connect(ui->_3DDrawWidget, &UrgDrawWidget3D::positionClicked,
            this, &Plot3DViewerPlugin::orthoMouseClicked);

    connect(ui->orthoZoominButton, &QAbstractButton::clicked,
            ui->_3DDrawWidget, &UrgDrawWidget3D::largerZoom);
    connect(ui->orthoZzoomoutButton, &QAbstractButton::clicked,
            ui->_3DDrawWidget, &UrgDrawWidget3D::smallerZoom);
//    connect(ui->orthoZoomfitButton, &QAbstractButton::clicked,
//            ui->_3DDrawWidget, &UrgDrawWidget3D::initializeView);

    connect(ui->settingsButton, &QToolButton::clicked,
            this, [=](){

        m_settings->show();
        m_settings->raise();

        int screenBottom = QApplication::desktop()->screenGeometry(QApplication::desktop()->screenNumber(m_settings)).bottom();

        QPoint position = ui->settingsButton->mapToGlobal(QPoint(0, ui->settingsButton->height()));
        m_settings->move(position);

        QRect settingsRect = m_settings->geometry();
        settingsRect.setHeight(680);
        if(settingsRect.bottom() > screenBottom ){
            int diff = settingsRect.bottom() - screenBottom;
            settingsRect.setHeight(settingsRect.height() - diff);
        }
        m_settings->setGeometry(settingsRect);

    });
//    connect(ui->orthoClear, &QAbstractButton::clicked,
//            ui->_3DDrawWidget, &UrgDrawWidget3D::clear);

    connect(ui->orthoRefreshAction, &QAbstractButton::clicked,
            this, &Plot3DViewerPlugin::refresh);

    connect(ui->orthoSnapshot, &QAbstractButton::clicked,
            this, &Plot3DViewerPlugin::orthoSnapshot);
}

void Plot3DViewerPlugin::setupShortcuts()
{
    //    (void) new QShortcut(QKeySequence(Qt::ALT + Qt::Key_O), this, SLOT(showOrthoView()));
    //    (void) new QShortcut(QKeySequence(Qt::ALT + Qt::Key_T), this, SLOT(showTableView()));
    //    (void) new QShortcut(QKeySequence(Qt::ALT + Qt::Key_G), this, SLOT(toggleGrid()));
}

QVector<QVector<long > > Plot3DViewerPlugin::invertData(const QVector<QVector<long > > &steps, bool toInvert)
{
    QVector<QVector<long > > result = steps;
    if (toInvert && !result.isEmpty()) {
        std::reverse(result.begin(), result.end());
    }
    return result;
}

void Plot3DViewerPlugin::clear()
{
    localData.ranges.clear();
    localData.levels.clear();
//    ui->_3DDrawWidget->clear();
}

void Plot3DViewerPlugin::setDistanceLineColor(int echo, QColor color)
{
//    ui->_3DDrawWidget->setRangeLineColor(echo, color);
}

void Plot3DViewerPlugin::setDistancePointColor(int echo, QColor color)
{
//    ui->_3DDrawWidget->setRangePointColor(echo, color);
}

void Plot3DViewerPlugin::setDistanceShow(int echo, bool state)
{
//    ui->_3DDrawWidget->setRangeShow(echo, state);
}

void Plot3DViewerPlugin::setDistanceLineSize(int size)
{
//    ui->_3DDrawWidget->setRangeLineSize(size);
}

void Plot3DViewerPlugin::setDistancePointSize(int size)
{
//    ui->_3DDrawWidget->setRangePointSize(size);
}

void Plot3DViewerPlugin::setIntensityLineColor(int echo, QColor color)
{
//    ui->_3DDrawWidget->setLevelLineColor(echo, color);
}

void Plot3DViewerPlugin::setIntensityPointColor(int echo, QColor color)
{
//    ui->_3DDrawWidget->setLevelPointColor(echo, color);
}

void Plot3DViewerPlugin::setIntensityShow(int echo, bool state)
{
//    ui->_3DDrawWidget->setIntensityShow(echo, state);
}

void Plot3DViewerPlugin::setIntensityLineSize(int size)
{
//    ui->_3DDrawWidget->setLevelLineSize(size);
}

void Plot3DViewerPlugin::setIntensityPointSize(int size)
{
//    ui->_3DDrawWidget->setLevelPointSize(size);
}

void Plot3DViewerPlugin::setEchoSelection(int index)
{
//    ui->_3DDrawWidget->setEchoSelection(index);
}

void Plot3DViewerPlugin::setDrawingMode(DrawMode mode)
{
//    switch (mode) {
//        case Points: {
//            ui->_3DDrawWidget->setDrawMode(UrgDrawWidget3D::Points);
//        }
//            break;
//        case Lines: {
//            ui->_3DDrawWidget->setDrawMode(UrgDrawWidget3D::Lines);
//        }
//            break;
//        case Polygon: {
//            ui->_3DDrawWidget->setDrawMode(UrgDrawWidget3D::Polygon);
//        }
//            break;
//        default: {
//            ui->_3DDrawWidget->setDrawMode(UrgDrawWidget3D::Lines);
//        }
//    }
}

void Plot3DViewerPlugin::setDrawPeriod(size_t msec)
{
//    ui->_3DDrawWidget->setDrawPeriod(msec);
}
void Plot3DViewerPlugin::recenter()
{
//    ui->_3DDrawWidget->setViewCenter(0.0, 0.0);
    refresh();
}

void Plot3DViewerPlugin::resetView()
{
//    ui->_3DDrawWidget->initializeView();
    currentScan = 0;
}

void Plot3DViewerPlugin::saveState(QSettings &settings)
{
    m_settings->saveState(settings);
}

void Plot3DViewerPlugin::restoreState(QSettings &settings)
{
    m_settings->restoreState(settings);
}

void Plot3DViewerPlugin::loadTranslator(const QString &locale)
{
    qApp->removeTranslator(&m_translator);
    if (m_translator.load(QString("plugin.%1").arg(locale), ":/Plot3DViewerPlugin")) {
        qApp->installTranslator(&m_translator);
    }
}

void Plot3DViewerPlugin::initialize()
{
    ui->setupUi(this);

    m_settings = new PlotSettingsWidget();
    m_settings->setWindowFlags(Qt::Popup);
//    ui->_3DDrawWidget->initializeView();

    setDistanceLineColor(0, Qt::green);
    setDistancePointColor(0, Qt::red);
    setIntensityLineColor(0, Qt::magenta);
    setIntensityPointColor(0, Qt::red);

    setDrawingMode(Lines);

    invertedView = false;

    setupConnections();
    setupShortcuts();

    selectedStep = -1;

    refreshTimerAvg.setAverageValue(refreshRate);
    refreshTimer.start();

    reszeValuesTable();

    ui->orthoRefreshAction->setVisible(false);

    connect(ui->_3DDrawWidget, &UrgDrawWidget3D::error,
            this, &Plot3DViewerPlugin::error);
    connect(ui->_3DDrawWidget, &UrgDrawWidget3D::information,
            this, &Plot3DViewerPlugin::information);
    connect(ui->_3DDrawWidget, &UrgDrawWidget3D::warning,
            this, &Plot3DViewerPlugin::warning);


    initSettings();

//    connect(ui->snapLastButton, &QAbstractButton::clicked,
//            ui->_3DDrawWidget, &UrgDrawWidget3D::snapLast);
//    connect(ui->snapClearButton, &QAbstractButton::clicked,
//            ui->_3DDrawWidget, &UrgDrawWidget3D::snapClear);
    m_settings->setTarget(this);
}

void Plot3DViewerPlugin::showDistance(bool state)
{
//    ui->_3DDrawWidget->setShowRange(state);
}

void Plot3DViewerPlugin::ShowIntensity(bool state)
{
//    ui->_3DDrawWidget->setShowLevel(state);
}

void Plot3DViewerPlugin::intensityOnTop(bool state)
{
//    ui->_3DDrawWidget->setLevelFirst(state);
}

void Plot3DViewerPlugin::setColorScheme(bool use_scheme, int scheme)
{
    if (use_scheme == false) {
        scheme = (int)ColorScheme::PLAIN_SCHEME;
    }
//    ui->_3DDrawWidget->setColorScheme(use_scheme, scheme);
}

void Plot3DViewerPlugin::setColoringMode(int mode)
{
//    ui->_3DDrawWidget->setColoringMode(mode);
}

