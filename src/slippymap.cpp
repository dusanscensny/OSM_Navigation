#include "slippymap.h"
#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include "iostream"


#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// maximum size of the magnifier
// Hint: see above to find why I picked this one :)
//#define MAX_MAGNIFIER 229 //velkost zvacsenej casti
/**
  *Hešovacia funkcia vytvorená pre ukladanie máp do pamäte.
  */
uint qHash(const QPoint& p)
{
    return p.x() * 17 ^ p.y();
}


/**
  *Konštanta vyjadruje veľkosť mapy v pixloch.
  */
const int tdim = 256;
/**
  *Funkcia na výpočet pozície v pixloch na obrazovke.
  *@param zemepisné súradnice a hodnota priblíženia
  *@return hodnota v pixloch
  */
QPointF tileForCoordinate(qreal lat, qreal lng, int zoom)
{
    qreal zn = static_cast<qreal>(1 << zoom);    
    qreal tx = (lng + 180.0) / 360.0;
    qreal ty = (1.0 - log(tan(lat * M_PI / 180.0) +
                          1.0 / cos(lat * M_PI / 180.0)) / M_PI) / 2.0;
    return QPointF(tx * zn, ty * zn);
}

/**
  *Funkcia na výpočet zemepisnej dĺžky z pozície na obrazovke.
  *@param pozícia v pixloch
  *@return zemepisná dĺžka
  */
qreal longitudeFromTile(qreal tx, int zoom)
{
    qreal zn = static_cast<qreal>(1 << zoom);
    qreal lat = tx / zn * 360.0 - 180.0;
    return lat;
}
/**
  *Funkcia na výpočet zemepisnej šírky z pozície na obrazovke.
  *@param pozícia v pixloch
  *@return zemepisná šírka
  */
qreal latitudeFromTile(qreal ty, int zoom)
{
    qreal zn = static_cast<qreal>(1 << zoom);
    qreal n = M_PI - 2 * M_PI * ty / zn;
    qreal lng = 180.0 / M_PI * atan(0.5 * (exp(n) - exp(-n)));
    return lng;
}

/**
  *Konštruktor ktorý vytvára samotnú mapu.
  *@param Premenné potrebné na inicializáciu mapy.
  */
SlippyMap::SlippyMap(QObject *parent):QObject(parent)
            , width(800)
            , height(600)
            , zoom(8)
            , latitude(59.9138204)
            , longitude(10.7387413)            
            , navigate(false)
            , numberOfTracks(0)
{
    m_emptyTile = QPixmap(tdim, tdim);
    m_emptyTile.fill(Qt::lightGray);

    QNetworkDiskCache *cache = new QNetworkDiskCache;

    cache->setCacheDirectory(QDesktopServices::storageLocation
                              (QDesktopServices::CacheLocation));
        printf("%s",QDesktopServices::storageLocation(QDesktopServices::CacheLocation).toAscii().constData());
    m_manager.setCache(cache);
    connect(&m_manager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(handleNetworkData(QNetworkReply*)));    
}



    void SlippyMap::invalidate() {
        if (width <= 0 || height <= 0)
                return;

        QPointF ct = tileForCoordinate(latitude, longitude, zoom);
        qreal tx = ct.x();
        qreal ty = ct.y();


        /* horný ľavý roh stredovej časti obrázka
           
           */
        int xp = width / 2 - (tx - floor(tx)) * tdim;
        int yp = height / 2 - (ty - floor(ty)) * tdim;

        /* prvá časť obrázka vertikálne a horizontálne
           */
        int xa = (xp + tdim - 1) / tdim;
        int ya = (yp + tdim - 1) / tdim;
        int xs = static_cast<int>(tx) - xa;
        int ys = static_cast<int>(ty) - ya;

        /* offset pre ľavý horný roh
           */
        m_offset = QPoint(xp - xa * tdim, yp - ya * tdim);

        // last tile vertical and horizontal
        int xe = static_cast<int>(tx) + (width - xp - 1) / tdim;
        int ye = static_cast<int>(ty) + (height - yp - 1) / tdim;

        // build a rect
        m_tilesRect = QRect(xs, ys, xe - xs + 1, ye - ys + 1);        
        if (m_url.isEmpty())
            download();

        emit updated(QRect(0, 0, width, height));
    }
