#include "EnemyPlane.h"

EnemyPlane::EnemyPlane(double x, double y, const QString &image, EnemyType type)
    : Sprite(x,y,image), m_type(type)
{
    if(type==small)
    {
        this->animas[AT_destroy].addAnima("../images/enemy1_down1.png");
        this->animas[AT_destroy].addAnima("../images/enemy1_down2.png");
        this->animas[AT_destroy].addAnima("../images/enemy1_down3.png");
        this->animas[AT_destroy].addAnima("../images/enemy1_down4.png");
    }
    if(type==middle)
    {
        this->animas[AT_destroy].addAnima("../images/enemy1_down1.png");
        this->animas[AT_destroy].addAnima("../images/enemy1_down2.png");
        this->animas[AT_destroy].addAnima("../images/enemy1_down3.png");
        this->animas[AT_destroy].addAnima("../images/enemy1_down4.png");
    }
    if(type==big)
    {
        this->animas[AT_normal].addAnima("../images/enemy3_n2.png");
        this->animas[AT_destroy].addAnima("../images/enemy3_down1.png");
        this->animas[AT_destroy].addAnima("../images/enemy3_down2.png");
        this->animas[AT_destroy].addAnima("../images/enemy3_down3.png");
        this->animas[AT_destroy].addAnima("../images/enemy3_down4.png");
        this->animas[AT_destroy].addAnima("../images/enemy3_down5.png");
        this->animas[AT_destroy].addAnima("../images/enemy3_down6.png");
    }
}

void EnemyPlane::update()
{
    Sprite::update();
    if(this->position.y()>700+5){
        this->state = ES_clean;
    }
}