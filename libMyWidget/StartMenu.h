#pragma once

#include <QWidget>
#include <QPushButton>
#include <QMovie>
#include <QPainter>

enum StartMenuState{
    SMS_start,
    SMS_choose_mode,
    SMS_hide
};

class StartMenu: public QWidget
{
    Q_OBJECT
public:
    StartMenu(QWidget* parent);
    void draw(QPainter *painter);
    void update();
    void hide();
signals:
    void signalSingleMode();
    void signalDoubleMode();
    void signalExit();
    void signalLoadGame();

private:
    StartMenuState state;
    int x, y;
    QMovie* movie;
    QPushButton singleButton, doubleButton, loadButton;
    QPushButton startButton, exitButton;
};