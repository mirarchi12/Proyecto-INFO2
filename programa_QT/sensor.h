#ifndef SENSOR_H
#define SENSOR_H

#include <QObject>
#include <QList>
#include "looper.h"
class Sensor : public QObject
{
    Q_OBJECT
    friend class looper;
public:

    explicit Sensor(looper& loop, QObject *parent = nullptr);

private:
    bool piezoState[8] = {false};
    int ceroState[8] = {0};
    bool looping = false;
    looper& loop;

signals:
    void playSound(int piezoVal, int i);
    void signalLooping(int);
    void signalResizeSequence(int);
    void signalPlaySequence();

public slots:
    void sensorPiezo(const QList<int> piezoVal);
    void setLooping(QList<int>);
};

#endif // SENSOR_H
