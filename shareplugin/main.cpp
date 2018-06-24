#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickView>
#include <QQmlContext>

#include "adapter.h"
#include "bttransfer.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    qmlRegisterType<BtTransfer>("Shareplugin", 0, 1, "BtTransfer");

    QQuickView view;
    QObject::connect(view.engine(), &QQmlEngine::quit, &app, &QCoreApplication::quit);

    view.setSource(QUrl(QStringLiteral("qrc:///Main.qml")));
    view.setResizeMode(QQuickView::SizeRootObjectToView);
    view.show();
    return app.exec();
}

