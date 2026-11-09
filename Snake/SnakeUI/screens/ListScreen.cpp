#include "screens/ListScreen.h"

#include "app/Theme.h"
#include "widgets/ArtWidget.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QScrollArea>
#include <QVBoxLayout>

namespace
{
constexpr int kListHeight = 456;
constexpr int kRowSpacing = 8;
constexpr int kHeaderSpacing = 10;
constexpr int kScrollBarGap = 4;
} // namespace

ListScreen::ListScreen(const ArtSpec& headerArt, const QSize& headerArtSize, QWidget* parent)
    : PanelScreen(Theme::SettingsWidth, parent)
{
    auto* header = new QWidget(this);
    auto* headerLayout = new QHBoxLayout(header);
    headerLayout->setContentsMargins(0, 0, 0, 0);
    headerLayout->setSpacing(kHeaderSpacing);
    m_headerText = new QLabel(header);
    m_headerText->setObjectName(QStringLiteral("panelHeader"));
    headerLayout->addStretch();
    headerLayout->addWidget(new ArtWidget(headerArt, headerArtSize, header));
    headerLayout->addWidget(m_headerText);
    headerLayout->addStretch();
    addSection(header);

    auto* content = new QWidget;
    content->setObjectName(QStringLiteral("listContent"));
    m_rows = new QVBoxLayout(content);
    m_rows->setContentsMargins(0, 0, kScrollBarGap, 0);
    m_rows->setSpacing(kRowSpacing);

    auto* scroll = new QScrollArea(this);
    scroll->setWidgetResizable(true);
    scroll->setFrameShape(QFrame::NoFrame);
    scroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scroll->setFixedHeight(kListHeight);
    scroll->setWidget(content);
    addSection(scroll);

    addBackButton();
}

void ListScreen::setHeaderText(const QString& text)
{
    m_headerText->setText(text);
}

void ListScreen::clearRows()
{
    while (QLayoutItem* item = m_rows->takeAt(0))
    {
        delete item->widget();
        delete item;
    }
}

void ListScreen::addRow(QWidget* row)
{
    m_rows->addWidget(row);
}

void ListScreen::endRows()
{
    m_rows->addStretch();
}

QWidget* ListScreen::rowsParent() const
{
    return m_rows->parentWidget();
}
