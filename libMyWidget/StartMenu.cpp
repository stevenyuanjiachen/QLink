#include "StartMenu.h"
#include "MyWidgetResList.h"
#include <QImage>
#include <QPen>
#include <QLinearGradient>
#include <QPainterPath>

StartMenu::StartMenu(QWidget *parent) : QWidget(nullptr), x(0), y(0),
                                        singleButton(parent), doubleButton(parent)
{
    movie = new QMovie(START_MENU_GIF);
    movie->start();

    // single button
    singleButton.setGeometry(x + 375, y + 389, 231, 111);
    singleButton.setStyleSheet(
        "QPushButton {"
        "border-radius: 5px;"
        "background-color: transparent;"
        "background-image: url(../res/images/UI/single_mode_button.png);" /* 将按钮背景设为图片 */
        "background-position: center;"
        "background-repeat: no-repeat;"
        "}"
        "QPushButton:pressed {"
        "background-color: rgba(0, 0, 0, 0.1);" /* 按下时添加半透明效果 */
        "}");
    singleButton.hide();
    connect(&singleButton, &QPushButton::clicked, this, [=]()
            { emit signalSingleMode(); });

    // double button
    doubleButton.setGeometry(x + 375, y + 510, 231, 111);
    doubleButton.setStyleSheet(
        "QPushButton {"
        "border-radius: 5px;"
        "background-color: transparent;"
        "background-image: url(../res/images/UI/double_mode_button.png);" /* 将按钮背景设为图片 */
        "background-position: center;"
        "background-repeat: no-repeat;"
        "}"
        "QPushButton:pressed {"
        "background-color: rgba(0, 0, 0, 0.1);" /* 按下时添加半透明效果 */
        "}");
    doubleButton.hide();
    connect(&doubleButton, &QPushButton::clicked, this, [=]()
            { emit signalDoubleMode(); });
}

void StartMenu::draw(QPainter *painter)
{
    QImage currentFrame = movie->currentImage();
    painter->drawImage(x, y, currentFrame);

    singleButton.show();
    doubleButton.show();

    // 创建文本路径
    QFont font;
    int fontId = QFontDatabase::addApplicationFont(PIXEL_FONT);
    QString fontFamily = QFontDatabase::applicationFontFamilies(fontId).at(0);
    font.setFamily(fontFamily);
    font.setPixelSize(65);

    QPainterPath textPath;
    QString text = "QLINK";
    textPath.addText(300, 350, font, text); // 起始位置 x: 10, y: 100

    // 创建线性渐变，从上到下的黄色渐变
    QLinearGradient gradient(0, 0, 0, 100);
    gradient.setColorAt(0.0, QColor(247, 234, 174)); // 浅黄色
    gradient.setColorAt(1.0, QColor(221, 148, 95));  // 深黄色

    painter->setBrush(gradient);
    painter->setPen(QPen(Qt::black, 4));
    painter->drawPath(textPath);
}

void StartMenu::hide()
{
    singleButton.hide();
    doubleButton.hide();
}
