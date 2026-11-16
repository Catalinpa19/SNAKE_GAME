#include "widgets/BoardWidget.h"

#include "app/Theme.h"
#include "widgets/GuideWidget.h"

#include <QLabel>
#include <QPainter>
#include <QPainterPath>
#include <QResizeEvent>
#include <QVBoxLayout>

#include <algorithm>

namespace
{
constexpr int kColumns = 17;
constexpr int kRows = 15;
constexpr int kPreferredCell = 30;
constexpr int kMinimumCell = 16;
constexpr int kCornerRadius = 6;
constexpr int kGuideTop = 64;
constexpr int kNoticeBottom = 24;
} // namespace

BoardWidget::BoardWidget(QWidget* parent)
    : QWidget(parent)
    , m_light(Theme::GroundLight)
    , m_dark(Theme::GroundDark)
    , m_guide(new GuideWidget(this))
    , m_overlay(new QVBoxLayout(this))
{
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    auto* notice = new QLabel(tr("Gameplay is still in development"), this);
    notice->setObjectName(QStringLiteral("devNotice"));

    m_overlay->addSpacing(kGuideTop);
    m_overlay->addWidget(m_guide, 0, Qt::AlignHCenter);
    m_overlay->addStretch();
    m_overlay->addWidget(notice, 0, Qt::AlignHCenter);
    m_overlay->addSpacing(kNoticeBottom);
}

QSize BoardWidget::sizeHint() const
{
    return {kColumns * kPreferredCell, kRows * kPreferredCell};
}

QSize BoardWidget::minimumSizeHint() const
{
    return {kColumns * kMinimumCell, kRows * kMinimumCell};
}

void BoardWidget::setColors(const QColor& light, const QColor& dark)
{
    m_light = light;
    m_dark = dark;
    update();
}

void BoardWidget::setGuideVisible(bool visible)
{
    m_guide->setVisible(visible);
}

QRect BoardWidget::boardRect() const
{
    const int cell = std::max(1, std::min(width() / kColumns, height() / kRows));
    QRect board(0, 0, cell * kColumns, cell * kRows);
    board.moveCenter(rect().center());
    return board;
}

void BoardWidget::resizeEvent(QResizeEvent* event)
{
    QWidget::resizeEvent(event);

    const QRect board = boardRect();
    m_overlay->setContentsMargins(board.left(), board.top(), width() - board.right() - 1, height() - board.bottom() - 1);
}

void BoardWidget::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    const QRect board = boardRect();
    const int cell = board.width() / kColumns;

    QPainterPath clip;
    clip.addRoundedRect(board, kCornerRadius, kCornerRadius);
    painter.setClipPath(clip);

    for (int row = 0; row < kRows; ++row)
    {
        for (int col = 0; col < kColumns; ++col)
        {
            painter.fillRect(board.left() + col * cell, board.top() + row * cell, cell, cell,
                             (row + col) % 2 == 0 ? m_light : m_dark);
        }
    }
}
