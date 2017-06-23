#include "PointCloud.h"

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

PointCloud::PointCloud(QObject *parent)
    : RenderableItem (parent)
    , m_listIndex(0)
    , m_pointSize(2)
    , m_rangeFactor(0.001)
    , m_color(Qt::white)
{

}

PointCloud::~PointCloud()
{
    if(m_initialized){
        glDeleteLists(m_listIndex, 1);
    }
}


void PointCloud::render()
{
    if(!m_initialized){
        init();
    }
    if(m_visible){
        glCallList(m_listIndex);
    }
}

void PointCloud::init()
{
    if(m_listIndex > 0){
        glDeleteLists(m_listIndex, 1);
    }
    m_listIndex = glGenLists(1);
    glNewList(m_listIndex, GL_COMPILE);

    glPointSize(m_pointSize);
    glBegin( GL_POINTS );


    float factor = m_rangeFactor;

    if(!m_colorModel){
        glColor4ub(m_color.red()
                   , m_color.green()
                   , m_color.blue()
                   , m_color.alpha());
    }

   for(int i = 0; i < m_points.size(); ++i){
        QVector4D point = m_points[i];
        if(m_colorModel){
            QColor color = m_colorModel->color(i, point.x(), point.y(), point.z(), point.w());
            glColor4ub(color.red()
                       , color.green()
                       , color.blue()
                       , color.alpha());
        }
        glVertex3f( point.x() * factor, point.y() * factor, point.z() * factor);
    };

    glEnd();

    glEndList();
    m_initialized = true;
}

QVector<QVector4D> PointCloud::points() const
{
    return m_points;
}

void PointCloud::setPoints(const QVector<QVector4D> &points)
{
    m_points = points;
    m_initialized = false;
}

float PointCloud::pointSize() const
{
    return m_pointSize;
}

void PointCloud::setPointSize(float pointSize)
{
    m_pointSize = pointSize;
    m_initialized = false;
}

float PointCloud::rangeFactor() const
{
    return m_rangeFactor;
}

void PointCloud::setRangeFactor(float rangeFactor)
{
    m_rangeFactor = rangeFactor;
    m_initialized = false;
}

QColor PointCloud::color() const
{
    return m_color;
}

void PointCloud::setColor(const QColor &color)
{
    m_color = color;
    m_initialized = false;
}
