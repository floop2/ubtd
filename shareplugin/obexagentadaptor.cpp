/*
 * This file was generated by qdbusxml2cpp version 0.8
 * Command line was: qdbusxml2cpp -c ObexAgentAdaptor -a obexagentadaptor obexagent.xml
 *
 * qdbusxml2cpp is Copyright (C) 2015 Digia Plc and/or its subsidiary(-ies).
 *
 * This is an auto-generated file.
 * Do not edit! All changes made to it will be lost.
 */

#include "obexagentadaptor.h"
#include <QtCore/QMetaObject>
#include <QtCore/QByteArray>
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVariant>

/*
 * Implementation of adaptor class ObexAgentAdaptor
 */

ObexAgentAdaptor::ObexAgentAdaptor(QObject *parent)
    : QDBusAbstractAdaptor(parent)
{
    // constructor
    setAutoRelaySignals(true);
}

ObexAgentAdaptor::~ObexAgentAdaptor()
{
    // destructor
}

void ObexAgentAdaptor::Complete(const QDBusObjectPath &transfer)
{
    // handle method call org.openobex.Agent.Complete
    QMetaObject::invokeMethod(parent(), "Complete", Q_ARG(QDBusObjectPath, transfer));
}

void ObexAgentAdaptor::Error(const QDBusObjectPath &transfer, const QString &message)
{
    // handle method call org.openobex.Agent.Error
    QMetaObject::invokeMethod(parent(), "Error", Q_ARG(QDBusObjectPath, transfer), Q_ARG(QString, message));
}

void ObexAgentAdaptor::Progress(const QDBusObjectPath &transfer, int transferred)
{
    // handle method call org.openobex.Agent.Progress
    QMetaObject::invokeMethod(parent(), "Progress", Q_ARG(QDBusObjectPath, transfer), Q_ARG(int, transferred));
}

void ObexAgentAdaptor::Release()
{
    // handle method call org.openobex.Agent.Release
    QMetaObject::invokeMethod(parent(), "Release");
}

QString ObexAgentAdaptor::Request(const QDBusObjectPath &transfer)
{
    // handle method call org.openobex.Agent.Request
    QString path;
    QMetaObject::invokeMethod(parent(), "Request", Q_RETURN_ARG(QString, path), Q_ARG(QDBusObjectPath, transfer));
    return path;
}

