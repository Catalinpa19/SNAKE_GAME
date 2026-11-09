#pragma once

#include "widgets/ArtImage.h"

#include <QAbstractButton>
#include <QtSvg/QSvgRenderer>

class OptionButton : public QAbstractButton
{
public:
    OptionButton(const ArtSpec& art, const QSize& cellSize, int artBox, QWidget* parent = nullptr);

    QSize sizeHint() const override;

    void setLocked(bool locked);

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    ArtImage m_art;
    QSvgRenderer m_lockIcon{QStringLiteral(":/icons/lock.svg")};
    QSize m_cellSize;
    int m_artBox;
    bool m_locked = false;
};
