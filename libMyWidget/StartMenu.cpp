#include "StartMenu.h"

#include <QFont>
#include <QImage>
#include <QLinearGradient>
#include <QPainterPath>
#include <QPen>

#include "MyWidgetResList.h"

StartMenu::StartMenu(QWidget *parent)
    : QWidget(nullptr), x(0), y(0), state(SMS_start),
      startButton(parent),
      exitButton(parent),
      singleButton(parent),
      doubleButton(parent),
      loadButton(parent),
      inputM(parent),
      inputN(parent),
      inputTime(parent),
      okButton(parent)
{
    movie = new QMovie(START_MENU_GIF);
    movie->start();

    // startButton
    startButton.setGeometry(x + 375, y + 389, 231, 111);
    startButton.setStyleSheet(
        "QPushButton {"
        "border-radius: 5px;"
        "background-color: transparent;"
        "background-image: url(../res/images/UI/start_button.png);" /* 将按钮背景设为图片 */
        "background-position: center;"
        "background-repeat: no-repeat;"
        "}"
        "QPushButton:pressed {"
        "background-color: rgba(0, 0, 0, 0.1);" /* 按下时添加半透明效果 */
        "}");
    startButton.hide();
    connect(&startButton, &QPushButton::clicked, this, [=]() { state = SMS_choose_mode; });

    // exit button
    exitButton.setGeometry(x + 375, y + 510, 231, 111);
    exitButton.setStyleSheet(
        "QPushButton {"
        "border-radius: 5px;"
        "background-color: transparent;"
        "background-image: url(../res/images/UI/exit_button.png);" /* 将按钮背景设为图片 */
        "background-position: center;"
        "background-repeat: no-repeat;"
        "}"
        "QPushButton:pressed {"
        "background-color: rgba(0, 0, 0, 0.1);" /* 按下时添加半透明效果 */
        "}");
    exitButton.hide();
    connect(&exitButton, &QPushButton::clicked, this, [=]() { emit signalExit(); });

    // single button
    singleButton.setGeometry(x + 375, y + 370, 202, 99);
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
    connect(&singleButton, &QPushButton::clicked, this, [=]() {
        gamemode = 1;
        state = SMS_input;
    });

    // double button
    doubleButton.setGeometry(x + 375, y + 485, 202, 99);
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
    connect(&doubleButton, &QPushButton::clicked, this, [=]() {
        gamemode = 2;
        state = SMS_input;
    });

    // load button
    loadButton.setGeometry(x + 375, y + 600, 202, 99);
    loadButton.setStyleSheet(
        "QPushButton {"
        "border-radius: 5px;"
        "background-color: transparent;"
        "background-image: url(../res/images/UI/sq_load_button.png);" /* 将按钮背景设为图片 */
        "background-position: center;"
        "background-repeat: no-repeat;"
        "}"
        "QPushButton:pressed {"
        "background-color: rgba(0, 0, 0, 0.1);" /* 按下时添加半透明效果 */
        "}");
    loadButton.hide();
    connect(&loadButton, &QPushButton::clicked, this, [=]() { emit signalLoadGame(); });

    // input mode
    int fontId = QFontDatabase::addApplicationFont(PIXEL_FONT);
    QString fontFamily = QFontDatabase::applicationFontFamilies(fontId).at(0);
    // inputM
    labelM = new QLabel("M:", parent);
    labelM->setStyleSheet("font-family: '" + fontFamily +
                          "';"
                          "color: #22060b;"
                          "font-size: 35px;");
    labelM->setGeometry(x + 375, y + 400, 49, 38);
    inputM.setGeometry(x + 375 + 60, y + 400, 104, 36);
    labelM->hide();
    inputM.hide();
    // inputN
    labelN = new QLabel("N:", parent);
    labelN->setStyleSheet("font-family: '" + fontFamily +
                          "';"
                          "color: #22060b;"
                          "font-size: 35px;");
    labelN->setGeometry(x + 375, y + 450, 49, 38);
    inputN.setGeometry(x + 375 + 60, y + 450, 104, 36);
    labelN->hide();
    inputN.hide();
    // inputTime
    labelTime = new QLabel("Time:", parent);
    labelTime->setStyleSheet("font-family: '" + fontFamily +
                             "';"
                             "color: #22060b;"
                             "font-size: 30px;");
    labelTime->setGeometry(x + 285, y + 500, 139, 38);
    inputTime.setGeometry(x + 375 + 60, y + 500, 104, 36);
    labelTime->hide();
    inputTime.hide();
    // ok button
    okButton.setGeometry(x + 290, y + 550, 250, 47);
    okButton.setStyleSheet(
        "QPushButton {"
        "border-radius: 8px;"
        "background-color: transparent;"
        "background-image: url(../res/images/UI/ok_button.png);" /* 将按钮背景设为图片 */
        "background-position: center;"
        "background-repeat: no-repeat;"
        "}"
        "QPushButton:pressed {"
        "background-color: rgba(0, 0, 0, 0.1);" /* 按下时添加半透明效果 */
        "}");
    okButton.hide();
    connect(&okButton, &QPushButton::clicked, this, newGame);
}

