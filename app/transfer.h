#ifndef TRANSFER_H
#define TRANSFER_H

#include <QObject>
#include <QDBusInterface>

class Transfer : public QObject
{
    Q_OBJECT
    Q_ENUMS(Status)
    Q_PROPERTY(QString filename READ filename NOTIFY filenameChanged)
    Q_PROPERTY(QString filePath READ filePath NOTIFY filePathChanged)
    Q_PROPERTY(qint32 total READ total NOTIFY totalChanged)
    Q_PROPERTY(qint32 transferred READ transferred NOTIFY transferredChanged)
    Q_PROPERTY(Status status READ status NOTIFY statusChanged)

public:
    enum Status {
        StatusQueued,
        StatusActive,
        StatusSuspended,
        StatusComplete,
        StatusError
    };

    explicit Transfer(const QString &path, const QString &filePath, QObject *parent = 0);

    QString path() const;
    void move(const QString &newFilePath);

    QString filename() const;
    QString filePath() const;

    quint64 total() const;
    quint64 transferred() const;

    Status status() const;

signals:
    void filenameChanged();
    void filePathChanged();
    void totalChanged();
    void transferredChanged();
    void statusChanged();

private:
    QVariant fetchProperty(const QString &propertyName) const;
    static Status statusStringToStatus(const QString &statusString);

private slots:
    void propertiesChanged(const QString &interface, const QVariantMap &changedProperties, const QStringList &invalidatedProperties);

private:
    QDBusInterface *m_iface;
    QString m_path;
    QString m_filePath;
    QString m_filename;
    qint64 m_total;
    qint64 m_transferred;
    Status m_status;
};

#endif // TRANSFER_H
