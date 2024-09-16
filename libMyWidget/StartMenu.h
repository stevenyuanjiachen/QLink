#pragma once

#include <QKeyEvent>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QMovie>
#include <QPainter>
#include <QPushButton>
#include <QWidget>

enum StartMenuState { SMS_start, SMS_choose_mode, SMS_input, SMS_hide };

class StartMenu : public QWidget {
    Q_OBJECT
public:
    StartMenu(QWidget* parent);
    void draw(QPainter* painter);
    void update();
    void hide();
    void show();
    void newGame();

signals:
    void signalExit();
    void signalLoadGame();
    void signalNewGame(int gamemode, int m, int n, int time);

public slots:
    void keyReleaseEvent(QKeyEvent* event) override;

private:
    StartMenuState state;
    int gamemode;
    int x, y;
    QMovie* movie;
    QPushButton singleButton, doubleButton, loadButton;
    QPushButton startButton, exitButton;
    QLabel *labelM, *labelN, *labelTime;
    QLineEdit inputM, inputN, inputTime;
    QPushButton okButton;
};