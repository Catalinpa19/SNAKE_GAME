#pragma once

#include <QWidget>

class TitleWidget : public QWidget
{
public:
    explicit TitleWidget(QWidget* parent = nullptr);

protected:
    void paintEvent(QPaintEvent* event) override;
};
