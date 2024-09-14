#include "MyProgressBar.h"
#include "MyWidgetResList.h"
#include <QFontMetrics>
#include <QFile>
#include <QRegularExpression>

MyProgressBar::MyProgressBar(int x, int y, double time)
    : QObject(nullptr), x(x), y(y), pause(false),
      time(time * 1000), value(time * 1000), timeText("00:00")
{
    // set the frame
    frame = PROGRESSBAR_IMAGE;

    // set the bar
    barX = x + BAR_INTERVAL_LEFT ;
    barY = y + BAR_INTERVAL_UP ;
    barWidth = BAR_INTERVAL_RIGHT - BAR_INTERVAL_LEFT ;
    barHeight = BAR_INTERVAL_DOWN - BAR_INTERVAL_UP ;
    barRect.setRect(barX, barY, barWidth, barHeight);
    barPen.setStyle(Qt::NoPen);
    barBrush.setColor(QColor(237, 216, 91));
    barBrush.setStyle(Qt::SolidPattern);

    // set the text
    textPen.setColor(QColor(81, 47, 54));
    int fontId = QFontDatabase::addApplicationFont(PIXEL_FONT);
    QString fontFamily = QFontDatabase::applicationFontFamilies(fontId).at(0);
    textFont.setFamily(fontFamily);
    textFont.setPixelSize(12);
    
    QFontMetrics fm(textFont);
    int textWidth = fm.horizontalAdvance(timeText);
    int textHeight = fm.height();
    textX = x + BAR_INTERVAL_LEFT + (BAR_INTERVAL_RIGHT - BAR_INTERVAL_LEFT - textWidth) / 2;
    textY = y + BAR_INTERVAL_UP + (BAR_INTERVAL_DOWN - BAR_INTERVAL_UP + textHeight) / 2 + 3;

    // auto update
    timer.callOnTimeout(this, &MyProgressBar::update);
    timer.start(1);
}

void MyProgressBar::draw(QPainter *painter)
{
    painter->setRenderHint(QPainter::Antialiasing, false);
    painter->setRenderHint(QPainter::TextAntialiasing, false);

    // draw the frame
    painter->drawPixmap(x, y, frame);

    // draw the bar
    if (value > 0)
    {
        painter->setPen(barPen);
        painter->setBrush(barBrush);
        painter->drawRoundedRect(barRect, 3, 3);
    }

    // draw the text
    painter->setPen(textPen);
    painter->setFont(textFont);
    painter->drawText(textX, textY, timeText);
}

void MyProgressBar::update()
{
    if(pause == true) return;

    value -= 1;
    if(value<=0)
    {
        emit signalEnd();
        timeText = "00:00";
        return ;
    }

    // update the bar
    double barLength = barWidth * value / time;
    barRect.setRect(barX, barY, barLength, barHeight);

    // update the text
    int totalSeconds = value / 1000;
    int minutes = totalSeconds / 60;
    int seconds = totalSeconds % 60;
    // Format time as mm:ss
    timeText = QString("%1:%2")
                           .arg(minutes, 2, 10, QChar('0'))  // Ensure two digits for minutes
                           .arg(seconds, 2, 10, QChar('0')); // Ensure two digits for seconds
}

void MyProgressBar::addTime(int time)
{
    value += 1000* time;
    if (value>this->time) this->time = value;
}

void MyProgressBar::init(int time)
{
    time = value = time*1000;
}

void MyProgressBar::pauseBar()
{
    pause = true;
}

void MyProgressBar::contineBar()
{
    pause = false;
}

void MyProgressBar::saveState(const QString &filePath)
{
    QFile file(filePath);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "无法打开文件:" << file.errorString();
        return;
    }

    // 读取文件内容
    QString fileContent;
    QTextStream in(&file);
    fileContent = in.readAll();
    file.close();

    QRegularExpression regex;
    QString newLine;
    // time
    regex.setPattern("progressBar\\.time:\\s*\\d+");
    newLine = QString("progressBar.time: %1").arg(time);
    fileContent.replace(regex, newLine);
    // value
    regex.setPattern("progressBar\\.value:\\s*\\d+");
    newLine = QString("progressBar.value: %1").arg(value);
    fileContent.replace(regex, newLine);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "无法写入文件:" << file.errorString();
        return ;
    }

    QTextStream out(&file);
    out << fileContent;
    file.close();
}

void MyProgressBar::loadState(const QString &filePath)
{
    QFile file(filePath);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "无法打开文件:" << file.errorString();
        return; 
    }

    QTextStream in(&file);
    QString fileContent = in.readAll();
    file.close();

    QRegularExpression regex;
    QRegularExpressionMatch match;

    regex.setPattern("progressBar\\.time:\\s*(\\d+)");
    match = regex.match(fileContent);
    if (match.hasMatch()) time = match.captured(1).toInt();
    else qDebug() << "未找到 progressBar.time 行";

    regex.setPattern("progressBar\\.value:\\s*(\\d+)");
    match = regex.match(fileContent);
    if (match.hasMatch()) value = match.captured(1).toInt();
    else qDebug() << "未找到 progressBar.value 行";
}
