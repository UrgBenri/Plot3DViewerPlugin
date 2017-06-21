#include "Scene.h"

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <algorithm>

Scene::Scene(QObject *parent)
    : RenderableItem (parent)
    , m_backgroundColor(108, 108, 132)
{

}

Scene::~Scene()
{
    qDeleteAll(m_viewports);
    qDeleteAll(m_items);
}

void Scene::render()
{
    GLint width = m_size.width();
    GLint height = m_size.height();

    // Set static configs:
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_ALPHA_TEST);
    glEnable(GL_TEXTURE_2D);

    glViewport(0, 0, width, height);
    glLoadIdentity();

    QSize sceneSize = m_size;
    QVector<RenderableItem *> items = m_items;
    std::for_each(m_viewports.begin(), m_viewports.end(), [sceneSize, items](Viewport *item){
        item->setSceneSize(sceneSize);
        item->render();
        std::for_each(items.begin(), items.end(), [](RenderableItem *item){
            item->render();
        });
        item->renderBorder();
    });
    // Assure we restore the original viewport:
    glViewport( 0, 0, width, height );
    glFlush();
}

void Scene::init()
{
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_ALPHA_TEST);
    glEnable(GL_TEXTURE_2D);

    // Set the background color:
    // --------------------------------------
    glClearColor(m_backgroundColor.redF()
                 , m_backgroundColor.greenF()
                 , m_backgroundColor.blueF()
                 , m_backgroundColor.alphaF());

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT |  GL_ACCUM_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    std::for_each(m_viewports.begin(), m_viewports.end(), [](Viewport *view){
        view->init();
    });
    std::for_each(m_items.begin(), m_items.end(), [](RenderableItem *item){
        item->init();
    });

}

void Scene::mouseMoveEvent(QMouseEvent *event)
{
    QPoint pos(event->pos().x(), event->pos().y());
    Viewport *item = viewportForPos(pos);
    if(item){
        item->mouseMoveEvent(event);
    }
}

void Scene::mousePressEvent(QMouseEvent *event)
{
    std::for_each(m_viewports.begin(), m_viewports.end(), [](Viewport *view){
        view->setSelected(false);
    });
    QPoint pos(event->pos().x(), event->pos().y());
    Viewport *item = viewportForPos(pos);
    if(item){
        item->mousePressEvent(event);
        item->setSelected(true);
    }
}

void Scene::mouseReleaseEvent(QMouseEvent *event)
{
    QPoint pos(event->pos().x(), event->pos().y());
    Viewport *item = viewportForPos(pos);
    if(item){
        item->mouseReleaseEvent(event);
    }
}

void Scene::wheelEvent(QWheelEvent *event)
{
    QPoint pos(event->pos().x(), event->pos().y());
    Viewport *item = viewportForPos(pos);
    if(item){
        item->wheelEvent(event);
    }
}

Viewport * Scene::viewportForPos(const QPoint &pos) const
{
    QVector<Viewport *> ports = m_viewports;
    std::sort(ports.begin(), ports.end(), [](Viewport *a, Viewport *b) -> bool{
        return b->viewRect().contains(a->viewRect());
    });

    auto it = std::find_if(ports.begin(), ports.end(), [pos](Viewport *view) -> bool{
            return view->viewRect().contains(pos);
    });

    if(it != ports.end()){
        return *it;
    }

    return Q_NULLPTR;
}

Viewport *Scene::selectedViewport()
{
    auto it = std::find_if(m_viewports.begin(), m_viewports.end(), [](Viewport *view) -> bool{
            return view->isSelected();
    });

    if(it != m_viewports.end()){
        return *it;
    }
    return Q_NULLPTR;
}

void Scene::zoomIn()
{
    Viewport *view = selectedViewport();
    if(view){
        view->camera()->zoomIn();
    }
}

void Scene::zoomOut()
{
    Viewport *view = selectedViewport();
    if(view){
        view->camera()->zoomOut();
    }
}

QSize Scene::size() const
{
    return m_size;
}

void Scene::setSize(const QSize &size)
{
    m_size = size;
}

void Scene::addViewport(Viewport *view, const QString &name)
{
    if(view){
        m_nameViewportMap.insert(name, m_viewports.size());
        m_viewports.append(view);
        view->setParent(this);
    }
}

void Scene::addItem(RenderableItem *item, const QString &name)
{
    if(item){
        m_nameItemMap.insert(name, m_items.size());
        m_items.append(item);
        item->setParent(this);
    }
}

Viewport *Scene::viewport(const QString &name) const
{
    if(m_nameViewportMap.contains(name)){
        return m_viewports[m_nameViewportMap[name]];
    }

    return Q_NULLPTR;
}

RenderableItem *Scene::item(const QString &name) const
{
    if(m_nameItemMap.contains(name)){
        return m_items[m_nameItemMap[name]];
    }

    return Q_NULLPTR;
}

QColor Scene::backgroundColor() const
{
    return m_backgroundColor;
}

void Scene::setBackgroundColor(const QColor &backgroundColor)
{
    m_backgroundColor = backgroundColor;
}
