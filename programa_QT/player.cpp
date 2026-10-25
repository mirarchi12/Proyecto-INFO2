#include "player.h"
#include <QSoundEffect>
#include <QUrl>
#include <QDir>
#include <QDebug>
#include <QFile>
#include <QtMath>
#include <QStringList>
#include <algorithm>
#include <QRegularExpression>

Player::Player(QObject *parent) : QObject{parent}
{

    QDir LibSounds("C:/Users/Noxi-PC/Desktop/Proyecto_INFO/programa_QT/Sounds");

    QStringList Sounds = LibSounds.entryList({"*.wav"}, QDir::Files);

    std::sort(Sounds.begin(), Sounds.end(),
        [](const QString &a, const QString &b) {

        int numA = a.section('.', 0, 0).remove(QRegularExpression("[^1-8]")).toInt();
        int numB = b.section('.', 0, 0).remove(QRegularExpression("[^1-8]")).toInt();
        return numA < numB; });

    for (int i=0; i< 8; i++) {
        QSoundEffect *player = new QSoundEffect(this);

        player->setSource(QUrl::fromLocalFile(LibSounds.filePath(Sounds[i])));
        players.append(player);
    }
}


void Player::playSound(int PiezoVal, int i) {

    qreal sens = qBound(0.0, PiezoVal/4095.0, 1.0);
    qreal volume = qPow(sens, 0.5);

    players[i]->setVolume(volume);
        players[i]->play();

}

