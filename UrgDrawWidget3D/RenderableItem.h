#ifndef RENDERABLEITEM_H
#define RENDERABLEITEM_H

#include <QString>
#include <QColor>
#include <QVector3D>

class RenderableItem
{
public:
    explicit RenderableItem();
    virtual ~RenderableItem();

    virtual void render() = 0;
    virtual void init() ;

    bool isVisible() const;
    void setVisible(bool visible);

    QVector3D pos() const;
    void setPos(const QVector3D &pos);

protected:
    bool m_visible;
    QVector3D m_pos;
    bool m_initialized;
};

#endif // RENDERABLEITEM_H
