#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickView>
#include <QQmlContext>

#include "adapter.h"
#include "obexd.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    app.setApplicationName("ubtd.mzanetti");

    Adapter adapter;
//    BluezClient client;
    Obexd obexd;

    QQuickView view;

    view.rootContext()->setContextProperty("obexd", &obexd);
    qmlRegisterUncreatableType<Transfer>("ubtd", 1, 0, "Transfer", "get them from obexd");

    view.setSource(QUrl(QStringLiteral("qrc:///Main.qml")));
    view.setResizeMode(QQuickView::SizeRootObjectToView);
    view.show();
    return app.exec();
}

