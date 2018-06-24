#ifndef TRANSFER_H
#define TRANSFER_H

#include <QObject>

class Transfer : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString filename READ filename NOTIFY filenameChanged)
    Q_PROPERTY(QString filePath READ filePath NOTIFY filePathChanged)
    Q_PROPERTY(qint32 total READ total NOTIFY totalChanged)
    Q_PROPERTY(qint32 transferred READ transferred NOTIFY transferredChanged)
    Q_PROPERTY(bool completed READ completed NOTIFY completedChanged)
    Q_PROPERTY(bool success READ success NOTIFY completedChanged)

public:
    explicit Transfer(const QString &path, const QString &filePath, const QString &filename, QObject *parent = 0);

    QString path() const;
    void move(const QString &newFilePath);

    QString filename() const;
    QString filePath() const;

    void setStarted();
    void setCompleted(bool success);

    qint32 total() const;
    qint32 transferred() const;

    bool completed() const;
    bool success() const;

signals:
    void filenameChanged();
    void filePathChanged();
    void totalChanged();
    void transferredChanged();
    void completedChanged();

private slots:
    void progress(qint32 total, qint32 transferred);

private:
    QString m_path;
    QString m_filePath;
    QString m_filename;
    qint32 m_total;
    qint32 m_transferred;
    bool m_completed;
    bool m_success;
};

#endif // TRANSFER_H
