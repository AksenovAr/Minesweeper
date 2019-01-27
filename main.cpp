#include "MineSweeperModel.h"

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <qqmlengine.h>
#include <qqmlcontext.h>
#include <qqml.h>
#include <QtQuick/qquickitem.h>
#include <QtQuick/qquickview.h>
#include "MineSweeperModel.h"

//![0]
int main(int argc, char ** argv)
{
    QStringList paths = QCoreApplication::libraryPaths();
    paths.append(".");
    paths.append("imageformats");
    paths.append("platforms");
    paths.append("bearer");
    paths.append("qmltooling");
    QCoreApplication::setLibraryPaths(paths);

    QGuiApplication app(argc, argv);
    app.setWindowIcon(QIcon(":/images/tile3.ico"));

    QQmlApplicationEngine engine;

    qmlRegisterType<MineSweeperModel>("MineSweeperModel", 1, 0, "MineSweeperModel");

    engine.load( QUrl("qrc:view.qml") );
    if (engine.rootObjects().isEmpty())
             return -1;

    QObject *topLevel = engine.rootObjects().value(0);
    QQuickWindow *window = qobject_cast<QQuickWindow *>(topLevel);

    window->show();
    return app.exec();
}

