#include "Axis.h"

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "MathUtils.h"
#include "gl_utils.h"

#include <QDebug>

Axis::Axis(float xmin, float ymin, float zmin, float xmax, float ymax, float zmax, float frecuency, float lineWidth, bool marks) :
    m_xmin(xmin),m_ymin(ymin),m_zmin(zmin),
    m_xmax(xmax),m_ymax(ymax),m_zmax(zmax),
    m_frecuency(frecuency),
    m_lineWidth(lineWidth),
    m_marks(marks),
    m_initialized(false),
    m_listIndex(0)
{

}

Axis::~Axis()
{
    if(m_initialized){
        glDeleteLists(m_listIndex, 1);
    }
}

void Axis::render()
{
    if(!m_initialized){
        init();
    }
    glCallList(m_listIndex);
}

void Axis::init()
{
    m_listIndex = glGenLists(1);
    glNewList(m_listIndex, GL_COMPILE);

    glPushMatrix();
    glLineWidth(m_lineWidth);
//    checkOpenGLError();

    glBegin( GL_LINES );
    //    glColor4ub(m_color.R,m_color.G,m_color.B,m_color.A);

    //X axis
    glColor4ub(255,0,0,255);
    glVertex3f( m_xmin, 0.0f, 0.0f );
    glVertex3f( m_xmax, 0.0f, 0.0f );
    //Y axis
    glColor4ub(0,255,0,255);
    glVertex3f( 0.0f, m_ymin, 0.0f );
    glVertex3f( 0.0f, m_ymax, 0.0f);
    //Z axis
    glColor4ub(0,0,255,255);
    glVertex3f( 0.0f, 0.0f, m_zmin );
    glVertex3f( 0.0f, 0.0f, m_zmax );

    glEnd();
    glPopMatrix();
//    checkOpenGLError();

    glLineWidth(1.0f);
//    checkOpenGLError();

//    glDisable (GL_BLEND);
//    checkOpenGLError();

    // Draw the "tick marks":
    if (m_marks ==true)
    {
        char n[50];

        // X axis
        glPushMatrix();
        glColor4ub(255,0,0,255);
        glTranslatef(m_xmin,.0f,.05f);
        glRotatef(180,0,0,1);
        glRotatef(90,1,0,0);
        for (float i = m_xmin ; i<= m_xmax ; i = i + m_frecuency)
        {
            std::snprintf(n,50,"%.02f",i);
            gl_utils::glDrawText(n, 0.25 /* scale */,  gl_utils::FILL );
            glTranslatef(-m_frecuency,0,0);
        }

        glPopMatrix();
        glPushMatrix();
        glTranslatef(m_xmax+0.5f*m_frecuency,0,0);
        glRotatef(180,0,0,1);
        glRotatef(90,1,0,0);
        gl_utils::glDrawText("+X", 0.3, gl_utils::NICE );
        glPopMatrix();


        // Y axis
        glPushMatrix();
        glColor4ub(0,255,0,255);
        glTranslatef(.0f,m_ymin,.05f);
        glRotatef(90,0,0,1);
        glRotatef(90,1,0,0);
        for (float i = m_ymin ; i<= m_ymax ; i = i + m_frecuency)
        {
//            if (std::abs(i)>1e-4)
//            {	// Dont draw the "0" more than once
                std::snprintf(n,50,"%.02f",i);
                gl_utils::glDrawText(n, 0.25 /* scale */,  gl_utils::FILL );
//            }
            glTranslatef(0,m_frecuency,0);
        }

        glPopMatrix();
        glPushMatrix();
        glTranslatef(0,m_ymax+1.0f*m_frecuency,0);
        glRotatef(-90,0,0,1);
        glRotatef(90,1,0,0);
        gl_utils::glDrawText("+Y", 0.3, gl_utils::NICE );
        glPopMatrix();


        // Z axis
        glPushMatrix();
        glColor4ub(0,0,255,255);
        glTranslatef(.0f,.0f,m_zmin);
        glRotatef(180,0,0,1);
        glRotatef(90,1,0,0);
        for (float i = m_zmin ; i<= m_zmax ; i = i + m_frecuency)
        {
//            if (std::abs(i)>1e-4)
//            {	// Dont draw the "0" more than once
                std::snprintf(n,50,"%.02f",i);
                gl_utils::glDrawText(n, 0.25 /* scale */,  gl_utils::FILL );
//            }
            glTranslatef(0,0,m_frecuency);
        }

        glPopMatrix();
        glPushMatrix();
        glTranslatef(0,0,m_zmax+0.5f*m_frecuency);
        glRotatef(180,0,0,1);
        glRotatef(90,1,0,0);
        gl_utils::glDrawText("+Z", 0.3, gl_utils::NICE );
        glPopMatrix();

    }

    glEndList();
    m_initialized = true;
}

void Axis::checkOpenGLError()
{
    int	 openglErr;
    if ( ( openglErr= glGetError()) != GL_NO_ERROR ) {
        qDebug() << "OpenGL error: " << QString( (char*)gluErrorString(openglErr) );
    }
}

void Axis::setFrequency(float f)
{
    m_frecuency=f;

    m_initialized = false;
}

void Axis::setAxisLimits(float xmin, float ymin, float zmin, float xmax, float ymax, float zmax)
{
    m_xmin=xmin; m_ymin=ymin; m_zmin=zmin;
    m_xmax=xmax; m_ymax=ymax; m_zmax=zmax;

    m_initialized = false;
}
