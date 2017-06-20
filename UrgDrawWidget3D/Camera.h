#ifndef CAMERA_H
#define CAMERA_H

#include <QVector3D>
#include <QMouseEvent>
#include <QWheelEvent>

#include "RenderableItem.h"

class Camera: public RenderableItem
{
public:
    Camera();

    virtual void render() override;

    void mouseMoveEvent(QMouseEvent* event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
    void zoomIn();
    void zoomOut();

    enum ViewMode {
        CameraViewMode = 0,
        FrontViewMode,
        LeftViewMode,
        TopViewMode,
        BottomViewMode,
        RightViewMode,
        RearViewMode,
    };

    QRect viewportRect() const;
    void setViewportRect(const QRect &viewportRect);

    ViewMode viewMode() const;
    void setViewMode(const ViewMode &mode);

private:
    int mouseLastX;
    int mouseLastY;
    float cameraPointingX;
    float cameraPointingY;
    float cameraPointingZ;
    float cameraZoomDistance;
    float cameraElevationDeg;
    float cameraAzimuthDeg;
    float cameraProjectiveFOVDeg;
    bool cameraIsProjective;
    QRect m_viewportRect;

    bool m_now_pressed;
    bool m_now_moving;

    ViewMode m_viewMode;

    Qt::MouseButton m_mouseButton;

    struct TLastProjectiveMatrixInfo
    {
        TLastProjectiveMatrixInfo() : is_projective(true),eye(0,0,0),pointing(0,0,0),up(0,0,0), FOV(30), viewport_width(640), viewport_height(480), azimuth(0), elev(0), zoom(1)
        {}
        bool is_projective;  // true: projective, false: ortho
        QVector3D 	eye;		//!< The camera is here.
        QVector3D 	pointing; 	//!< The camera points to here
        QVector3D	up; 		//!< Up vector of the camera.
        float FOV; //!< FOV in degrees.
        size_t viewport_width, viewport_height; //!< In pixels. This may be smaller than the total render window.
        float azimuth, elev; //!< Camera elev & azimuth, in radians.
        float zoom;
    };
    mutable TLastProjectiveMatrixInfo m_lastProjMat;  //!< Info updated with each "render()" and used in "get3DRayForPixelCoord"
    float  m_clip_min, m_clip_max; //!< The min/max clip depth distances (default: 0.1 - 10000)

    void checkOpenGLError();
    void OnUserManuallyMovesCamera(float new_cameraPointingX, float new_cameraPointingY, float new_cameraPointingZ, float new_cameraZoomDistance, float new_cameraElevationDeg, float new_cameraAzimuthDeg);
};

#endif // CAMERA_H
