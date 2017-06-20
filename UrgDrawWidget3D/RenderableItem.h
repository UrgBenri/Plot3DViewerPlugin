#ifndef RENDERABLEITEM_H
#define RENDERABLEITEM_H

class RenderableItem
{
public:
    explicit RenderableItem();

    virtual void render()  = 0;
    virtual void init() ;
};

#endif // RENDERABLEITEM_H
