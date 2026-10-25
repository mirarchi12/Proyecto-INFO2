#include "looper.h"
#include <QDebug>
#include <QCoreApplication>
#include <QObject>
#include <QTimer>

looper::looper(QObject *parent) : QObject{parent} {
    delayPlay.setSingleShot(true);
    connect(&delayPlay, &QTimer::timeout, this, &looper::playSequence);
    delayPlay.setTimerType(Qt::PreciseTimer);
}

void looper::readSequence(QList<int> PiezoVal) {
    if (PiezoVal.size() != 8) {
        qDebug() << "Error en la lectura de la secuencia";
        exit(1);
    }
    sequence.append(PiezoVal);
    timesRead.append(timer.elapsed());
}


void looper::resetSequence() {
    read=0;
    timeSequence=0;
    sequence.clear();
    timesRead.clear();
    delayPlay.stop();
    timer.start();
}

void looper::resizeSequence(int bpm) {
    double timeCompas = 4 * (60000.0 / bpm);

    if (timesRead.empty() || sequence.empty()){
        return;
    }

    int bars = qRound(timesRead.last() / timeCompas);

    bars = qMax(1, bars);

    timeSequence = (bars) * timeCompas;

    qDebug() << "BPM recibido:" << bpm;
    qDebug() << "Tiempo del compas:" << timeCompas;
    qDebug() << "Ultimo tiempo:" << timesRead.last();
    qDebug() << "Cantidad de compases:" << bars;
    qDebug() << "Limite de recorte:" << (int) timeSequence;

    while (!timesRead.isEmpty() && (timesRead.last() - timesRead.first()) >= timeSequence) {
        timesRead.removeLast();
        sequence.removeLast();
    }

    qDebug() << "Lecturas restantes:" << sequence.size();
    qDebug() << "Ultimo tiempo restante:" << timesRead.last();
}

void looper::playSequence() {

    if (sequence.isEmpty() || sequence.size() != timesRead.size()) {
        delayPlay.stop();
        return;
    }
    timeSound = cycleStart + (timesRead[read] - timesRead[0]);
    now = sequenceClock.elapsed();

    if(timeSound > now) {
        delayPlay.start(static_cast<int>(timeSound - now));
        return;
    }

    emit signalreadLineSequence(sequence[read]);

        if (read+1 < sequence.size()){
            read++;
        }
        else {
            read=0;
            cycleStart += timeSequence;
        }

        nextTimeSound = cycleStart + (timesRead[read] - timesRead[0]);

        remaining = nextTimeSound - sequenceClock.elapsed();

        delayPlay.start(static_cast<int>(qMax(qint64(0), remaining)));
}

void looper::startSequenceClock() {

    if (sequence.isEmpty() || sequence.size() != timesRead.size())
        return;

    delayPlay.stop();

    read = 0;
    cycleStart = 0;

    sequenceClock.start();

    playSequence();
}