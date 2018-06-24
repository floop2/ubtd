#ifndef OBEXAGENT_H
#define OBEXAGENT_H

#include <QObject>
#include <QDBusObjectPath>
#include <QDebug>
#include <QDBusConnection>
#include <QDBusAbstractAdaptor>

#define DBUS_ADAPTER_AGENT_PATH "/test/client/agent"

#include "obexagentadaptor.h"

class ObexAgent : public QObject
{
    Q_OBJECT
public:
    explicit ObexAgent(QObject *parent = 0);

public slots:
    void Complete(const QDBusObjectPath &transfer);
    void Error(const QDBusObjectPath &transfer, const QString &message);
    void Progress(const QDBusObjectPath &transfer, int transferred);
    void Release();
    QString Request(const QDBusObjectPath &transfer);

private:
    QDBusConnection m_dbus;
    ObexAgentAdaptor *m_agent;
};

#endif // OBEXAGENT_H
