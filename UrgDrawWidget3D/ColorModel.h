#ifndef COLORMODEL_H
#define COLORMODEL_H

#include <QColor>
#include <QVector3D>
#include <QVector4D>

#include "ColorScheme.h"

class ColorModel
{
public:
    explicit ColorModel();

    typedef enum {
        COLOR_PLAIN=-1,
        COLOR_BY_RANGE=0,
        COLOR_BY_LEVEL,
        COLOR_BY_STEP,
        COLOR_BY_Z,
        COLOR_BY_X,
        COLOR_BY_Y,
        COLOR_BY_ECHO, //not used
    } coloring_modes_t;

    typedef struct data_extrema_t {
        data_extrema_t() {
            range_min = (float)std::numeric_limits<long>::max();
            range_max = (float)std::numeric_limits<long>::min();
            level_min = (float)std::numeric_limits<long>::max();
            level_max = (float)std::numeric_limits<long>::min();
            coord_min = QVector3D((float)std::numeric_limits<long>::max(), (float)std::numeric_limits<long>::max(), (float)std::numeric_limits<long>::max());
            coord_max = QVector3D((float)std::numeric_limits<long>::min(), (float)std::numeric_limits<long>::min(), (float)std::numeric_limits<long>::min());
        }

        float range_min;
        float range_max;
        float level_min;
        float level_max;
        QVector3D coord_min;
        QVector3D coord_max;
    } data_extrema_t;

    virtual QColor color(int step, float x, float y, float z, float intensity) const;

    void setColorScheme(int scheme);
    void setColoringMode(int mode);

    void setMinMaxCoordinates(const QVector<QVector4D> &ranges);

    QColor plainColor() const;
    void setPlainColor(const QColor &plainColor);

private:
    ColorScheme m_colorScheme;
    coloring_modes_t m_colorMode;
    data_extrema_t m_extrema;
    QColor m_plainColor;
};

#endif // COLORMODEL_H
