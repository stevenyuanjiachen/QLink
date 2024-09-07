#pragma once

#include <QString>
#include <QFontDatabase>

const QString PROGRESSBAR_IMAGE = "../res/images/MyProgressBar/progressBar_frame";
const int FRAME_INTERVAL_UP = 11;
const int FRAME_INTERVAL_LEFT = 42;

int fontId = QFontDatabase::addApplicationFont("../res/font/PressStart2P.ttf");
QString fontFamily = QFontDatabase::applicationFontFamilies(fontId).at(0);