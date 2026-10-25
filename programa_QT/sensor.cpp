#include "sensor.h"
#include "looper.h"
#include <QDebug>

Sensor::Sensor(looper& loop, QObject *parent)  :  loop(loop), QObject{parent}
{}

void Sensor::sensorPiezo(QList<int> piezoVal) {
    for (int i=0; i<piezoVal.size(); i++) {
        if (piezoVal[i] > 150){
            ceroState[i] =0;

            if(!piezoState[i]){
            emit playSound(piezoVal[i], i);
            piezoState[i] = true;
            }
        }
        else {
            ceroState[i]++;
        }

        if(ceroState[i] >= 20){
            piezoState[i] = false;
            ceroState[i] = 0;
            }
        }
    }

void Sensor::setLooping(QList<int> loopData) {
    if(loopData[0]){
        if (!looping) {
            loop.resetSequence();
        }
        looping = true;
        emit signalLooping(loopData[1]);
    }
    else {
        if (looping){
            looping = false;
            emit signalResizeSequence(loopData[1]);
            emit signalPlaySequence();
        }
    }
}
