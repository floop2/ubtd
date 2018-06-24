#include "transfer.h"

#include <QDBusInterface>
#include <QDebug>
#include <QFile>
#include <QStandardPaths>

Transfer::Transfer(const QString &path, const QString &filePath, const QString &filename, QObject *parent) :
    QObject(parent),
    m_path(path),
    m_filePath(filePath),
    m_filename(filename),
    m_total(0),
    m_transferred(0),
    m_completed(false),
    m_success(false)
{
    QDBusConnection::sessionBus().connect("org.openobex", path, "org.openobex.Transfer", "Progress", "ii", this, SLOT(progress(qint32, qint32)));
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

void Transfer::setStarted()
{
    qDebug() << "transfer started";
    // TODO...
}

void Transfer::setCompleted(bool success)
{
    qDebug() << "transfer completed";
    m_completed = true;
    m_success = success;

    emit completedChanged();
}

qint32 Transfer::total() const
{
    return m_total;
}

qint32 Transfer::transferred() const
{
    return m_transferred;
}

bool Transfer::completed() const
{
    return m_completed;
}

bool Transfer::success() const
{
    return m_success;
}

void Transfer::progress(qint32 total, qint32 transferred)
{
    qDebug() << "transfer progress" << transferred << "/" << total;
    if (m_total != total) {
        m_total = total;
        emit totalChanged();
    }
    m_transferred = transferred;
    emit transferredChanged();
}
