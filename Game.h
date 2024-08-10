#ifndef GAME_H
#define GAME_H

#include <QWidget>
#include <QIcon>
#include <QPainter>
#include <QTimer>
#include <QKeyEvent>

#include "Sprite.h"
#include "PlayerPlane.h"
#include "Map.h"

class QPainter;

class Game : public QWidget
{
    Q_OBJECT
public:
    explicit Game(QWidget* parent = nullptr);
    void run();
public: // 功能函数
    void init_game(int w, int h, const QString& title,const QIcon& = QIcon());
    void update_game();
    void draw_game(QPainter* painter);
    void clean_game();
protected:
    void paintEvent(QPaintEvent* event) override;
    void closeEvent(QCloseEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;
private:
};

#endif