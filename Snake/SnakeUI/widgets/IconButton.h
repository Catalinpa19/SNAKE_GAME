#pragma once

#include "widgets/SvgButton.h"

// Frameless icon button, used for the arrows of the color carousels.
class IconButton : public SvgButton
{
public:
    IconButton(const QString& iconPath, const QSize& size, QWidget* parent = nullptr);

protected:
    void paintEvent(QPaintEvent* event) override;
};
