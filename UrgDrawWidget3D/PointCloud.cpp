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

    glLineWidth(m_pointSize);
    glBegin( GL_POINTS );

    ColorModel *cm = m_colorModel;

    float factor = m_rangeFactor;

    std::for_each(m_points.begin(), m_points.end(), [factor, cm](const QVector3D &point){
        if(cm){
            QColor color = cm->color(point.x(), point.y(), point.z(), 1);
            glColor4ub(color.red()
                       , color.green()
                       , color.blue()
                       , color.alpha());
        }
        glVertex3f( point.x() * factor, point.y() * factor, point.z() * factor);
    });

    glEnd();

    glEndList();
    m_initialized = true;
}

QVector<QVector3D> PointCloud::points() const
{
    return m_points;
}

void PointCloud::setPoints(const QVector<QVector3D> &points)
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
