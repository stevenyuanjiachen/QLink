#ifndef HERO_H
#define HERO_H

#include "Sprite.h"
#include "Box.h"
#include "Animation.h"
#include "ResourceList.h"
#include <QElapsedTimer>

const int HERO_STATE_NUM = 8;
enum HeroState
{
    HS_stand_up, HS_stand_down, HS_stand_left, HS_stand_right, 
    HS_move_up, HS_move_down, HS_move_left, HS_move_right,
};
enum HeroBlockState
{
    block_none,
    block_up,
    block_down,
    block_left,
    block_right
};

class Hero : public Sprite
{
    Q_OBJECT
public:
    Hero(double x, double y);
    void update() override;
    void draw(QPainter *painter) override;

public:
    void setState(HeroState state) { this->state = state; }
    HeroState getState() const { return this->state; }
    bool intersects(const QRect &coll) { return collider.intersects(coll); }
    void collideEvent();
    void addTriggeredBox(Box *box);
    Box *getTriggeredBox() const { return triggeredBox; }
    void resetTriggeredBox() { triggeredBox = nullptr; }

private:
    HeroState state;
    HeroBlockState blockState;
    QVector<Animation> anima;
    Box *triggeredBox;
    QElapsedTimer triggerTimer;
};

#endif