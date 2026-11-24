#pragma once

#include <QAbstractButton>
#include <QString>
#include <QWidget>

inline QAbstractButton* findButton(QWidget& parent, const QString& text)
{
    for (auto* button : parent.findChildren<QAbstractButton*>())
    {
        if (button->text() == text)
            return button;
    }
    return nullptr;
}
