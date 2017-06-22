#include "Axis.h"

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "MathUtils.h"
#include "gl_utils.h"

#include <QDebug>

Axis::Axis(QObject *parent
           , float xmin, float ymin, float zmin
           , float xmax, float ymax, float zmax
           , float frecuency, float lineWidth, bool marks)
    : RenderableItem (parent)
    , m_xmin(xmin),m_ymin(ymin),m_zmin(zmin)
    , m_xmax(xmax),m_ymax(ymax),m_zmax(zmax)
    , m_frequency(frecuency)
    , m_lineWidth(lineWidth)
    , m_textScale(0.1f)
    , m_listIndex(0)
{
    for (int i=0;i<3;i++) m_marks[i] = marks;

    //x:180, 0, 90
    m_textRot[0][0] = 180.f; m_textRot[0][1] = 0.f;  m_textRot[0][2] = 90.f;
    //y:90, 0, 90
    m_textRot[1][0] =  90.f; m_textRot[1][1] = 0.f;  m_textRot[1][2] = 90.f;
    //z:180, 0, 90
    m_textRot[2][0] = 180.f; m_textRot[2][1] = 0.f;  m_textRot[2][2] = 90.f;
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
    if(m_visible){
        glCallList(m_listIndex);
    }
}

void Axis::init()
{
    if(m_listIndex > 0){
        glDeleteLists(m_listIndex, 1);
    }
    m_listIndex = glGenLists(1);
    glNewList(m_listIndex, GL_COMPILE);

    glPushMatrix();
    glLoadIdentity();

    glEnable (GL_BLEND);
    //        checkOpenGLError();
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //        checkOpenGLError();

    //        ASSERT_(m_frequency>=0);

    glLineWidth(m_lineWidth);
//    checkOpenGLError();
    glBegin( GL_LINES );

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
//    checkOpenGLError();

    glLineWidth(1.0f);
//    checkOpenGLError();

    glDisable (GL_BLEND);
//    checkOpenGLError();
    glPopMatrix();

    // Draw the "tick marks":
    char n[50];
    if (m_marks[0])
    {
        // X axis
        glPushMatrix();
        glLoadIdentity();
        glColor4ub(255,0,0,255);
        glTranslatef(m_xmin,.0f,.05f);
        for (float i = m_xmin ; i<= m_xmax ; i = i + m_frequency)
        {
            std::snprintf(n,50,"%.02f",i);
            glPushMatrix();
            glRotatef(m_textRot[0][0],0,0,1);
            glRotatef(m_textRot[0][1],0,1,0);
            glRotatef(m_textRot[0][2],1,0,0);
            gl_utils::glDrawText(n, m_textScale,  gl_utils::FILL );
            glPopMatrix();
            glTranslatef(m_frequency,0,0);
        }

        glPopMatrix();
        glPushMatrix();
        glTranslatef(m_xmax+1.0f*m_frequency,0,0);
        glRotatef(m_textRot[0][0],0,0,1);
        glRotatef(m_textRot[0][1],0,1,0);
        glRotatef(m_textRot[0][2],1,0,0);
        gl_utils::glDrawText("+X", m_textScale*1.2, gl_utils::NICE );
        glPopMatrix();
    }
    if (m_marks[1])
    {
        // Y axis
        glPushMatrix();
        glLoadIdentity();
        glColor4ub(0,255,0,255);
        glTranslatef(.0f,m_ymin,.05f);
        for (float i = m_ymin ; i<= m_ymax ; i = i + m_frequency)
        {
            if (std::abs(i)>1e-4)
            {	// Dont draw the "0" more than once
                std::snprintf(n,50,"%.02f",i);
                glPushMatrix();
                glRotatef(m_textRot[1][0],0,0,1);
                glRotatef(m_textRot[1][1],0,1,0);
                glRotatef(m_textRot[1][2],1,0,0);
                gl_utils::glDrawText(n, m_textScale,  gl_utils::FILL );
                glPopMatrix();
            }
            glTranslatef(0,m_frequency,0);
        }

        glPopMatrix();
        glPushMatrix();
        glTranslatef(0,m_ymax+.5f*m_frequency,0);
        glRotatef(m_textRot[1][0],0,0,1);
        glRotatef(m_textRot[1][1],0,1,0);
        glRotatef(m_textRot[1][2],1,0,0);
        gl_utils::glDrawText("+Y", m_textScale*1.2, gl_utils::NICE );
        glPopMatrix();
    }
    if (m_marks[2])
    {
        // Z axis
        glPushMatrix();
        glLoadIdentity();
        glColor4ub(0,0,255,255);
        glTranslatef(.0f,.0f,m_zmin);
        for (float i = m_zmin ; i<= m_zmax ; i = i + m_frequency)
        {
            if (std::abs(i)>1e-4)
            {	// Dont draw the "0" more than once
                glPushMatrix();
                glRotatef(m_textRot[2][0],0,0,1);
                glRotatef(m_textRot[2][1],0,1,0);
                glRotatef(m_textRot[2][2],1,0,0);
                std::snprintf(n,50,"%.02f",i);
                gl_utils::glDrawText(n, m_textScale,  gl_utils::FILL );
                glPopMatrix();
            }
            glTranslatef(0,0,m_frequency);
        }

        glPopMatrix();
        glPushMatrix();
        glTranslatef(0,0,m_zmax+0.5f*m_frequency);
        glRotatef(m_textRot[2][0],0,0,1);
        glRotatef(m_textRot[2][1],0,1,0);
        glRotatef(m_textRot[2][2],1,0,0);
        gl_utils::glDrawText("+Z", m_textScale*1.2, gl_utils::NICE );
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
//    ASSERT_(f>0);
    m_frequency=f;
    //CRenderizableDisplayList::notifyChange();
}

float Axis::getFrequency() const {
    return m_frequency;
}
void Axis::setLineWidth(float w) {
    m_lineWidth=w;
    //CRenderizableDisplayList::notifyChange();
}
float Axis::getLineWidth() const {
    return  m_lineWidth;
}

void Axis::enableTickMarks(bool v) {
    for (int i=0;i<3;i++) m_marks[i]=v;
    //CRenderizableDisplayList::notifyChange();
}
void Axis::enableTickMarks(bool show_x, bool show_y, bool show_z)
{
    m_marks[0]=show_x;
    m_marks[1]=show_y;
    m_marks[2]=show_z;
    //CRenderizableDisplayList::notifyChange();
}
void Axis::setTextScale(float f) {
//    ASSERT_(f>0);
    m_textScale=f;
    //CRenderizableDisplayList::notifyChange();
}
float Axis::getTextScale() const {
    return m_textScale;
}

void Axis::setAxisLimits(float xmin,float ymin, float zmin, float xmax,float ymax, float zmax)
{
    m_xmin=xmin; m_ymin=ymin; m_zmin=zmin;
    m_xmax=xmax; m_ymax=ymax; m_zmax=zmax;
    //CRenderizableDisplayList::notifyChange();
}
void Axis::setTextLabelOrientation(int axis, float yaw_deg, float pitch_deg, float roll_deg)
{
//    ASSERT_(axis>=0 && axis<3);
    m_textRot[axis][0]=yaw_deg;
    m_textRot[axis][1]=pitch_deg;
    m_textRot[axis][2]=roll_deg;
}
void Axis::getTextLabelOrientation(int axis, float &yaw_deg, float &pitch_deg, float &roll_deg) const
{
//    ASSERT_(axis>=0 && axis<3);
    yaw_deg = m_textRot[axis][0];
    pitch_deg = m_textRot[axis][1];
    roll_deg = m_textRot[axis][2];
}
