#pragma once

#include <QPushButton>
#include <QPainter>
#include <QPixmap>

class PauseMenu : public QWidget
{
    Q_OBJECT
public:
    PauseMenu(QWidget *parent, int x, int y);
    void draw(QPainter *painter);
    void hide();
signals:
    void signalContinue();
    void signalSaveGame();
    void signalLoadGame();
    void signalQuitGame();

private:
    int x, y;
    QPixmap pixmap;
    QPushButton contineButton, saveButton, loadButton, quitButton;
};