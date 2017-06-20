#include "GridPlaneXY.h"

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

GridPlaneXY::GridPlaneXY(float xMin, float xMax, float yMin, float yMax, float z, float frequency) :
    m_xMin(xMin),
    m_xMax(xMax),
    m_yMin(yMin),
    m_yMax(yMax),
    m_plane_z(z),
    m_frequency(frequency),
    m_initialized(false),
    m_listIndex(0)
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
    glCallList(m_listIndex);
}

void GridPlaneXY::init()
{
    m_listIndex = glGenLists(1);
    glNewList(m_listIndex, GL_COMPILE);

    glPushMatrix();
    glLineWidth(1);

    glBegin(GL_LINES);

    glColor4ub(180,180,180,255);


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
