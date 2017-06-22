#include "Viewport.h"

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <QDebug>
#include "gl_utils.h"

Viewport::Viewport(QObject *parent, const QRectF &rect)
    : RenderableItem(parent)
    , m_rect(rect)
{
    initMembers();
}

Viewport::~Viewport()
{

}

void Viewport::render()
{
    QRect vRect = viewportRect();

    glViewport(vRect.x(), vRect.y(), vRect.width(), vRect.height());
    glLoadIdentity();

    glScissor(vRect.x(), vRect.y(), vRect.width(), vRect.height());
    glEnable(GL_SCISSOR_TEST);

    GLdouble old_colors[4];

    glGetDoublev(GL_COLOR_CLEAR_VALUE, old_colors );
    glClearColor(m_backgroundColor.redF()
                 , m_backgroundColor.greenF()
                 , m_backgroundColor.blueF()
                 , m_backgroundColor.alphaF());

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT |  GL_ACCUM_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    glClearColor(old_colors[0],old_colors[1],old_colors[2],old_colors[3]);
    glDisable(GL_SCISSOR_TEST);

    m_camera.setViewportRect(viewRect());
    m_camera.render();

    // PART 2: Set the MODELVIEW matrix
    // --------------------------------------
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void Viewport::mouseMoveEvent(QMouseEvent *event)
{
    m_camera.mouseMoveEvent(event);
}

void Viewport::mousePressEvent(QMouseEvent *event)
{
    m_camera.mousePressEvent(event);
}

void Viewport::mouseReleaseEvent(QMouseEvent *event)
{
    m_camera.mouseReleaseEvent(event);
}

void Viewport::wheelEvent(QWheelEvent *event)
{
    m_camera.wheelEvent(event);
}

QSize Viewport::sceneSize() const
{
    return m_sceneSize;
}

void Viewport::setSceneSize(const QSize &sceneSize)
{
    m_sceneSize = sceneSize;
}

void Viewport::setRect(const QRectF &rect)
{
    m_rect = rect;
    m_initialized = false;
}

QRect Viewport::viewportRect() const
{
    GLint vx = GLint(m_sceneSize.width() * m_rect.x());
    GLint vy = GLint(m_sceneSize.height() * (1.0 - m_rect.y() - m_rect.height()));
    GLsizei vw = GLsizei(m_sceneSize.width() * m_rect.width());
    GLsizei vh = GLsizei(m_sceneSize.height()  * m_rect.height());

    return QRect(vx, vy, vw, vh);
}

QRect Viewport::viewRect() const
{
    GLint vx = GLint(m_sceneSize.width() * m_rect.x());
    GLint vy = GLint(m_sceneSize.height() * m_rect.y());
    GLsizei vw = GLsizei(m_sceneSize.width() * m_rect.width());
    GLsizei vh = GLsizei(m_sceneSize.height()  * m_rect.height());

    return QRect(vx, vy, vw, vh);
}

void Viewport::renderBorder()
{
    glPushMatrix();
    glLineWidth( m_borderWidth );
    if(isSelected()){
        glColor4f(m_selectedBorderColor.redF()
                  , m_selectedBorderColor.greenF()
                  , m_selectedBorderColor.blueF()
                  , m_selectedBorderColor.alphaF());
    } else {
        glColor4f(m_borderColor.redF()
                  , m_borderColor.greenF()
                  , m_borderColor.blueF()
                  , m_borderColor.alphaF());
    }
    glDisable(GL_DEPTH_TEST);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    //                glDisable(GL_LIGHTING);  // Disable lights when drawing lines
    glBegin( GL_LINE_LOOP );
    glVertex2f(-1,-1);
    glVertex2f(-1, 1);
    glVertex2f( 1, 1);
    glVertex2f( 1,-1);
    glEnd();
    //                glEnable(GL_LIGHTING);  // Disable lights when drawing lines

    if(!m_name.isEmpty()){
        glColor4ub(m_nameColor.red()
                   , m_nameColor.green()
                   , m_nameColor.blue()
                   , m_nameColor.alpha());

        char n[50];
        glRasterPos3f(-1,0.85,0);
        std::snprintf(n,50,"%s", m_name.toLatin1().constData());
        gl_utils::renderTextBitmap( n, GLUT_BITMAP_TIMES_ROMAN_24 );
    }

    glEnable(GL_DEPTH_TEST);
    glPopMatrix();
}

QString Viewport::name() const
{
    return m_name;
}

void Viewport::setName(const QString &name)
{
    m_name = name;
}

QColor Viewport::nameColor() const
{
    return m_nameColor;
}

void Viewport::setNameColor(const QColor &nameColor)
{
    m_nameColor = nameColor;
}

bool Viewport::isSelected() const
{
    return m_selected;
}

void Viewport::setSelected(bool selected)
{
    m_selected = selected;
}

QColor Viewport::borderColor() const
{
    return m_borderColor;
}

void Viewport::setBorderColor(const QColor &borderColor)
{
    m_borderColor = borderColor;
}

QColor Viewport::selectedBorderColor() const
{
    return m_selectedBorderColor;
}

void Viewport::setSelectedBorderColor(const QColor &selectedBorderColor)
{
    m_selectedBorderColor = selectedBorderColor;
}

void Viewport::initMembers()
{
    m_backgroundColor = QColor(108, 108, 132);
    m_borderWidth = int(2);
    m_borderColor = QColor(Qt::black);
    m_selectedBorderColor = QColor( Qt::yellow);
    m_name = QString("");
    m_nameColor = QColor(Qt::white);
    m_selected = false;
}
