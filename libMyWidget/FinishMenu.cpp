#include "FinishMenu.h"
#include "MyWidgetResList.h"

FinishMenu::FinishMenu(QWidget *parent, int x, int y) : 
    QWidget(nullptr), x(x), y(y), quitButton(parent)
{
    pixmap.load(FINISH_MENU_IMAGE);

    // quit button
    quitButton.setGeometry(x + 175, y + 339, 231, 111);
    quitButton.setStyleSheet(
        "QPushButton {"
        "border-radius: 5px;"
        "background-color: transparent;"
        "background-image: url(../res/images/UI/quit_button.png);" /* 将按钮背景设为图片 */
        "background-position: center;"
        "background-repeat: no-repeat;"
        "}"
        "QPushButton:pressed {"
        "background-color: rgba(0, 0, 0, 0.1);" /* 按下时添加半透明效果 */
        "}");
    quitButton.hide();
    connect(&quitButton, &QPushButton::clicked, this, [=](){
        emit signalQuitGame();
    });
}

void FinishMenu::draw(QPainter *painter)
{
    painter->drawPixmap(x, y, pixmap);
    quitButton.show();
}

void FinishMenu::hide()
{
    quitButton.hide();
}
