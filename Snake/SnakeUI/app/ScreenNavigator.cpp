#include "app/ScreenNavigator.h"

#include <QGridLayout>
#include <QStackedWidget>

namespace
{
constexpr int kPageMargin = 16;
constexpr int kHudClearance = 72;
} // namespace

ScreenNavigator::ScreenNavigator(QStackedWidget* stack, QObject* parent)
    : QObject(parent)
    , m_stack(stack)
{
}

void ScreenNavigator::addScreen(Screen id, QWidget* screen, bool fillsPage)
{
    auto* page = new QWidget;
    auto* layout = new QGridLayout(page);
    layout->setContentsMargins(kPageMargin, fillsPage ? kHudClearance : kPageMargin, kPageMargin, kPageMargin);
    layout->addWidget(screen, 0, 0, fillsPage ? Qt::Alignment() : Qt::AlignCenter);

    m_stack->addWidget(page);
    m_pages[id] = page;
}

void ScreenNavigator::goTo(Screen id)
{
    const auto it = m_pages.find(id);
    if (it == m_pages.end())
        return;

    m_stack->setCurrentWidget(it->second);
    m_current = id;
    emit screenChanged(id);
}
