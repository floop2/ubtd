#include "obexd.h"

#include "obexagent.h"

#include <QDBusReply>

Obexd::Obexd(QObject *parent) :
    QAbstractListModel(parent),
    m_dbus(QDBusConnection::sessionBus()),
    m_manager("org.bluez.obex", "/org/bluez/obex", "org.bluez.obex.AgentManager1", m_dbus)
{
    qDebug() << "creating agent on dbus";

    m_agent = new ObexAgent(this);

    qDebug() << "registering agent on obexd-server";

    QDBusReply<void > reply = m_manager.call("RegisterAgent", qVariantFromValue(QDBusObjectPath(DBUS_ADAPTER_AGENT_PATH)));
    if (!reply.isValid())
            qWarning() << "Error registering agent for the default adapter:" << reply.error();

    connect(m_agent, &ObexAgent::authorized, this, &Obexd::newTransfer);

    QDir dir(QStandardPaths::writableLocation(QStandardPaths::CacheLocation));
    qDebug() << "have entries:" << dir.entryList() << dir.path();
    foreach (const QFileInfo &fileInfo, dir.entryInfoList(QDir::Files)) {
        Transfer* t = new Transfer("/completed", fileInfo.absoluteFilePath(), this);
        m_transfers.append(t);
    }
}

int Obexd::rowCount(const QModelIndex &parent) const
{
    return m_transfers.count();
}

QVariant Obexd::data(const QModelIndex &index, int role) const
{
//    qDebug() << "asked for data" << index.row() << role << m_transfers.count();
    switch(role) {
    case RoleFilename:
        return m_transfers.at(index.row())->filename();
    case RoleFilePath:
        return m_transfers.at(index.row())->filePath();
    case RoleSize:
        return m_transfers.at(index.row())->total();
    case RoleTransferred:
        return m_transfers.at(index.row())->transferred();
    case RoleStatus:
        return m_transfers.at(index.row())->status();
    case RoleDate:
        return QFileInfo(m_transfers.at(index.row())->filePath() + "/" + m_transfers.at(index.row())->filename()).created();
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
    roles.insert(RoleStatus, "status");
    roles.insert(RoleDate, "date");
    return roles;
}

void Obexd::deleteFile(int index)
{
    beginRemoveRows(QModelIndex(), index, index);
    Transfer* t = m_transfers.takeAt(index);
    QFile f(t->filePath() + "/" + t->filename());
    f.remove();
    endRemoveRows();
}

void Obexd::newTransfer(const QString &path)
{
    qDebug() << "new transfer" << path;

    QString targetPath = QStandardPaths::writableLocation(QStandardPaths::CacheLocation);
    QDir dir(targetPath);
    if (!dir.exists()) {
        dir.mkpath(targetPath);
    }

    Transfer *t = new Transfer(path, targetPath, this);
    connect(t, &Transfer::totalChanged, this, &Obexd::transferProgress);
    connect(t, &Transfer::transferredChanged, this, &Obexd::transferProgress);
    connect(t, &Transfer::statusChanged, this, &Obexd::transferStatusChanged);

    beginInsertRows(QModelIndex(), m_transfers.count(), m_transfers.count());
    m_transfers.append(t);
    endInsertRows();
    qDebug() << "added to model";

    // TODO: This should show pop up a question whether to accept this transfer.
    // Given we're an app and not a service at this point, let's just accept it.
    m_agent->accept(path, targetPath + "/" + t->filename());

}

void Obexd::transferProgress()
{
    Transfer *t = qobject_cast<Transfer*>(sender());
    QModelIndex idx = index(m_transfers.indexOf(t));
    emit dataChanged(idx, idx, QVector<int>() << RoleTransferred << RoleSize);
}

void Obexd::transferStatusChanged()
{
    Transfer *t = qobject_cast<Transfer*>(sender());
    QModelIndex idx = index(m_transfers.indexOf(t));
    emit dataChanged(idx, idx, QVector<int>() << RoleStatus);
}

