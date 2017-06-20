#ifndef GRIDPLANEXY_H
#define GRIDPLANEXY_H

#include "RenderableItem.h"

class GridPlaneXY: public RenderableItem
{
public:
    GridPlaneXY(float				xMin = -10,
                float				xMax = 10 ,
                float				yMin = -10,
                float				yMax = 10,
                float				z    = 0,
                float				frequency = 1
                );
    virtual ~GridPlaneXY();

    virtual void render() override;
    virtual void init() override;

protected:
    float	m_xMin, m_xMax;
    float	m_yMin, m_yMax;
    float	m_plane_z;
    float	m_frequency;
    bool    m_initialized;
    unsigned int m_listIndex;
};

#endif // GRIDPLANEXY_H
