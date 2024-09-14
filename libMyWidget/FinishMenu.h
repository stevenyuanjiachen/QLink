#pragma once

#include <QPushButton>
#include <QPainter>
#include <QPixmap>

class FinishMenu : public QWidget
{
    Q_OBJECT
public:
    FinishMenu(QWidget *parent, int x, int y);
    void draw(QPainter *painter);
    void hide();
signals:
    void signalQuitGame();

private:
    int x, y;
    QPixmap pixmap;
    QPushButton quitButton;
};