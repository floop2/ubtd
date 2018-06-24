#include "adapter.h"

#include <QDebug>
#include <QDBusReply>

Adapter::Adapter(QObject *parent) :
    QObject(parent),
    m_dbus("org.bluez", "/", "org.bluez.Manager", QDBusConnection::systemBus(), this)
{
    init();
}

void Adapter::init()
{
    QDBusMessage reply = m_dbus.call("ListAdapters");
    QDBusArgument arg = reply.arguments().first().value<QDBusArgument>();

    // We only bother with the first adapter
    arg.beginArray();
    QDBusObjectPath path;
    arg >> path;
    arg.endArray();

    qDebug() << "have adapter" << path.path();

    QDBusInterface iface("org.bluez", path.path(), "org.bluez.Adapter", QDBusConnection::systemBus(), this);
    reply = iface.call("SetProperty", "Discoverable", QVariant::fromValue(QDBusVariant(true)));
    qDebug() << "reply" << reply;

}
