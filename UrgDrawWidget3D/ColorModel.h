#ifndef COLORMODEL_H
#define COLORMODEL_H

#include <QColor>

class ColorModel
{
public:
    explicit ColorModel();

    virtual QColor color(float x, float y, float z, float intensity) const = 0;
};

#endif // COLORMODEL_H
