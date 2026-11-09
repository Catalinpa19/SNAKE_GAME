#pragma once

#include "widgets/ArtImage.h"

#include <QFrame>

class QHBoxLayout;
class QLabel;
class QLayout;
class QVBoxLayout;

// A row of the achievement and shop lists: art on the left, text lines in the middle, an optional action.
class ListRow : public QFrame
{
public:
    ListRow(const ArtSpec& art, const QSize& artSize, bool dimmed, QWidget* parent = nullptr);

    void addTitle(const QString& text);
    void addText(const QString& text);
    void addArtLine(const QString& prefix, const ArtSpec& art, const QSize& artSize, const QString& text);
    void setAction(QWidget* action);

private:
    QLabel* addLabel(const QString& text, const char* objectName, QLayout* layout);

    bool m_dimmed;
    QHBoxLayout* m_layout;
    QVBoxLayout* m_texts;
};
