#include "RenderableItem.h"

RenderableItem::RenderableItem(QObject *parent)
    : QObject(parent)
    , m_visible(true)
    , m_pos(0, 0, 0)
    , m_initialized(false)
{

}

RenderableItem::~RenderableItem()
{

}

void RenderableItem::init()
{

}

bool RenderableItem::isVisible() const
{
    return m_visible;
}

void RenderableItem::setVisible(bool visible)
{
    m_visible = visible;
}

QVector3D RenderableItem::pos() const
{
    return m_pos;
}

void RenderableItem::setPos(const QVector3D &pos)
{
    m_pos = pos;
    m_initialized = false;
}
