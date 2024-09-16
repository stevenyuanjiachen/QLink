#pragma once

#include <QLabel>
#include <QPainter>
#include <QPixmap>
#include <QPushButton>

class FinishMenu : public QWidget {
    Q_OBJECT
public:
    FinishMenu(QWidget *parent, int x, int y);
    void draw(QPainter *painter);
    void hide();
    void setLabel(int gamemode, int score);
    
signals:
    void signalQuitGame();

private:
    int gamemode;
    int x, y;
    QLabel label;
    QPixmap pixmap;
    QPushButton quitButton;
};