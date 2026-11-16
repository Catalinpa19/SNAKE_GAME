#pragma once

#include <QString>

// 999 -> "999", 1000 -> "1k", 1500 -> "1.5k", 1000000 -> "1m", 1000000000 -> "1b". Decimals are truncated.
inline QString formatCount(qint64 value)
{
    struct Unit
    {
        qint64 size;
        QLatin1Char suffix;
    };
    constexpr Unit units[] = {{1'000'000'000, QLatin1Char('b')}, {1'000'000, QLatin1Char('m')}, {1'000, QLatin1Char('k')}};

    for (const Unit& unit : units)
    {
        if (value < unit.size)
            continue;

        const qint64 tenths = value * 10 / unit.size;
        const QString whole = QString::number(tenths / 10);
        return tenths % 10 == 0 ? whole + unit.suffix : whole + QLatin1Char('.') + QString::number(tenths % 10) + unit.suffix;
    }
    return QString::number(value);
}
