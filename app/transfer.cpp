#include "transfer.h"

#include <QDBusInterface>
#include <QDebug>
#include <QFile>
#include <QStandardPaths>
#include <QDBusReply>
#include <QFileInfo>

Transfer::Transfer(const QString &path, const QString &filePath, QObject *parent) :
    QObject(parent),
    m_path(path),
    m_filePath(filePath),
    m_total(0),
    m_transferred(0),
    m_status(Status::StatusQueued)
{

    if (path == "/completed") {
        QFileInfo fi(filePath);
        m_filePath = fi.absolutePath();
        m_filename = fi.fileName();
        m_status = StatusComplete;

    } else {

        m_iface = new QDBusInterface("org.bluez.obex", path, "org.bluez.obex.Transfer", QDBusConnection::sessionBus(), this);

        qDebug() << "name" << fetchProperty("Name");

        m_filename = fetchProperty("Name").toString();
        m_total = fetchProperty("Size").toULongLong();
        m_status = statusStringToStatus(fetchProperty("Status").toString());
        m_transferred = fetchProperty("Transferred").toULongLong();
        qDebug() << "filename" << m_filename;

        qDebug() << "connected signal" << QDBusConnection::sessionBus().connect("org.bluez.obex", path, "org.freedesktop.DBus.Properties", "PropertiesChanged", "sa{sv}as", this, SLOT(propertiesChanged(QString, QVariantMap, QStringList)));
    }

}

QString Transfer::path() const
{
    return m_path;
}

void Transfer::move(const QString &newFilePath)
{
    QFile f;
    f.setFileName(m_filePath + m_filename);
    if (!f.copy(newFilePath + "/" + m_filename)) {
        qWarning() << "Copy file failed. Destination:" << newFilePath + "/" + m_filename;
        return;
    }

    f.remove();
    m_filePath = newFilePath;
    emit filePathChanged();
}

QString Transfer::filename() const
{
    return m_filename;
}

QString Transfer::filePath() const
{
    return m_filePath;
}

quint64 Transfer::total() const
{
    return m_total;
}

quint64 Transfer::transferred() const
{
    return m_transferred;
}

Transfer::Status Transfer::status() const
{
    return m_status;
}

QVariant Transfer::fetchProperty(const QString &propertyName) const
{

    QDBusInterface ff("org.bluez.obex", m_path, "org.freedesktop.DBus.Properties");
    QDBusMessage m = ff.call("Get", "org.bluez.obex.Transfer1", propertyName);
    if (m.type() != QDBusMessage::ErrorMessage && m.arguments().count() == 1) {
        QDBusVariant dbv = qdbus_cast<QDBusVariant>(m.arguments().first());
        qDebug() << "property" << propertyName << dbv.variant();

        return dbv.variant();

    }
    qDebug() << "error getting property:" << propertyName << m.errorMessage();
    return QVariant();
}

Transfer::Status Transfer::statusStringToStatus(const QString &statusString)
{
    if (statusString == "queued") {
        return StatusQueued;
    }
    if (statusString == "active") {
        return StatusActive;
    }
    if (statusString == "suspended") {
        return StatusSuspended;
    }
    if (statusString == "complete") {
        return StatusComplete;
    }
    if (statusString == "error") {
        return StatusError;
    }
    return StatusError;
}

void Transfer::propertiesChanged(const QString &interface, const QVariantMap &changedProperties, const QStringList &invalidatedProperties)
{
    qDebug() << "properties changed" << interface << changedProperties << invalidatedProperties;
    if (interface != "org.bluez.obex.Transfer1") {
        return;
    }
    if (changedProperties.contains("Transferred")) {
        m_transferred = changedProperties.value("Transferred").toULongLong();
        emit transferredChanged();
    }
    if (changedProperties.contains("Status")) {
        m_status = statusStringToStatus(changedProperties.value("Status").toString());
        emit statusChanged();
    }
}
