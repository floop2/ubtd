#ifndef OBEXAGENT_H
#define OBEXAGENT_H

#include <QObject>
#include <QDBusObjectPath>
#include <QDebug>
#include <QDBusConnection>
#include <QDBusAbstractAdaptor>

#define DBUS_ADAPTER_AGENT_PATH "/test/agent"

#include "obexagentadaptor.h"

class ObexAgent : public QObject, protected QDBusContext
{
    Q_OBJECT
public:
    explicit ObexAgent(QObject *parent = 0);

    void accept(const QString & path, const QString &fileName);

signals:
    void authorized(const QString &path);

public slots:
    QString AuthorizePush(const QDBusObjectPath &transfer);
    void Cancel() {}
    void Release() {}

private:
    QDBusConnection m_dbus;
    ObexAgentAdaptor *m_agent;

    QHash<QString, QDBusMessage> m_pendingRequests;
};

#endif // OBEXAGENT_H
