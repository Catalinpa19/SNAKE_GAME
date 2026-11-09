#include "widgets/ArtImage.h"

#include <QFile>
#include <QPaintDevice>
#include <QPainter>

#include <algorithm>
#include <cmath>

ArtImage::ArtImage(const ArtSpec& spec)
    : m_name(spec.name)
{
    setReplacements(spec.replacements);
}

void ArtImage::setReplacements(const ArtReplacements& replacements)
{
    QFile file(QStringLiteral(":/art/%1.svg").arg(m_name));
    if (!file.open(QIODevice::ReadOnly))
        return;

    QByteArray svg = file.readAll();
    for (const auto& [from, to] : replacements)
        svg.replace(from.toUtf8(), to.toUtf8());
    m_renderer.load(svg);
}

void ArtImage::paint(QPainter& painter, const QRectF& target)
{
    if (!m_renderer.isValid())
        return;

    const QSizeF source = m_renderer.viewBoxF().size();
    const qreal dpr = painter.device()->devicePixelRatio();
    const qreal fit = std::min(target.width() / source.width(), target.height() / source.height());
    const qreal scale = std::max(1.0, std::floor(fit * dpr)) / dpr;

    QRectF drawn(QPointF(), source * scale);
    drawn.moveCenter(target.center());

    painter.save();
    painter.setRenderHint(QPainter::Antialiasing, false);
    m_renderer.render(&painter, drawn);
    painter.restore();
}
