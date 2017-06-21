#include "Camera.h"

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <QDebug>

#include "MathUtils.h"

namespace
{
const qreal SENSIBILITY_DEG_PER_PIXEL = 0.1f;
const float EPSILON = 1.0e-5;
const float InitialCameraZoomDistance = 10;
}

Camera::Camera(QObject *parent)
    : RenderableItem (parent)
{
    cameraPointingX = 0;
    cameraPointingY = 0;
    cameraPointingZ	= 0;
    cameraZoomDistance = 2;
    cameraElevationDeg = 35; //45;
    cameraAzimuthDeg = -90; //45
    cameraIsProjective = true;
    cameraProjectiveFOVDeg = 45;
    m_clip_min = 0.001;
    m_clip_max = 10000;

    m_now_pressed = false;
    m_now_moving = false;
}

Camera::~Camera()
{

}

void Camera::render()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // Set the camera
    //---------------
    GLint	win_dims[4];
    glGetIntegerv( GL_VIEWPORT, win_dims );
    int vw = win_dims[2];
    int vh = win_dims[3];
    m_viewportRect = QRect(win_dims[0], win_dims[1], win_dims[2], win_dims[3]);
    m_lastProjMat.viewport_width  = vw;
    m_lastProjMat.viewport_height = vh;

    m_lastProjMat.azimuth = deg2rad(cameraAzimuthDeg);
    m_lastProjMat.elev    = deg2rad(cameraElevationDeg);

    m_lastProjMat.eye.setX(cameraPointingX +  max(0.01f,cameraZoomDistance) * cos(m_lastProjMat.azimuth) * cos(m_lastProjMat.elev));
    m_lastProjMat.eye.setY(cameraPointingY +  max(0.01f,cameraZoomDistance) * sin(m_lastProjMat.azimuth) * cos(m_lastProjMat.elev));
    m_lastProjMat.eye.setZ(cameraPointingZ +  max(0.01f,cameraZoomDistance) * sin(m_lastProjMat.elev));

    if (fabs(fabs(cameraElevationDeg)-90.f)>EPSILON) {
        m_lastProjMat.up.setX(0.f);
        m_lastProjMat.up.setY(0.f);
        m_lastProjMat.up.setZ(11.f);
    } else {
        float sgn = cameraElevationDeg>0 ? 1:-1;
        m_lastProjMat.up.setX(-cos(deg2rad(cameraElevationDeg))*sgn);
        m_lastProjMat.up.setY(sin(deg2rad(cameraElevationDeg))*sgn);
        m_lastProjMat.up.setZ(0.f);
    }

    m_lastProjMat.is_projective = cameraIsProjective;
    m_lastProjMat.FOV           = cameraProjectiveFOVDeg;
    m_lastProjMat.pointing.setX(cameraPointingX);
    m_lastProjMat.pointing.setY(cameraPointingY);
    m_lastProjMat.pointing.setZ(cameraPointingZ);
    m_lastProjMat.zoom    		= cameraZoomDistance;

    if (cameraIsProjective) {
        gluPerspective( cameraProjectiveFOVDeg, vw/double(vh),m_clip_min,m_clip_max);
        checkOpenGLError();
    } else {
        const double ratio = vw/double(vh);
        double Ax = cameraZoomDistance*0.5;
        double Ay = cameraZoomDistance*0.5;

        if (ratio>1)
            Ax *= ratio;
        else
        {
            if (ratio!=0) Ay /=ratio;
        }

        glOrtho( -Ax,Ax,-Ay,Ay,-0.5*m_clip_max,0.5*m_clip_max);
        checkOpenGLError();
    }

    gluLookAt(
                m_lastProjMat.eye.x(),
                m_lastProjMat.eye.y(),
                m_lastProjMat.eye.z(),
                m_lastProjMat.pointing.x(),
                m_lastProjMat.pointing.y(),
                m_lastProjMat.pointing.z(),
                m_lastProjMat.up.x(),
                m_lastProjMat.up.y(),
                m_lastProjMat.up.z());
    checkOpenGLError();
}

void Camera::checkOpenGLError()
{
    int	 openglErr;
    if ( ( openglErr= glGetError()) != GL_NO_ERROR ) {
        qDebug() << "OpenGL error: " << QString( (char*)gluErrorString(openglErr) );
    }
}

void Camera::mousePressEvent(QMouseEvent* event)
{
    QPoint pos(event->pos().x(), event->pos().y());
    if(m_viewportRect.contains(pos)){
        mouseLastX = event->pos().x();
        mouseLastY = event->pos().y();
        m_mouseButton = event->button();
        if(event->button() == Qt::LeftButton || event->button() == Qt::RightButton){
            m_now_pressed = true;
            m_now_moving = false;
            event->accept();
        }
    }
}

