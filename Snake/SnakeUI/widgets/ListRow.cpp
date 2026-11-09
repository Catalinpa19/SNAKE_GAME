#include "widgets/ListRow.h"

#include "app/Theme.h"
#include "widgets/ArtWidget.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QVBoxLayout>

namespace
{
constexpr int kRowPadding = 8;
constexpr int kRowSpacing = 10;
constexpr int kLineSpacing = 6;
} // namespace

ListRow::ListRow(const ArtSpec& art, const QSize& artSize, bool dimmed, QWidget* parent)
    : QFrame(parent)
    , m_dimmed(dimmed)
    , m_layout(new QHBoxLayout(this))
    , m_texts(new QVBoxLayout)
{
    setObjectName(QStringLiteral("listRow"));
    setProperty("dimmed", dimmed);

    m_layout->setContentsMargins(kRowPadding, kRowPadding, kRowPadding, kRowPadding);
    m_layout->setSpacing(kRowSpacing);

    auto* image = new ArtWidget(art, artSize, this);
    image->setOpacity(dimmed ? Theme::DimmedOpacity : 1.0);
    m_layout->addWidget(image);

    m_texts->setSpacing(2);
    m_layout->addLayout(m_texts, 1);
}

void ListRow::addTitle(const QString& text)
{
    addLabel(text, "rowTitle", m_texts);
}

void ListRow::addText(const QString& text)
{
    addLabel(text, "rowText", m_texts);
}

void ListRow::addArtLine(const QString& prefix, const ArtSpec& art, const QSize& artSize, const QString& text)
{
    auto* line = new QHBoxLayout;
    line->setSpacing(kLineSpacing);

    if (!prefix.isEmpty())
        addLabel(prefix, "rowHint", line);
    auto* image = new ArtWidget(art, artSize, this);
    image->setOpacity(m_dimmed ? Theme::DimmedOpacity : 1.0);
    line->addWidget(image);
    addLabel(text, "rowText", line);
    line->addStretch();

    m_texts->addLayout(line);
}

void ListRow::setAction(QWidget* action)
{
    m_layout->addWidget(action);
}

QLabel* ListRow::addLabel(const QString& text, const char* objectName, QLayout* layout)
{
    auto* label = new QLabel(text, this);
    label->setObjectName(QLatin1StringView(objectName));
    label->setProperty("dimmed", m_dimmed);
    label->setWordWrap(true);
    layout->addWidget(label);
    return label;
}
