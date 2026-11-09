#pragma once

#include "app/Catalog.h"
#include "screens/ListScreen.h"

struct ProfileData;

class ShopScreen : public ListScreen
{
    Q_OBJECT

public:
    explicit ShopScreen(const ProfileData& profile, QWidget* parent = nullptr);

    void refresh(const ProfileData& profile);

signals:
    void purchaseRequested(ColorKind kind, const QString& id);
};
