#pragma once

#include "Item.h"

class Shuffle: public Item
{
    Q_OBJECT
public:
    Shuffle(int x=0, int y=0);
    ~Shuffle() {};
    void pickUp() override;

signals:
    void signalAddTime(int time);
};