#pragma once

#include "app/Catalog.h"
#include "screens/ListScreen.h"

struct UiState;

class ShopScreen : public ListScreen
{
    Q_OBJECT

public:
    explicit ShopScreen(const UiState& state, QWidget* parent = nullptr);

    void refresh(const UiState& state);

signals:
    void purchaseRequested(ColorKind kind, int color);
};
