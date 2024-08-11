#ifndef ENEMYPLANE_H
#define ENEMYPLANE_H

#include "Sprite.h"

enum EnemyType{
    small, middle, big
};

class EnemyPlane: public Sprite
{
public:
    EnemyPlane(double x, double y, const QString& image, EnemyType type);
    void update() override;
private:
    EnemyType m_type;
};

#endif