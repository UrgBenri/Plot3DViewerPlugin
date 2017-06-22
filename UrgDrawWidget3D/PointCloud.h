#ifndef POINTCLOUD_H
#define POINTCLOUD_H

#include <QColor>
#include <QVector>
#include <QVector3D>

#include "RenderableItem.h"

class PointCloud: public RenderableItem
{
    Q_OBJECT
public:
    explicit PointCloud(QObject *parent = Q_NULLPTR);
    virtual ~PointCloud();

    virtual void render() Q_DECL_OVERRIDE;
    virtual void init() Q_DECL_OVERRIDE;

    QVector<QVector3D> points() const;
    void setPoints(const QVector<QVector3D> &points);

    float pointSize() const;
    void setPointSize(float pointSize);

    float rangeFactor() const;
    void setRangeFactor(float rangeFactor);

private:
    unsigned int m_listIndex;
    QVector<QVector3D> m_points;
    float m_pointSize;
    float m_rangeFactor;
};

#endif // POINTCLOUD_H
