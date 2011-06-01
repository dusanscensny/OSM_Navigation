#include <QtCore>

#if QT_VERSION < 0x0040500
#error You need Qt 4.5 or newer
#endif

#include "mapzoom.h"
/**
  *Program OSM_Navigation
  *Aplikácia vyvýjaná ako bakalársky projekt v Žilinskej Univerzite na Fakulte Riadenia a Informatiky
  *V tomto projekte bol ako základ použitý vzor z Qt grafických vzorov pod názvom Qt Graphics Dojo,
  *Dojo = japonský pojem vyjadrujúci mieso cesty.
  *
  *
  *V hlavnom progame na na začiatku nainicializovany QTranslator a následne sa spúšta aplikácia.
  *
  */

int main(int argc, char **argv)
{
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
#if defined(Q_WS_X11)
    QApplication::setGraphicsSystem("raster");
#endif

    QApplication app(argc, argv);
    QTranslator appTranslator;
    appTranslator.load(QString("OSM_Nav_") +QLocale::system().name());
    app.installTranslator(&appTranslator);
    app.setApplicationName("OSM_Navigation");

    MapZoom w;
    w.setWindowTitle("OSM_Navigation");
    w.resize(800, 600);
    w.show();

    return app.exec();
}
