#include "obexd.h"

#include "obexagent.h"

#include <QDBusReply>

Obexd::Obexd(QObject *parent) :
    QAbstractListModel(parent),
    m_dbus(QDBusConnection::sessionBus()),
    m_manager("org.openobex", "/", "org.openobex.Manager", m_dbus)
{
    qDebug() << "creating agent on dbus";

    m_agent = new ObexAgent(this);

    qDebug() << "registering agent on obexd-server";

    QDBusReply<void > reply = m_manager.call("RegisterAgent", qVariantFromValue(QDBusObjectPath(DBUS_ADAPTER_AGENT_PATH)));
    if (!reply.isValid())
            qWarning() << "Error registering agent for the default adapter:" << reply.error();


    qDebug() << "done" << reply.error().message() << reply.error().type();

    m_dbus.connect("org.openobex", "/", "org.openobex.Manager", "TransferStarted", "o", this, SLOT(transferStarted(QDBusObjectPath)));
    m_dbus.connect("org.openobex", "/", "org.openobex.Manager", "TransferCompleted", "ob", this, SLOT(transferCompleted(QDBusObjectPath, bool)));
    connect(m_agent, &ObexAgent::authorized, this, &Obexd::newTransfer);

    QDir dir (QStandardPaths::standardLocations(QStandardPaths::DataLocation).first());
    if (!dir.exists()) {
        dir.mkpath(QStandardPaths::standardLocations(QStandardPaths::DataLocation).first());
    }
}

int Obexd::rowCount(const QModelIndex &parent) const
{
    return m_transfers.count();
}

QVariant Obexd::data(const QModelIndex &index, int role) const
{
    qDebug() << "asked for data" << index.row() << role << m_transfers.count();
    switch(role) {
    case RoleFilename:
        return m_transfers.at(index.row())->filename();
    case RoleFilePath:
        return m_transfers.at(index.row())->filePath();
    case RoleSize:
        return m_transfers.at(index.row())->total();
    case RoleTransferred:
        return m_transfers.at(index.row())->transferred();
    case RoleCompleted:
        return m_transfers.at(index.row())->completed();
    case RoleSuccess:
        return m_transfers.at(index.row())->success();
    }
    return QVariant();
}

QHash<int, QByteArray> Obexd::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles.insert(RoleFilename, "filename");
    roles.insert(RoleFilePath, "filePath");
    roles.insert(RoleSize, "size");
    roles.insert(RoleTransferred, "transferred");
    roles.insert(RoleCompleted, "completed");
    roles.insert(RoleSuccess, "success");
    return roles;
}

void Obexd::newTransfer(const QString &path, const QString &filePath, const QString &filename, const QString btAddress, const QString &type, int length)
{
    qDebug() << "new transfer" << path;
    Transfer *t = new Transfer(path, filePath, filename, this);
    connect(t, &Transfer::totalChanged, this, &Obexd::transferProgress);
    connect(t, &Transfer::transferredChanged, this, &Obexd::transferProgress);

    beginInsertRows(QModelIndex(), m_transfers.count(), m_transfers.count());
    m_transfers.append(t);
    endInsertRows();
    qDebug() << "added to model";
}

void Obexd::transferStarted(const QDBusObjectPath &transfer)
{
    qDebug() << "transfer started";
    foreach (Transfer *t, m_transfers) {
        if (t->path() == transfer.path()) {
            t->setStarted();
            return;
        }
    }
}

void Obexd::transferCompleted(QDBusObjectPath transfer, bool success)
{
    foreach (Transfer *t, m_transfers) {
        if (t->path() == transfer.path()) {
            t->setCompleted(success);
            QModelIndex idx = index(m_transfers.indexOf(t));
            t->move(QStandardPaths::standardLocations(QStandardPaths::DataLocation).first());
            emit dataChanged(idx, idx, QVector<int>() << RoleCompleted << RoleSuccess << RoleFilePath);
            return;
        }
    }
}

void Obexd::transferProgress()
{
    Transfer *t = qobject_cast<Transfer*>(sender());
    QModelIndex idx = index(m_transfers.indexOf(t));
    emit dataChanged(idx, idx, QVector<int>() << RoleTransferred << RoleSize);
}
