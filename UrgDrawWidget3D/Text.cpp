#include "Text.h"

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "gl_utils.h"

Text::Text(QObject *parent, const QString &text)
    : RenderableItem(parent)
    , m_text(text)
    , m_color(255, 255, 255)
{

}

Text::~Text()
{

}


void Text::render()
{
    glDisable(GL_DEPTH_TEST);
    //    glDisable(GL_LIGHTING);

    glColor4ub(m_color.red()
               , m_color.green()
               , m_color.blue()
               , m_color.alpha());
    // Set the "cursor" to the XYZ position:
    glRasterPos3f(0,0,0);//m_x,m_y,m_z);

    // Call the lists for drawing the text:
    gl_utils::renderTextBitmap( m_text.toLatin1().constData(), GLUT_BITMAP_TIMES_ROMAN_10 );

    //    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);
}

QColor Text::color() const
{
    return m_color;
}

void Text::setColor(const QColor &color)
{
    m_color = color;
}
