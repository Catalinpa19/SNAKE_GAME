#pragma once

#include "widgets/SvgButton.h"

// Small icon-only button that floats over every screen (shop, sound).
class HudButton : public SvgButton
{
public:
    explicit HudButton(const QString& iconPath, QWidget* parent = nullptr);

protected:
    void paintEvent(QPaintEvent* event) override;
};
