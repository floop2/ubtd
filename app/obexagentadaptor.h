/*
 * This file was generated by qdbusxml2cpp version 0.8
 * Command line was: qdbusxml2cpp -c ObexAgentAdaptor -a obexagentadaptor obexagent.xml
 *
 * qdbusxml2cpp is Copyright (C) 2015 Digia Plc and/or its subsidiary(-ies).
 *
 * This is an auto-generated file.
 * This file may have been hand-edited. Look for HAND-EDIT comments
 * before re-generating it.
 */

#ifndef OBEXAGENTADAPTOR_H_1436821583
#define OBEXAGENTADAPTOR_H_1436821583

#include <QtCore/QObject>
#include <QtDBus/QtDBus>
QT_BEGIN_NAMESPACE
class QByteArray;
template<class T> class QList;
template<class Key, class Value> class QMap;
class QString;
class QStringList;
class QVariant;
QT_END_NAMESPACE

/*
 * Adaptor class for interface org.bluez.obex.Agent1
 */
class ObexAgentAdaptor: public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.bluez.obex.Agent1")
    Q_CLASSINFO("D-Bus Introspection", ""
"  <interface name=\"org.openobex.Agent\">\n"
"    <method name=\"Release\">\n"
"      <annotation value=\"\" name=\"org.freedesktop.DBus.GLib.Async\"/>\n"
"    </method>\n"
"    <method name=\"AuthorizePush\">\n"
"      <annotation value=\"\" name=\"org.freedesktop.DBus.GLib.Async\"/>\n"
"      <arg type=\"o\" name=\"transfer\"/>\n"
"      <arg direction=\"out\" type=\"s\" name=\"path\"/>\n"
"    </method>\n"
"    <method name=\"Cancel\">\n"
"      <annotation value=\"\" name=\"org.freedesktop.DBus.GLib.Async\"/>\n"
"    </method>\n"
"  </interface>\n"
        "")
public:
    ObexAgentAdaptor(QObject *parent);
    virtual ~ObexAgentAdaptor();

public: // PROPERTIES
public Q_SLOTS: // METHODS
    void Release();
    QString AuthorizePush(const QDBusObjectPath &transfer);
    void Cancel();
Q_SIGNALS: // SIGNALS
};

#endif