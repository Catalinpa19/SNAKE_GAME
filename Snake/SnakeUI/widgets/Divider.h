#pragma once

#include <QFrame>

class Divider : public QFrame
{
public:
    explicit Divider(QWidget* parent = nullptr)
        : QFrame(parent)
    {
        setObjectName(QStringLiteral("divider"));
    }
};
