#include <QCoreApplication>
#include <QLocale>
#include <QTranslator>
#include <QDebug>
#include <QTextStream>
#include <QString>
#include <QSerialPort>
#include <QThread>
#include "Serial.h"
#include "sensor.h"
#include "player.h"
#include "looper.h"

#define TAM 8

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QTextStream out(stdout), in(stdin);
    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "programa_QT_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }

    serialRead serialPort;
    Player player;
    looper loop;
    Sensor sensor(loop);

    serialPort.setPort("COM3");

    QObject::connect(&serialPort, &serialRead::signalReadyReadPiezos, &sensor, [&]() { sensor.sensorPiezo(serialPort.getPiezos()); } );
    QObject::connect(&serialPort, &serialRead::signalReadyReadLoopData, &sensor, [&]() { sensor.setLooping(serialPort.getLoopData()); } );
    QObject::connect(&sensor, &Sensor::playSound, &player, &Player::playSound);
    QObject::connect(&sensor, &Sensor::signalLooping, &loop, [&]() { loop.readSequence(serialPort.getPiezos()); } );
    QObject::connect(&sensor, &Sensor::signalResizeSequence, &loop, &looper::resizeSequence);
    QObject::connect(&sensor, &Sensor::signalPlaySequence, &loop, [&]() { loop.startSequenceClock(); });
    QObject::connect(&loop, &looper::signalreadLineSequence, &sensor, &Sensor::sensorPiezo);

    return a.exec();
}