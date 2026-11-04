#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_SnakeUI.h"

class SnakeUI : public QMainWindow
{
    Q_OBJECT

public:
    SnakeUI(QWidget *parent = nullptr);
    ~SnakeUI();

private:
    Ui::SnakeUIClass ui;
};

