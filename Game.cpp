#include "Game.h"
#include <QRandomGenerator>
#include "EnemyPlane.h"

// add Entities
PlayerPlane* player;
Map* gameMap;

Game::Game(QWidget* parent)
    : QWidget(parent)
{
    QTimer* timer = new QTimer(this);
    timer->callOnTimeout(this, [=](){
        update_game();
        update();
    });
    timer->start(1000/60);
}

void Game::run()
{
    init_game(480, 700, "PlaneGame");
    this->show();
}

void Game::init_game(
    int w, int h, 
    const QString& title,
    const QIcon& icon)
{
    setFixedSize(w, h);
    setWindowTitle(title);
    if(!icon.isNull())
    {
        setWindowIcon(icon);
    }

    // add the entities
    gameMap = new Map(w, h);
    player = new PlayerPlane(190, 400, "../images/me1.png");
    player->setType(ET_player);
    Mgr->addEntity(player);
}

void Game::draw_game(QPainter *painter)
{
    gameMap->draw(painter);
    Mgr->draw(painter);
}

void Game::update_game()
{
    Mgr->refresh();
    gameMap->update();
    Mgr->update();
    
    // limit the position of player
    if(player->position.x()<0) 
        player->position.setX(0);
    if(player->position.x()>gameMap->width()-player->width()) 
        player->position.setX(gameMap->width()-player->width()); 
    if(player->position.y()<0)  
        player->position.setY(0);
    if(player->position.y()>gameMap->height()-player->height()) 
        player->position.setY(gameMap->height()-player->height());
    
    static int i=0;
    // player fire
    if(i%30==0) player->fire();
    // create enemy
    if(i%60==0) create_enemy();
    i=(i+1)%600;

    collision_detection();    
}

void Game::clean_game()
{
    delete Mgr;
    delete gameMap;
}

void Game::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    draw_game(&painter); 
}

void Game::closeEvent(QCloseEvent *event)
{
    this->clean_game();
}

void Game::keyPressEvent(QKeyEvent* event)
{
    switch (event->key())
    {
    case Qt::Key_W:
        player->velocity.setY(-1);
        break;
    case Qt::Key_A:
        player->velocity.setX(-1); 
        break;
    case Qt::Key_S:
        player->velocity.setY(1);
        break;
    case Qt::Key_D:
        player->velocity.setX(1);
        break;
    }
}

void Game::keyReleaseEvent(QKeyEvent *event)
{
    switch (event->key())
    {
    case Qt::Key_W:
        player->velocity.setY(0);
        break;
    case Qt::Key_A:
        player->velocity.setX(0);
        break;
    case Qt::Key_S:
        player->velocity.setY(0);
        break;
    case Qt::Key_D:
        player->velocity.setX(0);
        break;
    }
}

void Game::create_enemy()
{
    QStringList enemyImages=
    {
        "",
        "../images/enemy1.png",
        "../images/enemy2.png",
        "../images/enemy3_n1.png",
    };
    int type = QRandomGenerator::global()->bounded(100);
    if(0<=type && type<=65) type = 1;
    else if(65<type && type<=80) type = 2;
    else type = 3;

    EnemyPlane* enemy = new EnemyPlane(0,0,enemyImages[type]);
    enemy->setType(ET_enemy);
    Mgr->addEntity(enemy);

    int posx = QRandomGenerator::global()->bounded(480-enemy->width());
    enemy->position.setX(posx);
    enemy->position.setY(-enemy->height());
    enemy->velocity.setY(1);
}

void Game::collision_detection()
{
    for(auto enemy: Mgr->getEntities(ET_enemy))
    {
        EnemyPlane* foo = (EnemyPlane*) enemy;
        for(auto bullet: Mgr->getEntities(ET_bullet))
        {
            Bullet* bar = (Bullet*) bullet;
            if(foo->collider.intersects(bar->collider))
            {
                foo->destroy();
                bar->destroy();
            }
        }

        if(foo->collider.intersects(player->collider))
        {
            foo->destroy();
        }
        
    }
}
