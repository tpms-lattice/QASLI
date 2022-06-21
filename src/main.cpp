#include <QApplication>
#include "mainwindow.h"
#include <Qt3DRender/qt3drender-config.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    #if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
        qputenv("QT3D_RENDERER", "opengl");
    #endif

    MainWindow w;
    w.show();
    
    return a.exec();
}