/**
  *Funkcia, ktorá prekresľuje mapu aj všetky prvky na nej (trasy , body a akuálnu pozíciu).
  *
  */

    void SlippyMap::render(QPainter *p, const QRect &rect) {
        for (int x = -1; x <= m_tilesRect.width()+1; ++x)
            for (int y = -1; y <= m_tilesRect.height()+1; ++y) {
                QPoint tp(x + m_tilesRect.left(), y + m_tilesRect.top());
                QRect box = tileRect(tp);
                if (rect.intersects(box)) {
                    if (m_tilePixmaps.contains(tp))                       
                        p->drawPixmap(box, m_tilePixmaps.value(tp));                        
                    else{                        
                        p->drawPixmap(box, m_emptyTile);
                    }
                }              
            }

        p->brush();
        //vykreslí jednotlivé trasy
        QPen pen;        
        pen.setWidth(3);
        p->setPen(pen);        
        for(int j=0;j<allTracks.size();j++){
            if(j==0)pen.setColor(Qt::darkCyan);
            if(j==1)pen.setColor(Qt::green);
            if(j==2)pen.setColor(Qt::blue);
            if(j==3)pen.setColor(Qt::darkRed);
            if(j==4)pen.setColor(Qt::red);
            if(j==5)pen.setColor(Qt::cyan);
            if(j==6)pen.setColor(Qt::yellow);
            if(j==7)pen.setColor(Qt::magenta);
            oneTrack = allTracks.at(j);            
            p->setPen(pen);
            
            for(int i=0;i<oneTrack.size();i++){
                p->drawPolyline(oneTrack.at(i));
            }
        //vykreslí zaujímavé body
        }
        for(int i=0; i< wayPoints.size();i++){
            p->drawImage(wayPoints.at(i),QImage("img/target.png"));
        }
        //vykreslí pozíciu
        if(navigate){
        p->drawImage(width/2, height/2,QImage("img/poz.png"));
        }
    }
/**
  *Funkcia prepočítava nové hodnoty zemepisných súradníc pri pohybe mapy.
  *@param Hodnota v pixloch o ktorú sa mapa posunula.
  */

    void SlippyMap::pan(const QPoint &delta) {
        QPointF dx = QPointF(delta) / qreal(tdim);
        QPointF center = tileForCoordinate(latitude, longitude, zoom) - dx;
        latitude = latitudeFromTile(center.y(), zoom);
        longitude = longitudeFromTile(center.x(), zoom);
        invalidate();
    }

    void SlippyMap::handleNetworkData(QNetworkReply *reply) {
        QImage img;
        QPoint tp = reply->request().attribute(QNetworkRequest::User).toPoint();
        QUrl url = reply->url();
        if (!reply->error())
            if (!img.load(reply, 0))
                img = QImage();
        reply->deleteLater();
        m_tilePixmaps[tp] = QPixmap::fromImage(img);
        if (img.isNull())
            m_tilePixmaps[tp] = m_emptyTile;
        emit updated(tileRect(tp));

        // purge unused spaces
        QRect bound = m_tilesRect.adjusted(-2, -2, 2, 2);
        foreach(QPoint tp, m_tilePixmaps.keys())
        if (!bound.contains(tp))
            m_tilePixmaps.remove(tp);

        download();
    }

/**
  *Funkcia na sťahovanie máp z url stránky.
  */
    void SlippyMap::download() {
        QPoint grab(0, 0);
        for (int x = 0; x <= m_tilesRect.width(); ++x)
            for (int y = 0; y <= m_tilesRect.height(); ++y) {
                QPoint tp = m_tilesRect.topLeft() + QPoint(x, y);
                if (!m_tilePixmaps.contains(tp)) {
                    grab = tp;
                    break;
                }
            }
        if (grab == QPoint(0, 0)) {
            m_url = QUrl();
            return;
        }

        QString path = "http://tile.openstreetmap.org/%1/%2/%3.png"; //"http://khm1.google.com/kh?v=34&hl=en&x=%1&s=&y=%2&z=%3";//http://a.andy.sandbox.cloudmade.com/tiles/cycle/%1/%2/%3.png";//
        m_url = QUrl(path.arg(zoom).arg(grab.x()).arg(grab.y()));        
        QNetworkRequest request;
        request.setUrl(m_url);
        request.setRawHeader("Dusan", "OSM_Navigaton");
        request.setAttribute(QNetworkRequest::User, QVariant(grab));
        m_manager.get(request);
    }


    //void updated(const QRect &rect);


    QRect SlippyMap::tileRect(const QPoint &tp) {
        QPoint t = tp - m_tilesRect.topLeft();
        int x = t.x() * tdim + m_offset.x();
        int y = t.y() * tdim + m_offset.y();
        return QRect(x, y, tdim, tdim);
    }


/**
  *Nastavenie trate do premennej. Dôležité preto, lebo body sa pri zmenách na mape prepočítavajú.
  *@param Zoznam vštkých trás
  */
    void SlippyMap::setTrack(QVector<PolygonVector> pg){
        allTracks = pg;
        //printf("%d size: %d\n",pg.size(), allTracks.size());
        //allTracks.append(pg);
        //numberOfTracks++;
    }

/**
  *Funkcia na nastavenie významých bodov do premnennej wayPoints.
  *@param Zoznam bodov
  */
    void SlippyMap::setWpt(QVector<QPoint> wpt){
        wayPoints = wpt;
    }

    SlippyMap::~SlippyMap(){
    }
