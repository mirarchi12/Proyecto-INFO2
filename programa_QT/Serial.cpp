#include "Serial.h"

#include <QString>
#include <QSerialPort>
#include <QThread>
#include <QTextStream>
#include <QDebug>

serialRead::serialRead(QObject *parent) : QObject{parent} {


    QSerialPort::connect(&serialPort, &QSerialPort::readyRead, this, &serialRead::cargarDatos);
    };

    bool serialRead::setPort(QString port) {

        QTextStream out(stdout), in(stdin);
        serialPort.setPortName(port);
        serialPort.setBaudRate(921600);
        serialPort.setDataBits(QSerialPort::Data8);
        serialPort.setStopBits(QSerialPort::OneStop);
        serialPort.setParity(QSerialPort::NoParity);
        serialPort.setFlowControl(QSerialPort::NoFlowControl);

        if(!serialPort.open(QIODeviceBase::ReadOnly)){
            out << "No se pudo abrir el puerto serie" << Qt::endl;
            return(1);
        }
        QThread::sleep(3);

        serialPort.clear(QSerialPort::AllDirections);
        return(serialPort.open(QIODevice::ReadOnly));
    }

void serialRead::cargarDatos() {
    QString Line;

    //qDebug() << Line;
    while(serialPort.canReadLine()){
    Line = serialPort.readLine().trimmed();
        if (Line.isEmpty())
        continue;
    Lectura = Line.split("-", Qt::KeepEmptyParts);

    if (Lectura.size() == 10) {
        emit signalReadyReadPiezos();
        emit signalReadyReadLoopData();
    }
    else {
        qDebug() << "Cantidad erronea de Datos: " << Lectura.size();
        }
    }
}

QList<int> serialRead::getPiezos() {
    QList<int> piezoVal;
    for (int i=0; i<8; i++) {
        piezoVal.append(Lectura[i].toInt());
    }
    return(piezoVal);
}


QList<int> serialRead::getLoopData() {
    QList<int> loopData;
    for (int i=8; i<10; i++) {
        loopData.append(Lectura[i].toInt());
    }
    return(loopData);
}
