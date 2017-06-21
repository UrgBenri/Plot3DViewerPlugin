#include "GridPlaneXY.h"

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

GridPlaneXY::GridPlaneXY(float xMin, float xMax, float yMin, float yMax, float z, float frequency)
    : m_xMin(xMin)
    , m_xMax(xMax)
    , m_yMin(yMin)
    , m_yMax(yMax)
    , m_plane_z(z)
    , m_frequency(frequency)
    , m_listIndex(0)
    , m_color(190,190,190)
{
}

GridPlaneXY::~GridPlaneXY()
{
    if(m_initialized){
        glDeleteLists(m_listIndex, 1);
    }
}

void GridPlaneXY::render()
{
    if(!m_initialized){
        init();
    }

    if(m_visible){
        glCallList(m_listIndex);
    }
}

void GridPlaneXY::init()
{
    if(m_listIndex > 0){
        glDeleteLists(m_listIndex, 1);
    }
    m_listIndex = glGenLists(1);
    glNewList(m_listIndex, GL_COMPILE);

    glPushMatrix();
    glLineWidth(1);

    glTranslatef(m_pos.x(), m_pos.y(), m_pos.z());

    glBegin(GL_LINES);

    glColor4ub(m_color.red(),m_color.green(),m_color.blue(),m_color.alpha());


    for (float y=m_yMin;y<=m_yMax;y+=m_frequency)
    {
        glVertex3f( m_xMin,y,m_plane_z );
        glVertex3f( m_xMax,y,m_plane_z );
    }

    for (float x=m_xMin;x<=m_xMax;x+=m_frequency)
    {
        glVertex3f( x,m_yMin,m_plane_z );
        glVertex3f( x,m_yMax,m_plane_z );
    }


    glEnd();

    glPopMatrix();

    glEndList();
    m_initialized = true;
}

QColor GridPlaneXY::color() const
{
    return m_color;
}

void GridPlaneXY::setColor(const QColor &color)
{
    m_color = color;
    m_initialized = false;
}
