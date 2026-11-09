#include "screens/PanelScreen.h"

#include "app/Theme.h"
#include "widgets/Divider.h"
#include "widgets/MenuButton.h"

#include <QFrame>
#include <QVBoxLayout>

PanelScreen::PanelScreen(int width, QWidget* parent)
    : QWidget(parent)
    , m_screenLayout(new QVBoxLayout(this))
{
    setFixedWidth(width);
    m_screenLayout->setContentsMargins(0, 0, 0, 0);
    m_screenLayout->setSpacing(Theme::Spacing);

    auto* panel = new QFrame(this);
    panel->setObjectName(QStringLiteral("panel"));
    m_panelLayout = new QVBoxLayout(panel);
    m_panelLayout->setContentsMargins(Theme::PanelPadding, Theme::PanelPadding, Theme::PanelPadding, Theme::PanelPadding);
    m_panelLayout->setSpacing(Theme::Spacing);
    m_screenLayout->addWidget(panel, 1);
}

void PanelScreen::addSection(QWidget* section)
{
    if (m_panelLayout->count() > 0)
        m_panelLayout->addWidget(new Divider(m_panelLayout->parentWidget()));
    m_panelLayout->addWidget(section);
}

MenuButton* PanelScreen::addButton(const QString& text, const QString& iconPath)
{
    auto* button = new MenuButton(text, iconPath, this);
    m_screenLayout->addWidget(button);
    return button;
}

void PanelScreen::addBackButton()
{
    connect(addButton(tr("Back"), QStringLiteral(":/icons/back.svg")), &QAbstractButton::clicked, this,
            &PanelScreen::backRequested);
}
