#include "obexagent.h"

#include <QStandardPaths>

ObexAgent::ObexAgent(QObject *parent) :
    QObject(parent),
    m_dbus(QDBusConnection::sessionBus())
{
    qDebug() << "registering agent";
    m_agent = new ObexAgentAdaptor(this);
    if(!m_dbus.registerObject(DBUS_ADAPTER_AGENT_PATH, this))
        qCritical() << "Couldn't register agent at" << DBUS_ADAPTER_AGENT_PATH;

}

void ObexAgent::accept(const QString &path, const QString &fileName)
{
    if (!m_pendingRequests.contains(path)) {
        return;
    }
    QDBusMessage msg = m_pendingRequests.take(path);
    QDBusConnection::sessionBus().send(msg.createReply(fileName));
}

QString ObexAgent::AuthorizePush(const QDBusObjectPath &transfer)
{
    qDebug() << "authorize called" <<  transfer.path();


    setDelayedReply(true);
    m_pendingRequests[transfer.path()] = message();
    QTimer::singleShot(0, [this, transfer]() { emit authorized(transfer.path());});
    qDebug() << "returning";
    return QString();
}

