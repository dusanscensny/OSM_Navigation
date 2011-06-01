#ifndef LIGHTMAPS_H
#define LIGHTMAPS_H
#include <QWidget>
#include <slippymap.h>

/**
 * @author Dušan Ščensný
 * @brief V tejto triede prebieha vykreslenie jednotlivých častí na mape
 *
 */

typedef QVector<QPointF> PointVector; //Vektor bodov
bool rangeIn(QPoint p,int lim);

class LightMaps : public QWidget{
    Q_OBJECT
private:
/**
  *Rozsah dát pre vykreslenie.
  */
    QPointF min;
    QPointF max;
    QPoint getPoint(qreal lat,qreal lon);//zatial nevyuzita
    QPointF getPositionData(QPoint pos);    
/**
  *Dáta pre vykreslenie trás.
  */
    QVector<QPolygonF> track_in_points;
    PointVector trk;
/**
  *Dáta pre vykreslenie zaujímavých bodov sú udávané v geografických súradniciach kôli prepočtom pri pohybe mapy.
  */
    PointVector waypointCoor;
    QVector<PolygonVector> allTracks; //vsetky mozne trasy spolu v jednom poli



    void setRange();
/**
  * Funkcia prepočítava hodnoty bodov na mape.
  */
    void recalculateTrack();
/**
  *Funkcia prepočítava hodnoty bodov na mape.
  */
    void recalculateWpt();
/**
  * Funkcia vyhodnocuje rozsah na mape.
  */
    bool rangeIn(QPoint p,int lim);

public:
    LightMaps(QWidget *parent = 0);
    void setTrack(QVector<QPointF> track);//set one track to Vector of QPointF
    void setWayPoints(QVector<QPointF> wpt);//set one track to Vector of QPointF
    void setCenter(qreal lat, qreal lng); //set center of map
    QPoint getPixelFROMlatitude(QPointF pos);//fuction returns position on map from coordinates
    void drawActualPosition(bool status);

    virtual ~LightMaps();

public slots:
    void toggleNightMode();
    void setZoom_p();
    void setZoom_m();

private slots:
    void updateMap(const QRect &r);

protected:
    void resizeEvent(QResizeEvent *);
    void paintEvent(QPaintEvent *event); 
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *);
    void keyPressEvent(QKeyEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);



private:
    SlippyMap *m_normalMap;    
    bool pressed;
    bool snapped;
    QPoint pressPos;
    QPoint dragPos;
    QPointF actual_coor;        
    QPixmap zoomPixmap;
    QPixmap maskPixmap;
    bool invert;

};

#endif // LIGHTMAPS_H
