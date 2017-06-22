#ifndef TEXT_H
#define TEXT_H

#include <QColor>

#include "RenderableItem.h"

class Text: public RenderableItem
{
    Q_OBJECT
public:
    explicit Text(QObject *parent = Q_NULLPTR, const QString &text = "");
    virtual ~Text();

    virtual void render() Q_DECL_OVERRIDE;

    QColor color() const;
    void setColor(const QColor &color);

private:
    QColor m_color;
    QString m_text;
};

#endif // TEXT_H
