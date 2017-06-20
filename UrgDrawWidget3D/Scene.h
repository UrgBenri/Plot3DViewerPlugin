#ifndef SCENE_H
#define SCENE_H

#include <QVector>
#include <QSize>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QColor>

#include "RenderableItem.h"
#include "Viewport.h"

class Scene: public RenderableItem
{
public:
    explicit Scene();
    virtual ~Scene();

    virtual void render() Q_DECL_OVERRIDE;
    virtual void init() Q_DECL_OVERRIDE;

    void mouseMoveEvent(QMouseEvent* event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);

    void zoomIn();
    void zoomOut();


    QSize size() const;
    void setSize(const QSize &size);

    void addViewport(Viewport *view);
    void addItem(RenderableItem *item);

    QColor backgroundColor() const;
    void setBackgroundColor(const QColor &backgroundColor);

private:
    QSize m_size;
    QVector<Viewport *> m_viewports;
    QVector<RenderableItem *> m_items;
    QColor m_backgroundColor;

    Viewport *viewportForPos(const QPoint &pos) const;
    Viewport *selectedViewport();
};

#endif // SCENE_H
