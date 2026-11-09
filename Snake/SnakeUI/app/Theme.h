#pragma once

#include <QColor>
#include <QString>

namespace Theme
{
inline const QColor Gold(0xFB, 0xB9, 0x00);
inline const QColor Cream(0xFF, 0xF3, 0xCF);
inline const QColor Ink(0x2A, 0x1B, 0x0A);
inline const QColor TitleShadow(0x4A, 0x34, 0x18);

inline const QColor GroundLight(0xC9, 0xA2, 0x5F);
inline const QColor GroundDark(0xB9, 0x8F, 0x4C);

inline const QColor Panel(18, 13, 9, 165);
inline const QColor HudHover(52, 40, 28, 210);
inline const QColor HudPressed(30, 22, 14, 230);
inline const QColor Selection(0xC9, 0xA2, 0x5F, 60);
inline const QColor Hover(255, 255, 255, 22);

inline const QColor Button = GroundLight;
inline const QColor ButtonHover(0xDA, 0xB5, 0x74);
inline const QColor ButtonPressed = GroundDark;

constexpr int PanelWidth = 320;
constexpr int SettingsWidth = 360;
constexpr int HeaderHeight = 360;
constexpr int PanelRadius = 8;
constexpr int PanelPadding = 12;
constexpr int ButtonHeight = 48;
constexpr int ButtonRadius = 8;
constexpr int Spacing = 10;
constexpr qreal DimmedOpacity = 0.4;

inline QString styleSheet()
{
    return QStringLiteral(R"(
QFrame#panel {
    background: rgba(18, 13, 9, 165);
    border-radius: 8px;
}
QFrame#divider {
    background: rgba(201, 162, 95, 50);
    min-height: 1px;
    max-height: 1px;
}
QLabel#panelTitle {
    color: #FBB900;
    font-size: 22px;
    font-weight: 600;
}
QLabel#panelText {
    color: #FFF3CF;
    font-size: 14px;
}
QLabel#panelHeader {
    color: #FFF3CF;
    font-size: 24px;
}

QFrame#listRow {
    background: rgba(201, 162, 95, 36);
    border-radius: 6px;
}
QFrame#listRow[dimmed="true"] {
    background: rgba(255, 255, 255, 10);
}
QLabel#rowTitle {
    color: #FBB900;
    font-size: 15px;
    font-weight: 600;
}
QLabel#rowText {
    color: #FFF3CF;
    font-size: 12px;
}
QLabel#rowHint {
    color: #C9A25F;
    font-size: 12px;
}
QLabel#rowTitle[dimmed="true"], QLabel#rowText[dimmed="true"], QLabel#rowHint[dimmed="true"] {
    color: #8C7A5A;
}

QPushButton#buyButton {
    background: #C9A25F;
    color: #2A1B0A;
    border: none;
    border-radius: 6px;
    font-size: 15px;
    font-weight: 600;
}
QPushButton#buyButton:hover {
    background: #DAB574;
}
QPushButton#buyButton:pressed {
    background: #B98F4C;
}
QPushButton#buyButton:disabled {
    background: rgba(255, 255, 255, 18);
    color: #8C7A5A;
}

QScrollArea, QWidget#listContent {
    background: transparent;
}
QScrollBar:vertical {
    background: transparent;
    width: 8px;
    margin: 0;
}
QScrollBar::handle:vertical {
    background: rgba(201, 162, 95, 140);
    border-radius: 4px;
    min-height: 24px;
}
QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {
    height: 0;
}
QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {
    background: transparent;
}
)");
}
} // namespace Theme
