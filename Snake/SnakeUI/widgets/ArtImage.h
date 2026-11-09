#pragma once

#include <QList>
#include <QString>
#include <QtSvg/QSvgRenderer>

#include <utility>

class QPainter;

using ArtReplacements = QList<std::pair<QString, QString>>;

struct ArtSpec
{
    ArtSpec(const QString& name = {}, const ArtReplacements& replacements = {})
        : name(name)
        , replacements(replacements)
    {
    }

    QString name;
    ArtReplacements replacements;
};

// Pixel-art SVG from ":/art/<name>.svg", drawn at a whole-pixel scale so the edges stay sharp.
class ArtImage
{
public:
    explicit ArtImage(const ArtSpec& spec);

    void setReplacements(const ArtReplacements& replacements);
    void paint(QPainter& painter, const QRectF& target);

private:
    QString m_name;
    QSvgRenderer m_renderer;
};
