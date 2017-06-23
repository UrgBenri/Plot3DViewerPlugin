#include "ColorModel.h"

#include <cmath>

ColorModel::ColorModel()
{
    setColorScheme(ColorScheme::PLAIN_SCHEME);
    setColoringMode(COLOR_PLAIN);
}

QColor ColorModel::color(int step, float x, float y, float z, float intensity) const
{
    QColor color;
    switch (m_colorMode) {
        case COLOR_BY_RANGE:{
            float range = std::sqrt(x*x + y*y + z*z);
            color = m_colorScheme.getColor(range);
        } break;
        case COLOR_BY_LEVEL:
            color = m_colorScheme.getColor(intensity);
        break;
        case COLOR_BY_STEP:
            color = m_colorScheme.getColor(step);
        break;
        case COLOR_BY_Z:
            color = m_colorScheme.getColor(z);
        break;
        case COLOR_BY_X:
            color = m_colorScheme.getColor(x);
            //qInfo() << "[" << step << "," << echo << "]:" << "Value: " << z << ", Color: " << color;
        break;
        case COLOR_BY_Y:
            color = m_colorScheme.getColor(y);
        break;
        case COLOR_PLAIN:
            color = m_plainColor;
        break;
    }
    return color;
}

void ColorModel::setColorScheme(int scheme)
{
    if (!scheme < ColorScheme::PLAIN_SCHEME || scheme > ColorScheme::GRAY_SCHEME) {
        scheme = (int)ColorScheme::PLAIN_SCHEME;
    }
    m_colorScheme.setScheme((ColorScheme::color_scheme_t)scheme);

}

void ColorModel::setColoringMode(int mode)
{
    if (mode < ColorModel::COLOR_PLAIN || mode > ColorModel::COLOR_BY_ECHO) {
        mode = (int)ColorModel::COLOR_PLAIN;
    }
    m_colorMode = (ColorModel::coloring_modes_t)mode;
}

void ColorModel::setMinMaxCoordinates(const QVector<QVector4D> &ranges)
{
    data_extrema_t extrema;
    for (int i = 0; i < ranges.size(); i++) {
        QVector4D point = ranges[i];
        extrema.coord_min.setX( qMin(point.x(), extrema.coord_min.x()) );
        extrema.coord_min.setY( qMin(point.y(), extrema.coord_min.y()) );
        extrema.coord_min.setZ( qMin(point.z(), extrema.coord_min.z()) );
        extrema.coord_max.setX( qMax(point.x(), extrema.coord_max.x()) );
        extrema.coord_max.setY( qMax(point.y(), extrema.coord_max.y()) );
        extrema.coord_max.setZ( qMax(point.z(), extrema.coord_max.z()) );

        float range = std::sqrt(point.x()*point.x() + point.y()*point.y() + point.z()*point.z());
        extrema.range_min = qMin(extrema.range_min, range);
        extrema.range_max = qMax(extrema.range_max, range);

        extrema.level_min = qMin(extrema.level_min, point.w());
        extrema.level_max = qMax(extrema.level_max, point.w());
    }
    m_extrema = extrema;

    switch (m_colorMode) {
        case COLOR_BY_RANGE:
            m_colorScheme.setLimits(m_extrema.range_min, m_extrema.range_max);
        break;
        case COLOR_BY_LEVEL:
            m_colorScheme.setLimits(m_extrema.level_min, m_extrema.level_max);
        break;
        case COLOR_BY_STEP:
            m_colorScheme.setLimits(0.0,(float)ranges.size());
        break;
        case COLOR_BY_Z:
            m_colorScheme.setLimits(m_extrema.coord_min.z(), m_extrema.coord_max.z());
        break;
        case COLOR_BY_X:
            m_colorScheme.setLimits(m_extrema.coord_min.x(), m_extrema.coord_max.x());
            //qInfo() << "[" << step << "," << echo << "]:" << "Value: " << z << ", Color: " << color;
        break;
        case COLOR_BY_Y:
            m_colorScheme.setLimits(m_extrema.coord_min.y(), m_extrema.coord_max.y());
        break;
    }
}

QColor ColorModel::plainColor() const
{
    return m_plainColor;
}

void ColorModel::setPlainColor(const QColor &plainColor)
{
    m_plainColor = plainColor;
}
