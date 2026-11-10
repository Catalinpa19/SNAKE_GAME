#pragma once

#include "widgets/ArtImage.h"

#include <QWidget>

class TitleWidget : public QWidget
{
public:
    explicit TitleWidget(QWidget* parent = nullptr);

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    ArtImage m_title{QStringLiteral("title")};
    ArtImage m_subtitle{QStringLiteral("subtitle")};
};