void Camera::mouseMoveEvent(QMouseEvent *event)
{
    //    qDebug() << "UrgDrawWidget3D::mouseMoveEvent";
    QPoint pos(event->pos().x(), event->pos().y());
    if(m_viewportRect.contains(pos)){
        int X = event->pos().x();
        int Y = event->pos().y();
        // Proxy variables to cache the changes:
        float	cameraPointingX = this->cameraPointingX;
        float	cameraPointingY = this->cameraPointingY;
        float 	cameraPointingZ = this->cameraPointingZ;
        float	cameraZoomDistance = this->cameraZoomDistance;
        float	cameraElevationDeg = this->cameraElevationDeg;
        float 	cameraAzimuthDeg = this->cameraAzimuthDeg;

        if (m_now_pressed && m_mouseButton == Qt::LeftButton){
            if ( event->modifiers() == Qt::ShiftModifier  )
            {
                // Zoom:
                cameraZoomDistance *= exp(0.01*(Y - mouseLastY));
                if (cameraZoomDistance<0.01) cameraZoomDistance = 0.01f;

                float	Az = -0.05*(X - mouseLastX);
                float	D = 0.001*cameraZoomDistance;
                cameraPointingZ += D*Az;
            }
            else if (event->modifiers() == Qt::ControlModifier)
            {
                // Rotate camera pointing direction:
                float	eye_x = cameraPointingX +  max(0.01f,(cameraZoomDistance)) * cos(deg2rad(cameraAzimuthDeg))*cos(deg2rad(cameraElevationDeg));
                float	eye_y = cameraPointingY +  max(0.01f,(cameraZoomDistance)) * sin(deg2rad(cameraAzimuthDeg))*cos(deg2rad(cameraElevationDeg));
                float	eye_z = cameraPointingZ +  max(0.01f,(cameraZoomDistance)) * sin(deg2rad(cameraElevationDeg));

                float A_AzimuthDeg = -SENSIBILITY_DEG_PER_PIXEL*(X - mouseLastX);
                float A_ElevationDeg = SENSIBILITY_DEG_PER_PIXEL*(Y - mouseLastY);

                // Orbit camera:
                cameraAzimuthDeg += A_AzimuthDeg;
                cameraElevationDeg += A_ElevationDeg;
                if (cameraElevationDeg<-90) cameraElevationDeg = -90;
                if (cameraElevationDeg>90) cameraElevationDeg = 90;

                // Move cameraPointing pos:
                cameraPointingX = eye_x - max(0.01f,(cameraZoomDistance)) * cos(deg2rad(cameraAzimuthDeg))*cos(deg2rad(cameraElevationDeg));
                cameraPointingY = eye_y - max(0.01f,(cameraZoomDistance)) * sin(deg2rad(cameraAzimuthDeg))*cos(deg2rad(cameraElevationDeg));
                cameraPointingZ = eye_z - max(0.01f,(cameraZoomDistance)) * sin(deg2rad(cameraElevationDeg));

            }
            else
            {
                switch(m_viewMode){
                    case CameraViewMode:
                        // Orbit camera:
                        cameraAzimuthDeg -= 0.2*(X - mouseLastX);
                        cameraElevationDeg += 0.2*(Y - mouseLastY);
                        if (cameraElevationDeg<-90.f) cameraElevationDeg = -90.f;
                        if (cameraElevationDeg>90.f) cameraElevationDeg = 90.f;
                    break;
                    case TopViewMode:
                        cameraPointingX -= 0.005*cameraZoomDistance*(X - mouseLastX);
                        cameraPointingY += 0.005*cameraZoomDistance*(Y - mouseLastY);
                    break;
                    case LeftViewMode:
                        cameraPointingX -= 0.005*cameraZoomDistance*(X - mouseLastX);
                        cameraPointingZ += 0.005*cameraZoomDistance*(Y - mouseLastY);
                    break;
                    case FrontViewMode:
                        cameraPointingY -= 0.005*cameraZoomDistance*(X - mouseLastX);
                        cameraPointingZ += 0.005*cameraZoomDistance*(Y - mouseLastY);
                    break;
                }


            }

            mouseLastX = X;
            mouseLastY = Y;

            // Potential user filter:
            OnUserManuallyMovesCamera(cameraPointingX
                                      , cameraPointingY
                                      , cameraPointingZ
                                      , cameraZoomDistance
                                      , cameraElevationDeg
                                      , cameraAzimuthDeg);
            event->accept();
        }
        else if(m_now_pressed && m_mouseButton == Qt::RightButton && m_viewMode == CameraViewMode){
            float	Ay = -(X - mouseLastX);
            float	Ax = -(Y - mouseLastY);
            float	D = 0.001*cameraZoomDistance;
            cameraPointingX += D*(Ax*cos(deg2rad(cameraAzimuthDeg)) - Ay*sin(deg2rad(cameraAzimuthDeg)));
            cameraPointingY += D*(Ax*sin(deg2rad(cameraAzimuthDeg)) + Ay*cos(deg2rad(cameraAzimuthDeg)));

            mouseLastX = X;
            mouseLastY = Y;

            // Potential user filter:
            OnUserManuallyMovesCamera(cameraPointingX
                                      , cameraPointingY
                                      , cameraPointingZ
                                      , cameraZoomDistance
                                      , cameraElevationDeg
                                      , cameraAzimuthDeg);

            event->accept();
        }

        qDebug() << "Camera:  " << cameraZoomDistance << cameraAzimuthDeg << cameraElevationDeg << ", pointing at:" << cameraPointingX << cameraPointingY << cameraPointingZ;
    }
}

