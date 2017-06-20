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

#ifndef PLOTWIDGET_H
#define PLOTWIDGET_H

#include <QWidget>
#include "UrgDrawWidget3D.h"

#include <time.h>
#include <QTime>
#include <QTimer>
#include <QPointF>
#include <QVector>
#include <QTranslator>

#include "MovingAverage.h"
#include "ViewerPluginInterface.h"
#include "PlotSettingsWidget.h"

namespace Ui {
class Plot3DViewerPlugin;
}

class Plot3DViewerPlugin : public ViewerPluginInterface
{
    Q_OBJECT
    Q_INTERFACES(ViewerPluginInterface)
    Q_PLUGIN_METADATA(IID "org.kristou.UrgBenri.Plot3DViewerPlugin")
    PLUGIN_FACTORY(Plot3DViewerPlugin)
public:
    explicit Plot3DViewerPlugin(QWidget *parent = 0);
    virtual ~Plot3DViewerPlugin();

    typedef enum {
        Points,
        Lines,
        Polygon,
    } DrawMode;

    void setDistanceLineColor(int echo, QColor color);
    void setDistancePointColor(int echo, QColor color);
    void setDistanceShow(int echo, bool state);
    void setDistanceLineSize(int size);
    void setDistancePointSize(int size);

    void setIntensityLineColor(int echo, QColor color);
    void setIntensityPointColor(int echo, QColor color);
    void setIntensityShow(int echo, bool state);
    void setIntensityLineSize(int size);
    void setIntensityPointSize(int size);

    void setEchoSelection(int index);

    void setDrawingMode(DrawMode mode);
    void setDrawPeriod(size_t msec);

    void recenter();
    void resetView();

    QString pluginName() const { return tr("Plot3D"); }
    QIcon pluginIcon() const{ return QIcon(":/Plot3DViewerPlugin/tabIcon"); }
    QString pluginDescription() const {return tr("Plot measurement data on a 3D canvas");}
    PluginVersion pluginVersion() const {return PluginVersion(1, 1, 0);}
    QString pluginAuthorName() const {return "Alexander Carballo";}
    QString pluginAuthorContact() const {return "alexandr@hokuyo-aut.co.jp";}
    int pluginLoadOrder() const {return 1;}
    bool pluginIsExperimental() const { return false; }

    QString pluginLicense() const { return "GPL"; }
    QString pluginLicenseDescription() const { return "GPL"; }

    void saveState(QSettings &settings);
    void restoreState(QSettings &settings);

    void loadTranslator(const QString &locale);

    void initialize();

    void showDistance(bool state);
    void ShowIntensity(bool state);
    void intensityOnTop(bool state);

    void setRotationOffset(int rotation);

    void setInvertedView(bool state) {invertedView = state;}
//    void setRangeSensor(const RangeSensor* sensor);

    void setIntensityRatio(int val);

    void reszeValuesTable();

    void setRefreshRate(int refreshRate_) {  refreshRate = refreshRate_;}

    void setColorScheme(bool use_scheme, int scheme);
    void setColoringMode(int mode);

protected:
    void changeEvent(QEvent* e);
    void hideEvent(QHideEvent *);

signals:
    void mousePositionChanged(long x_mm, long y_mm, int step);
    void mousePositionChanged(long Distance, int step);
    void mousePositionClicked(long x_mm, long y_mm, int step);

    void error(const QString &header, const QString &message);
    void warning(const QString &header, const QString &message);
    void information(const QString &header, const QString &message);

public slots:
    void addMeasurementData(const QString &id
                            , const PluginDataStructure &data);
    void clear();
    void refresh();
    void setSelectedStep(int step);

    void onLoad(PluginManagerInterface *manager);
    void onUnload();

private slots:
    void orthoMouseMove(bool state, long x, long y, int step);
    void orthoMouseClicked(bool state, long x, long y, int step);

    void orthoSnapshot();

    void toggleGrid();

    void updateValuesTable(int step);

private:
    Ui::Plot3DViewerPlugin *ui;
    void setupConnections();
    void setupShortcuts();
    bool invertedView;

    PluginDataStructure localData;

    QVector<QVector<long> > invertData(const QVector<QVector<long> > &steps, bool toInvert);
    int startStep;
    int endStep;

    int selectedStep;
    long currentScan;

    int refreshRate;
    QTime refreshTimer;
    MovingAverage<int> refreshTimerAvg;

    PlotSettingsWidget *m_settings;
    QTranslator m_translator;

    void initSettings();

};
#endif // PLOTWIDGET_H

