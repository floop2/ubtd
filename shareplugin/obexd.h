#ifndef OBEXD_H
#define OBEXD_H

#include <QAbstractListModel>
#include <QDBusConnection>
#include <QDBusInterface>

#include "transfer.h"

class ObexAgent;

class Obexd : public QAbstractListModel
{
    Q_OBJECT
public:
    enum Roles {
        RoleFilename,
        RoleFilePath,
        RoleSize,
        RoleTransferred,
        RoleCompleted,
        RoleSuccess
    };

    explicit Obexd(QObject *parent = 0);

    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QHash<int, QByteArray> roleNames() const;

signals:

private slots:
    void newTransfer(const QString &path, const QString &filePath, const QString &filename, const QString btAddress, const QString &type, int length);
    void transferStarted(const QDBusObjectPath &transfer);
    void transferCompleted(QDBusObjectPath transfer, bool success);

    void transferProgress();
private:
    ObexAgent *m_agent;
    QDBusConnection m_dbus;
    QDBusInterface m_manager;

    QList<Transfer*> m_transfers;
};

#endif // OBEXD_H
