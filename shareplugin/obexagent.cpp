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

void ObexAgent::Complete(const QDBusObjectPath &transfer)
{
    qDebug() << "Compelte called" << transfer.path();
}

void ObexAgent::Error(const QDBusObjectPath &transfer, const QString &message)
{
    qDebug() << "Error called" << transfer.path();
}

void ObexAgent::Progress(const QDBusObjectPath &transfer, int transferred)
{
    qDebug() << "Progress called" << transfer.path();
}

void ObexAgent::Release()
{
    qDebug() << "Release called";
}

QString ObexAgent::Request(const QDBusObjectPath &transfer)
{
    qDebug() << "Request called" << transfer.path();
}
