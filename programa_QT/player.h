#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
#include <QSoundEffect>
#include <QAudioOutput>

class Player : public QObject
{
    Q_OBJECT
public:
    explicit Player(QObject *parent = nullptr);

public slots:
    void playSound(int PiezoVal, int i);
private:
    QVector<QSoundEffect*> players;
};

#endif // PLAYER_H
