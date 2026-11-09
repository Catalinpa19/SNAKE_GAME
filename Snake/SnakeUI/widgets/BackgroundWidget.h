#pragma once

#include <QPixmap>
#include <QWidget>

class BackgroundWidget : public QWidget
{
public:
    explicit BackgroundWidget(QWidget* parent = nullptr);

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    QPixmap m_image{QStringLiteral(":/art/background.png")};
};
