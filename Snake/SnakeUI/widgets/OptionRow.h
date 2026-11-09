#pragma once

#include "widgets/ArtImage.h"

#include <QList>
#include <QWidget>

class OptionButton;
class QButtonGroup;
class QHBoxLayout;

struct OptionSpec
{
    ArtSpec art;
    QString tooltip;
    bool locked = false;
    QString lockedHint;
};

// A centered row of exclusive options. With more options than `visibleCount`, the arrows and the
// mouse wheel rotate the visible window around the list in a loop.
class OptionRow : public QWidget
{
    Q_OBJECT

public:
    OptionRow(const QList<OptionSpec>& options, const QSize& cellSize, int artBox, int selectedIndex = 0,
              int visibleCount = 0, QWidget* parent = nullptr);

    void setLocked(int index, bool locked);
    void select(int index);
    void selectRandomUnlocked();

signals:
    void selected(int index);

protected:
    void wheelEvent(QWheelEvent* event) override;

private:
    void rotate(int steps);
    void ensureVisible(int index);
    void refreshVisibleCells();
    int wrapped(int index) const;

    QList<OptionSpec> m_options;
    QList<OptionButton*> m_buttons;
    QButtonGroup* m_group;
    QHBoxLayout* m_cells;
    int m_visibleCount;
    int m_offset = 0;
};