void Camera::mouseReleaseEvent(QMouseEvent* event)
{
    QPoint pos(event->pos().x(), event->pos().y());
    if(m_viewportRect.contains(pos)){
        mouseLastX = event->pos().x();
        mouseLastY = event->pos().y();
        if(m_mouseButton == Qt::LeftButton){
            m_now_pressed = false;
            event->accept();
        }
    }
}

void Camera::wheelEvent(QWheelEvent* event)
{
    QPoint pos(event->pos().x(), event->pos().y());
    if(m_viewportRect.contains(pos)){
        float cameraZoomDistance = this->cameraZoomDistance;
        int degrees = event->delta() / 8;
        int steps = degrees / 15;

        cameraZoomDistance *= 1 - 0.03f*steps;

        // Potential user filter:
        OnUserManuallyMovesCamera(cameraPointingX, cameraPointingY, cameraPointingZ, cameraZoomDistance, cameraElevationDeg, cameraAzimuthDeg);

        event->accept();

        //    qCritical() << "Camera:  " << cameraZoomDistance << cameraAzimuthDeg << cameraElevationDeg << ", pointing at:" << cameraPointingX << cameraPointingY << cameraPointingZ;

    }
}

void Camera::zoomIn()
{
    float cameraZoomDistance = this->cameraZoomDistance;
    int degrees = 90;
    int steps = degrees / 15;

    cameraZoomDistance *= 1 - 0.03f*steps;

    // Potential user filter:
    OnUserManuallyMovesCamera(cameraPointingX, cameraPointingY, cameraPointingZ, cameraZoomDistance, cameraElevationDeg, cameraAzimuthDeg);
}

void Camera::zoomOut()
{
    float cameraZoomDistance = this->cameraZoomDistance;
    int degrees = -90;
    int steps = degrees / 15;

    cameraZoomDistance *= 1 - 0.03f*steps;

    // Potential user filter:
    OnUserManuallyMovesCamera(cameraPointingX, cameraPointingY, cameraPointingZ, cameraZoomDistance, cameraElevationDeg, cameraAzimuthDeg);
}

void Camera::reset()
{
    switch(m_viewMode){
        case CameraViewMode:
            cameraPointingX = 0;
            cameraPointingY = 0;
            cameraPointingZ	= 0;
            cameraZoomDistance = InitialCameraZoomDistance;
            cameraElevationDeg = 45; //45;
            cameraAzimuthDeg = -45; //45
            cameraIsProjective = true;
        break;
        case FrontViewMode:
            cameraPointingX = 0;
            cameraPointingY = 0;
            cameraPointingZ	= 0;
            cameraZoomDistance = InitialCameraZoomDistance;
            cameraElevationDeg = 0; //45;
            cameraAzimuthDeg = 0; //45
            cameraIsProjective = false;
        break;
        case LeftViewMode:
            cameraPointingX = 0;
            cameraPointingY = 0;
            cameraPointingZ	= 0;
            cameraZoomDistance = InitialCameraZoomDistance;
            cameraElevationDeg = 0; //45;
            cameraAzimuthDeg = -90; //45
            cameraIsProjective = false;
        break;
        case TopViewMode:
            cameraPointingX = 0;
            cameraPointingY = 0;
            cameraPointingZ	= 0;
            cameraZoomDistance = InitialCameraZoomDistance;
            cameraElevationDeg = 90; //45;
            cameraAzimuthDeg = 0; //45
            cameraIsProjective = false;
        break;

    }
    m_initialized = false;
}

QRect Camera::viewportRect() const
{
    return m_viewportRect;
}

Camera::ViewMode Camera::viewMode() const
{
    return m_viewMode;
}

void Camera::setViewMode(const ViewMode &mode)
{
    m_viewMode = mode;

    reset();
}

void Camera::OnUserManuallyMovesCamera(
        float	new_cameraPointingX,
        float 	new_cameraPointingY,
        float 	new_cameraPointingZ,
        float	new_cameraZoomDistance,
        float	new_cameraElevationDeg,
        float	new_cameraAzimuthDeg )
{
    cameraPointingX 	= new_cameraPointingX;
    cameraPointingY 	= new_cameraPointingY;
    cameraPointingZ 	= new_cameraPointingZ;
    cameraZoomDistance 	= new_cameraZoomDistance;
    cameraElevationDeg 	= new_cameraElevationDeg ;
    cameraAzimuthDeg 	= new_cameraAzimuthDeg;
}
