#ifndef AXIS_H
#define AXIS_H

#include "RenderableItem.h"

class Axis: public RenderableItem
{
    Q_OBJECT
public:
    explicit Axis(QObject *parent = Q_NULLPTR
            , float xmin = -1.0f
            , float ymin = -1.0f
            , float zmin = -1.0f
            , float xmax = 1.0f
            , float ymax = 1.0f
            , float zmax = 1.0f
            , float frecuency = 0.25f
            , float lineWidth = 3.0f
            , bool marks = false);
    virtual ~Axis();

    void setAxisLimits(float xmin,float ymin, float zmin, float xmax,float ymax, float zmax);
    void setFrequency(float f); //!< Changes the frequency of the "ticks"
    float getFrequency() const;
    void setLineWidth(float w);
    float getLineWidth() const;
    void setTextScale(float f); //!< Changes the size of text labels (default:0.25)
    float getTextScale() const;
    void setTextLabelOrientation(int axis, float yaw_deg, float pitch_deg, float roll_deg); //!< axis: {0,1,2}=>{X,Y,Z}
    void getTextLabelOrientation(int axis, float &yaw_deg, float &pitch_deg, float &roll_deg) const; //!< axis: {0,1,2}=>{X,Y,Z}

    void enableTickMarks(bool v=true);
    void enableTickMarks(bool show_x, bool show_y, bool show_z);

    virtual void render() override;
    virtual void init() override;

protected:
    float	m_xmin,m_ymin,m_zmin;
    float	m_xmax,m_ymax,m_zmax;
    float	m_frequency;
    float	m_lineWidth;
    bool	m_marks[3]; //!< draw marks for X,Y,Z
    float	m_textScale;
    float	m_textRot[3][3]; // {x,y,z},{yaw,pitch,roll}
    unsigned int m_listIndex;

private:
    void checkOpenGLError();

};

#endif // AXIS_H
