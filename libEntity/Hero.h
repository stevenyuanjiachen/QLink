#pragma once

#include "Sprite.h"
#include "Box.h"
#include "Animation.h"
#include "EntityResList.h"
#include <QElapsedTimer>
#include <QSet>

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
enum BuffType
{
    BT_none,
    BT_flash,
};

class Hero : public Sprite
{
    Q_OBJECT
public:
    Hero(double x, double y, int id=1);
    void update() override;
    void draw(QPainter *painter) override;

public:
    void setState(HeroState state) { this->state = state; }
    HeroState getState() const { return this->state; }
    int getID() const { return playerID; }
    void setPosition(int x, int y);
    bool intersects(const QRect &coll) { return collider.intersects(coll); }
    void collideEvent();
    void addTriggeredBox(Box *box);
    Box *getTriggeredBox() const { return triggeredBox; }
    void resetTriggeredBox() { triggeredBox = nullptr; }

    bool haveFlash() { return buffSet.contains(BT_flash); }
    void addBuff(BuffType buff);

    void saveHeroState(const QString& filePath);
    void loadHeroState(const QString& filePath);
private:
    int playerID;
    HeroState state;
    HeroBlockState blockState;
    QSet<BuffType> buffSet;
    QElapsedTimer flashElapsedTimer;
    QVector<Animation> anima;
    Box *triggeredBox;
};
