#pragma once

#include "widgets/SvgButton.h"

class MenuButton : public SvgButton
{
public:
    explicit MenuButton(const QString& text, const QString& iconPath = {}, QWidget* parent = nullptr);

    QSize sizeHint() const override;

protected:
    void paintEvent(QPaintEvent* event) override;
};
