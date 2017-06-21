#ifndef VIEWPORT_H
#define VIEWPORT_H

#include <QRectF>
#include <QColor>
#include <QString>

#include "RenderableItem.h"
#include "Camera.h"

class Viewport: public RenderableItem
{
    Q_OBJECT
public:
    explicit Viewport(QObject *parent = Q_NULLPTR, const QRectF &rect = QRectF(0, 0, 1, 1));
    virtual ~Viewport();

    virtual void render() override;

    void mouseMoveEvent(QMouseEvent* event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);

    QSize sceneSize() const;
    void setSceneSize(const QSize &sceneSize);

    void setRect(const QRectF &rect);
    inline QRectF rect() const {return m_rect;}

    QRect viewportRect() const;
    QRect viewRect() const;

    inline Camera *camera() {return &m_camera; }

    void renderBorder();

    QString name() const;
    void setName(const QString &name);

    QColor nameColor() const;
    void setNameColor(const QColor &nameColor);

    bool isSelected() const;
    void setSelected(bool selected);

    QColor borderColor() const;
    void setBorderColor(const QColor &borderColor);

    QColor selectedBorderColor() const;
    void setSelectedBorderColor(const QColor &selectedBorderColor);

private:
    QRectF m_rect;
    QSize m_sceneSize;
    Camera m_camera;
    QColor m_backgroundColor;
    int m_borderWidth;
    QColor m_borderColor;
    QColor m_selectedBorderColor;
    QString m_name;
    QColor m_nameColor;
    bool m_selected;

    void initMembers();
};

#endif // VIEWPORT_H
