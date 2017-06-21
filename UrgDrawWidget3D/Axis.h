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

    void setLineWidth(float w) { m_lineWidth = w;}
    float getLineWidth() const { return  m_lineWidth;}

    void enableTickMarks(bool v=true) { m_marks=v;}
    virtual void render() override;
    virtual void init() override;

protected:
    float	m_xmin,m_ymin,m_zmin;
    float	m_xmax,m_ymax,m_zmax;
    float	m_frecuency;
    float	m_lineWidth;
    bool	m_marks;
    unsigned int m_listIndex;

private:
    void checkOpenGLError();

};

#endif // AXIS_H
