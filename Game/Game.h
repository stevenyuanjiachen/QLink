#ifndef GAME_H
#define GAME_H

#include <QWidget>
#include <QIcon>
#include <QPainter>
#include <QTimer>

class QPainter;

class Game : public QWidget
{
    Q_OBJECT
public:
    explicit Game(QWidget* parent = nullptr);
    void run();
    void init_game(int w, int h, const QString& title,const QIcon& = QIcon());
    void clean_game();
    void update_game();
    void draw_game(QPainter* painter);
protected:
    void paintEvent(QPaintEvent* event) override;
};

#endif