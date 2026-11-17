#pragma once
#include <gmock/gmock.h>
#include "app/Catalog.h"
#include "app/ScreenNavigator.h"
// Mock signal consumers: production widgets do not expose virtual service APIs.
struct MockUiReceiver {
    MOCK_METHOD(void, action, (), ());
    MOCK_METHOD(void, index, (int), ());
    MOCK_METHOD(void, text, (const QString&), ());
    MOCK_METHOD(void, color, (ColorKind, const QString&), ());
    MOCK_METHOD(void, screen, (Screen), ());
};
