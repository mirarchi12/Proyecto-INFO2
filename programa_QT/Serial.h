#ifndef SERIAL_H
#define SERIAL_H

#include <QObject>
#include <QSerialPort>


class serialRead : public QObject {
    Q_OBJECT

public:
    explicit serialRead(QObject *parent = nullptr);

    bool setPort(QString port);
    QList<int> getPiezos();
    QList<int> getLoopData();

signals:
    void signalReadyReadPiezos();
    void signalReadyReadLoopData();

private slots:
    void cargarDatos();
private:
    QSerialPort serialPort;
    QStringList Lectura;
};

#endif // SERIAL_H
