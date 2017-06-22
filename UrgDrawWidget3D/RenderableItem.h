#ifndef RENDERABLEITEM_H
#define RENDERABLEITEM_H

#include <QString>
#include <QColor>
#include <QVector3D>

#include <QObject>

#include "ColorModel.h"

class RenderableItem: public QObject
{
    Q_OBJECT
public:
    explicit RenderableItem(QObject *parent = Q_NULLPTR);
    virtual ~RenderableItem();

    virtual void render() = 0;
    virtual void init() ;

    bool isVisible() const;
    void setVisible(bool visible);

    QVector3D pos() const;
    void setPos(const QVector3D &pos);

    ColorModel *colorModel() const;
    void setColorModel(ColorModel *colorModel);

protected:
    bool m_visible;
    QVector3D m_pos;
    bool m_initialized;
    ColorModel *m_colorModel;
};

#endif // RENDERABLEITEM_H
