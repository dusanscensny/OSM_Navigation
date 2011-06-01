#ifndef SLIPPYMAP_H
#define SLIPPYMAP_H
//=================
#include <QtCore>
#include <QtGui>
#include <QtNetwork>

#if QT_VERSION < 0x0040500
#error You need Qt 4.5 or newer
#endif

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif


/**
 * @author Dušan Ščensný
 * @brief Táto trieda kommunikeuje s GUI aplikáciou
 *
 */

QPointF tileForCoordinate(qreal lat, qreal lng, int zoom);
qreal longitudeFromTile(qreal tx, int zoom);//Vrati hodnotu zemepisnej dlzky
qreal latitudeFromTile(qreal ty, int zoom);//Vrati honotu zemepisnej sirky

typedef QVector<QPolygon> PolygonVector; //Vektor polygónov

class SlippyMap : public QObject
{
    Q_OBJECT

public:
    int width;
    int height;
    int zoom;
    qreal latitude;
    qreal longitude;
    bool navigate;

    SlippyMap(QObject *parent = 0);

    virtual ~SlippyMap();


    void invalidate();
    void render(QPainter *p, const QRect &rect);
    void pan(const QPoint &delta);
    void setTrack(QVector<PolygonVector> pg);
    void setWpt(QVector<QPoint> wpt);

private slots:
    void handleNetworkData(QNetworkReply *reply);
    void download();

signals:
    void updated(const QRect &rect);

protected:
    QRect tileRect(const QPoint &tp);

private:
    /**
      *Počet tratí načítaných zo súboru
      */
    int numberOfTracks;
    /**
      *jedna trať = pole polygónov
      */
    QVector<QPolygon> oneTrack;
    /**
      *Všetky trate = pole typu QVector<QPolygon> čím zastupuje všetky trate
      */
    QVector<PolygonVector> allTracks;
    /**
      *významné body
      */
    QVector<QPoint> wayPoints;

    QPoint m_offset;
    QRect m_tilesRect;
    QPixmap m_emptyTile;
    QHash<QPoint, QPixmap> m_tilePixmaps;
    QNetworkAccessManager m_manager;
    QUrl m_url;
};

#endif // SLIPPYMAP_H
