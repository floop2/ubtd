#ifndef ADAPTER_H
#define ADAPTER_H

#include <QObject>
#include <QDBusConnection>
#include <QDBusInterface>

class Adapter : public QObject
{
    Q_OBJECT
public:
    explicit Adapter(QObject *parent = 0);

signals:

public slots:

private:
    void init();

private:
    QDBusInterface m_dbus;
};

#endif // ADAPTER_H
