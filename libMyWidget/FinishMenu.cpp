#include "FinishMenu.h"

#include <QFontMetrics>

#include "MyWidgetResList.h"

FinishMenu::FinishMenu(QWidget *parent, int x, int y)
    : QWidget(nullptr), x(x), y(y), quitButton(parent), label(parent)
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
    connect(&quitButton, &QPushButton::clicked, this, [=]() { emit signalQuitGame(); });

    // label
    int fontId = QFontDatabase::addApplicationFont(PIXEL_FONT);
    QString fontFamily = QFontDatabase::applicationFontFamilies(fontId).at(0);
    label.setStyleSheet(
        "font-family: '" + fontFamily + "';"
        "color: #22060b;"
        "font-size: 35px;"
    );
    label.hide();
}

void FinishMenu::draw(QPainter *painter)
{
    painter->drawPixmap(x, y, pixmap);
    quitButton.show();
    label.show();
}

void FinishMenu::hide()
{
    quitButton.hide();
    label.hide();
}

void FinishMenu::setLabel(int gamemode, int score)
{
    QFontMetrics fontMetric(label.font());
    QString text;

    // inputM
    if (gamemode == 1) text = "Score: " + QString::number(score);
    if (gamemode == 2) text = "Player" + QString::number(score) + " Win!";

    int width = fontMetric.horizontalAdvance(text) + 10;
    int height = fontMetric.height();
    int labelX = x + (FINISH_MENU_WIDTH - width) / 2;
    int labelY = y + 230;
    label.setAlignment(Qt::AlignCenter);
    label.setText(text);
    label.setGeometry(labelX, labelY, width, height);
}
