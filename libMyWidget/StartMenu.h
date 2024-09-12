#pragma once

#include <QWidget>
#include <QPushButton>
#include <QMovie>
#include <QPainter>

class StartMenu: public QWidget
{
    Q_OBJECT
public:
    StartMenu(QWidget* parent);
    void draw(QPainter *painter);
    void hide();
signals:
    void signalSingleMode();
    void signalDoubleMode();

private:
    int x, y;
    QMovie* movie;
    QPushButton singleButton, doubleButton ;
};