#pragma once

#include "screens/PanelScreen.h"
#include "widgets/ArtImage.h"

class QLabel;
class QVBoxLayout;

// A panel with a header (art and a text) above a scrollable list of rows.
class ListScreen : public PanelScreen
{
public:
    ListScreen(const ArtSpec& headerArt, const QSize& headerArtSize, QWidget* parent = nullptr);

protected:
    void setHeaderText(const QString& text);
    void clearRows();
    void addRow(QWidget* row);
    void endRows();
    QWidget* rowsParent() const;

private:
    QLabel* m_headerText;
    QVBoxLayout* m_rows;
};
