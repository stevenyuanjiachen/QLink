#include "PauseMenu.h"
#include "MyWidgetResList.h"

PauseMenu::PauseMenu(QWidget *parent, int x, int y) : QWidget(nullptr), x(x), y(y),
                                                      contineButton(parent), saveButton(parent), loadButton(parent), quitButton(parent)
{
    pixmap.load(PAUSE_MENU_IMAGE);

    // continue button
    contineButton.setGeometry(x + 30, y + 139, 170, 170);
    contineButton.setStyleSheet(
        "QPushButton {"
        "    border-radius: 85px;"
        "    background-color: transparent;"
        "    background-image: url(../res/images/UI/continue_button.png);" /* 将按钮背景设为图片 */
        "    background-position: center;"
        "    background-repeat: no-repeat;"
        "}"
        "QPushButton:pressed {"
        "    background-color: rgba(0, 0, 0, 0.1);" /* 按下时添加半透明效果 */
        "}");
    contineButton.hide();
    connect(&contineButton, &QPushButton::clicked, this, [=](){
        emit signalContinue();
    });

    // save button
    saveButton.setGeometry(x + 205, y + 139, 170, 170);
    saveButton.setStyleSheet(
        "QPushButton {"
        "    border-radius: 85px;"
        "    background-color: transparent;"
        "    background-image: url(../res/images/UI/save_button.png);" /* 将按钮背景设为图片 */
        "    background-position: center;"
        "    background-repeat: no-repeat;"
        "}"
        "QPushButton:pressed {"
        "    background-color: rgba(0, 0, 0, 0.1);" /* 按下时添加半透明效果 */
        "}");
    saveButton.hide();
    connect(&saveButton, &QPushButton::clicked, this, [=](){
        emit signalSaveGame();
    });

    // load button
    loadButton.setGeometry(x + 380, y + 139, 170, 170);
    loadButton.setStyleSheet(
        "QPushButton {"
        "    border-radius: 85px;"
        "    background-color: transparent;"
        "    background-image: url(../res/images/UI/load_button.png);" /* 将按钮背景设为图片 */
        "    background-position: center;"
        "    background-repeat: no-repeat;"
        "}"
        "QPushButton:pressed {"
        "    background-color: rgba(0, 0, 0, 0.1);" /* 按下时添加半透明效果 */
        "}");
    loadButton.hide();
    connect(&loadButton, &QPushButton::clicked, this, [=](){
        emit signalLoadGame();
    });

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

void PauseMenu::draw(QPainter *painter)
{
    painter->drawPixmap(x, y, pixmap);
    quitButton.show();
    contineButton.show();
    saveButton.show();
    loadButton.show();
}

void PauseMenu::hide()
{
    quitButton.hide();
    contineButton.hide();
    saveButton.hide();
    loadButton.hide();
}
