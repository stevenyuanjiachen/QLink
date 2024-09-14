#pragma once

#include <QWidget>
#include <QPushButton>
#include <QMovie>
#include <QPainter>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>

enum StartMenuState{
    SMS_start,
    SMS_choose_mode,
    SMS_input,
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
    void startGame();
signals:
    void signalExit();
    void signalLoadGame();
    void signalStartGame(int gamemode, int m, int n, int time);

private:
    StartMenuState state;
    int gamemode;
    int x, y;
    QMovie* movie;
    QPushButton singleButton, doubleButton, loadButton;
    QPushButton startButton, exitButton;
    QLabel* labelM, * labelN, * labelTime;
    QLineEdit inputM, inputN, inputTime;
    QPushButton okButton;
};