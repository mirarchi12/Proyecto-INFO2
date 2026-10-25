#ifndef LOOPER_H
#define LOOPER_H

#include <QObject>
#include <QElapsedTimer>
#include <QTimer>

class looper : public QObject
{
    Q_OBJECT
public:
    looper(QObject *parent = nullptr);
    void resetSequence();
    void resizeSequence(int);
    void startSequenceClock();
    QElapsedTimer sequenceClock;
public slots:
    void readSequence(QList<int>);
    void playSequence();

signals:
    void signalreadLineSequence(QList<int>);

private:
    QVector<QVector<int>> sequence;
    QVector<qint64> timesRead;
    QElapsedTimer timer;
    QTimer delayPlay;
    int read = 0;
    long double timeSequence;
    qint64 timeSound;
    qint64 cycleStart=0;
    qint64 now;
    qint64 remaining;
    qint64 nextTimeSound;
};
#endif // LOOPER_H