void StartMenu::draw(QPainter *painter)
{
    QImage currentFrame = movie->currentImage();
    painter->drawImage(x, y, currentFrame);

    // 创建文本路径
    QFont font;
    int fontId = QFontDatabase::addApplicationFont(PIXEL_FONT);
    QString fontFamily = QFontDatabase::applicationFontFamilies(fontId).at(0);
    font.setFamily(fontFamily);
    font.setPixelSize(65);

    QPainterPath textPath;
    QString text = "QLINK";
    textPath.addText(300, 350, font, text);  // 起始位置 x: 10, y: 100

    // 创建线性渐变，从上到下的黄色渐变
    QLinearGradient gradient(0, 0, 0, 100);
    gradient.setColorAt(0.0, QColor(247, 234, 174));  // 浅黄色
    gradient.setColorAt(1.0, QColor(221, 148, 95));   // 深黄色

    painter->setBrush(gradient);
    painter->setPen(QPen(Qt::black, 4));
    painter->drawPath(textPath);
}

void StartMenu::update()
{
    switch (state) {
    case SMS_start:
        startButton.show();
        exitButton.show();
        singleButton.hide();
        doubleButton.hide();
        loadButton.hide();
        labelM->hide();
        inputM.hide();
        labelN->hide();
        inputN.hide();
        labelTime->hide();
        inputTime.hide();
        okButton.hide();
        break;
    case SMS_choose_mode:
        startButton.hide();
        exitButton.hide();
        singleButton.show();
        doubleButton.show();
        loadButton.show();
        labelM->hide();
        inputM.hide();
        labelN->hide();
        inputN.hide();
        labelTime->hide();
        inputTime.hide();
        okButton.hide();
        break;
    case SMS_input:
        startButton.hide();
        exitButton.hide();
        singleButton.hide();
        doubleButton.hide();
        loadButton.hide();
        labelM->show();
        inputM.show();
        labelN->show();
        inputN.show();
        labelTime->show();
        inputTime.show();
        okButton.show();
        break;
    }
}

void StartMenu::hide()
{
    startButton.hide();
    exitButton.hide();
    singleButton.hide();
    doubleButton.hide();
    loadButton.hide();
    labelM->hide();
    inputM.hide();
    labelN->hide();
    inputN.hide();
    labelTime->hide();
    inputTime.hide();
    okButton.hide();
    state = SMS_hide;
}

void StartMenu::show()
{
    state = SMS_start;
}

void StartMenu::newGame()
{
    bool okM, okN, okTime;

    // 获取并转换输入的数字
    int M = inputM.text().toInt(&okM);
    int N = inputN.text().toInt(&okN);
    int time = inputTime.text().toInt(&okTime);

    // 检查所有输入是否有效
    if (okM && okN && okTime) {
        if (M > 10 || N > 10) {
            QMessageBox::warning(this, "错误", "M*N 最大为 10*10");
            return;
        }
        if (M < 3 || N < 3) {
            QMessageBox::warning(this, "错误", "M*N 最小为 3*3");
            return;
        }
        if (time < 5) {
            QMessageBox::warning(this, "错误", "time 最小为 5s");
            return;
        }
    } else {
        QMessageBox::warning(this, "错误", "请确保所有输入都是有效的数字！");
        return;
    }

    emit signalNewGame(gamemode, M, N, time);
}

void StartMenu::keyReleaseEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape) {
        switch (state) {
        case SMS_choose_mode:
            state = SMS_start;
            break;
        case SMS_input:
            state = SMS_choose_mode;
            break;
        }
    }
}