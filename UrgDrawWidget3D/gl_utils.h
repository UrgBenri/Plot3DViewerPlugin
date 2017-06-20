#ifndef GL_UTILS_H
#define GL_UTILS_H

#include <string>
#include <QPointF>

class gl_utils
{
public:
    gl_utils();

    /** Different style for vectorized font rendering \sa T2DTextData */
    enum TOpenGLFontStyle {
        FILL = 0,       ///< renders glyphs as filled polygons
        OUTLINE = 1,    ///< renders glyphs as outlines with GL_LINES
        NICE = 2        ///< renders glyphs filled with antialiased outlines
    };

    static QPointF glDrawText(const std::string & text
                              , const double textScale
                              , enum TOpenGLFontStyle style = NICE
            , double spacing = 1.5
            , double kerning = 0.1);
    QPointF glGetExtends(const std::string &text, const double textScale, double spacing, double kerning);
    static void glSetFont(const std::string &fontname);
    const std::string &glGetFont();
};

#endif // GL_UTILS_H
