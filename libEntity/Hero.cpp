#include "Hero.h"
#include "Map.h"
#include <QFile>
#include <QDebug>
#include <QRegularExpression>

Hero::Hero(double x, double y)
    : Sprite(ET_hero, x, y, STAND_DOWN_LIST[0]),
      state(HS_stand_down), blockState(block_none),
      triggeredBox(nullptr)
{
    this->setType(ET_hero);
    anima.append(Animation(STAND_UP_LIST));
    anima.append(Animation(STAND_DOWN_LIST));
    anima.append(Animation(STAND_LEFT_LIST));
    anima.append(Animation(STAND_RIGHT_LIST));
    anima.append(Animation(MOVE_UP_LIST));
    anima.append(Animation(MOVE_DOWN_LIST));
    anima.append(Animation(MOVE_LEFT_LIST));
    anima.append(Animation(MOVE_RIGHT_LIST));
}

void Hero::update()
{
    Sprite::update();
    anima[state].update();
    // Reset the collider
    collider = QRect(position.x()+3, position.y() + 30, pixmap.width()-6, pixmap.height() - 30);

    // edge block
    if (this->position.y()+30 < MAP_BLOCK_UP)
        blockState = block_up;
    if (this->position.y() + pixmap.height() > MAP_BLOCK_DOWN)
        blockState = block_down;
    if (this->position.x() < MAP_BLOCK_LEFT)
        blockState = block_left;
    if (this->position.x() + pixmap.width() > MAP_BLOCK_RIGHT)
        blockState = block_right;

    // limit the position of the Hero
    switch (blockState)
    {
    case block_up:
        position.setY(position.y() + speed);
        blockState = block_none;
        break;
    case block_down:
        position.setY(position.y() - speed);
        blockState = block_none;
        break;
    case block_left:
        position.setX(position.x() + speed);
        blockState = block_none;
        break;
    case block_right:
        position.setX(position.x() - speed);
        blockState = block_none;
        break;
    }
}

void Hero::draw(QPainter *painter)
{
    anima[state].draw(painter, position);
}

void Hero::setPosition(int x, int y)
{
    position.setX(x);
    position.setY(y);
}

void Hero::collideEvent()
{
    // limit the position
    switch (state)
    {
    case HS_move_up:
        blockState = block_up;
        break;
    case HS_move_down:
        blockState = block_down;
        break;
    case HS_move_left:
        blockState = block_left;
        break;
    case HS_move_right:
        blockState = block_right;
        break;
    }
}

void Hero::addTriggeredBox(Box *box)
{
    if (triggeredBox == nullptr)
    {
        triggeredBox = box;
    }
    else
    {
        throw "Hero have had a triggeredBox";
    }
}

void Hero::addBuff(BuffType buff)
{
    if(!buffSet.contains(buff)) buffSet.insert(buff);
}

void Hero::saveHeroState(const QString &filePath)
{
    QFile file(filePath);

    // 打开文件并读取内容
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "无法打开文件:" << file.errorString();
        return;
    }

    QString fileContent;  // 保存整个文件的内容
    QTextStream in(&file);
    while (!in.atEnd()) {
        fileContent += in.readLine() + "\n";  // 逐行读取并存储到 fileContent
    }
    file.close();  // 关闭文件以便重新写入

    QRegularExpression regex("");
    // 找到 hero.x: 并替换其后的内容
    regex.setPattern("hero\\.x:\\s*\\S+");
    fileContent.replace(regex, "hero.x: " + QString::number(position.x()));
    // 找到 hero.y: 并替换其后的内容
    regex.setPattern("hero\\.y:\\s*\\S+");
    fileContent.replace(regex, "hero.y: " + QString::number(position.y()));
    // 找到 hero.state 并替换其后的内容
    regex.setPattern("hero\\.state:\\s*\\S+");
    fileContent.replace(regex, "hero.state: " + QString::number(int(state)));
    
    // 更新 buff
    if(buffSet.contains(BT_hint)){
        regex.setPattern("hero\\.state:\\s*\\S+");
        fileContent.replace(regex, "hero.state: true");
    } else {
        regex.setPattern("hero\\.state:\\s*\\S+");
        fileContent.replace(regex, "hero.state: false");
    }

    // 更新triggerBox
    if(triggeredBox != nullptr){
        regex.setPattern("hero\\.haveTriggerBox:\\s*\\S+");
        fileContent.replace(regex, "hero.haveTriggerBox: true");

        regex.setPattern("hero\\.triggerBoxR:\\s*\\S+");
        fileContent.replace(regex, "hero.triggerBoxR: " + QString::number(triggeredBox->getR()));
        
        regex.setPattern("hero\\.triggerBoxC:\\s*\\S+");
        fileContent.replace(regex, "hero.triggerBoxC: " + QString::number(triggeredBox->getC()));
    }else{
        regex.setPattern("hero\\.haveTriggerBox:\\s*\\S+");
        fileContent.replace(regex, "hero.haveTriggerBox: false");
    }

    // 重新打开文件以写入更新后的内容
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "无法打开文件:" << file.errorString();
        return;
    }

    QTextStream out(&file);
    out << fileContent;  // 写入修改后的内容
    file.close();  // 关闭文件
}
