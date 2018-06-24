#include "bttransfer.h"
#include "obexagent.h"

#include <QDebug>
#include <QFile>
#include <QBluetoothTransferRequest>
#include <QDBusReply>

BtTransfer::BtTransfer(QObject *parent) :
    QObject(parent),
    reply(0),
    m_progress(0),
    m_finished(false),
    m_error(QBluetoothTransferReply::NoError)
{
}

qreal BtTransfer::progress() const
{
    return m_progress;
}

bool BtTransfer::finished() const
{
    return m_finished;
}

bool BtTransfer::error() const
{
    return m_error != QBluetoothTransferReply::NoError;
}

QString BtTransfer::currentFile() const
{
    if (!reply || m_queue.isEmpty()) {
        return QString();
    }
    return m_queue.first().second;
}

void BtTransfer::sendFile(const QString &address, const QString &fileName)
{
    qDebug() << "Queuing send operation: " << address << fileName;
    m_queue.append(qMakePair<QBluetoothAddress, QString>(QBluetoothAddress(address), fileName));
    processQueue();
}

void BtTransfer::processQueue()
{
    if (reply) {
        qWarning() << "Still busy sending... Not starting another send operation.";
        return;
    }
    if (m_queue.isEmpty()) {
        return;
    }
    QPair<QBluetoothAddress, QString> item = m_queue.first();
    qDebug() << "Starting sending for:" << item.first << item.second;
    QBluetoothTransferRequest request(item.first);
    QFile *file = new QFile(item.second);
    reply = manager.put(request, file);
    connect(reply, SIGNAL(transferProgress(qint64,qint64)), this, SLOT(updateProgress(qint64,qint64)));
    connect(reply, SIGNAL(finished(QBluetoothTransferReply*)), this, SLOT(transferFinished(QBluetoothTransferReply*)));
    connect(reply, SIGNAL(error(QBluetoothTransferReply::TransferError)), this, SLOT(transferError(QBluetoothTransferReply::TransferError)));
}

void BtTransfer::updateProgress(qint64 transferred, qint64 total)
{
    m_progress = ((float)transferred)/((float)total);
    emit progressChanged();
}

void BtTransfer::transferFinished(QBluetoothTransferReply *reply)
{
    reply->deleteLater();
    m_queue.takeFirst();
    this->reply = 0;
    if (m_queue.isEmpty()) {
        m_finished = true;
        emit finishedChanged();
    } else {
        processQueue();
    }
    emit progressChanged();
}

void BtTransfer::transferError(QBluetoothTransferReply::TransferError lastError)
{
    m_error = lastError;
    reply->deleteLater();
    reply = 0;
    emit errorChanged();
    emit progressChanged();
}
