#include "app/ScreenNavigator.h"

#include <QGridLayout>
#include <QStackedWidget>

namespace
{
constexpr int kPageMargin = 16;
} // namespace

ScreenNavigator::ScreenNavigator(QStackedWidget* stack, QObject* parent)
    : QObject(parent)
    , m_stack(stack)
{
}

void ScreenNavigator::addScreen(Screen id, QWidget* screen)
{
    auto* page = new QWidget;
    auto* layout = new QGridLayout(page);
    layout->setContentsMargins(kPageMargin, kPageMargin, kPageMargin, kPageMargin);
    layout->addWidget(screen, 0, 0, Qt::AlignCenter);

    m_stack->addWidget(page);
    m_pages[id] = page;
}

void ScreenNavigator::goTo(Screen id)
{
    const auto it = m_pages.find(id);
    if (it == m_pages.end())
        return;

    m_stack->setCurrentWidget(it->second);
    emit screenChanged(id);
}
