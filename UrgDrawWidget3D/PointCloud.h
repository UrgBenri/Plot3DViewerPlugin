#ifndef POINTCLOUD_H
#define POINTCLOUD_H

#include <QColor>
#include <QVector>
#include <QVector4D>

#include "RenderableItem.h"

class PointCloud: public RenderableItem
{
    Q_OBJECT
public:
    explicit PointCloud(QObject *parent = Q_NULLPTR);
    virtual ~PointCloud();

    virtual void render() Q_DECL_OVERRIDE;
    virtual void init() Q_DECL_OVERRIDE;

    QVector<QVector4D> points() const;
    void setPoints(const QVector<QVector4D> &points);

    float pointSize() const;
    void setPointSize(float pointSize);

    float rangeFactor() const;
    void setRangeFactor(float rangeFactor);

    QColor color() const;
    void setColor(const QColor &color);

private:
    unsigned int m_listIndex;
    QVector<QVector4D> m_points;
    QColor m_color;
    float m_pointSize;
    float m_rangeFactor;
};

#endif // POINTCLOUD_H
