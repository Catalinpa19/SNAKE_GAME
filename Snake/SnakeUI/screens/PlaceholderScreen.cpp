#include "screens/PlaceholderScreen.h"

#include "app/Theme.h"

#include <QLabel>
#include <QVBoxLayout>

PlaceholderScreen::PlaceholderScreen(const QString& title, QWidget* parent)
    : PanelScreen(Theme::PanelWidth, parent)
{
    const auto addLabel = [this](const QString& text, const char* objectName) {
        auto* label = new QLabel(text, this);
        label->setObjectName(QLatin1StringView(objectName));
        label->setAlignment(Qt::AlignCenter);
        panelLayout()->addWidget(label);
    };

    panelLayout()->setAlignment(Qt::AlignCenter);
    panelLayout()->parentWidget()->setMinimumHeight(Theme::HeaderHeight);
    addLabel(title, "panelTitle");
    addLabel(tr("Coming soon..."), "panelText");
    addBackButton();
}
