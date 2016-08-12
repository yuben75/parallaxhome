#include <QApplication>
#include "parallaxhome.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    ParallaxHome w;
    w.resize(360, 640);
    w.show();
    return app.exec();
}
