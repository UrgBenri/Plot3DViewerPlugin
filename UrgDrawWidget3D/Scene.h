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
    Q_OBJECT
public:
    explicit Scene(QObject *parent = Q_NULLPTR);
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

    void addViewport(Viewport *view, const QString &name);
    void addItem(RenderableItem *item, const QString &name);

    Viewport *viewport(const QString &name) const;
    RenderableItem * item(const QString &name) const;

    QColor backgroundColor() const;
    void setBackgroundColor(const QColor &backgroundColor);

private:
    QSize m_size;
    QVector<Viewport *> m_viewports;
    QHash<QString, int> m_nameViewportMap;
    QVector<RenderableItem *> m_items;
    QHash<QString, int> m_nameItemMap;
    QColor m_backgroundColor;

    Viewport *viewportForPos(const QPoint &pos) const;
    Viewport *selectedViewport();
};

#endif // SCENE_H
