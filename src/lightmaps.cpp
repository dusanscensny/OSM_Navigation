#include "lightmaps.h"

const int tdim = 256;


/**
  *Konštruktor triedy Lightmaps, kontrola rozsahu, či sa bod nenachádza mimo viditelnej plochy
  *lim - limit za rozsah po ktorý necháme vykresliť body aj mimo plochy
  *@param Nainicializované premenné potrebné pri komunikácii s GUI rozhraním
  */
LightMaps::LightMaps(QWidget *parent)
            : QWidget(parent)
            , pressed(false)
            , snapped(false)  
            , invert(false) {
        m_normalMap = new SlippyMap(this);        
        connect(m_normalMap, SIGNAL(updated(QRect)), SLOT(updateMap(QRect)));
}

/**
  *Nastaví mapu na hodnotu vstupného parametra
  *@param Zemepisné súradnice
  */
void LightMaps::setCenter(qreal lat, qreal lng) {
    m_normalMap->latitude = lat;
    m_normalMap->longitude = lng;
    m_normalMap->invalidate();
}
/**
  *Nastaví premennú označujúcu nočný mód
  */
void LightMaps::toggleNightMode() {
    invert = !invert;
    update();
}
/**
  *zvýši zoom a aktualizuje mapu
  */
void LightMaps::setZoom_p(){
    if(m_normalMap->zoom <18){
        m_normalMap->zoom += 1;
        m_normalMap->invalidate();
    }
}
/**
  *zníži zoom a aktualizuje mapu
  */
void LightMaps::setZoom_m(){
    if(m_normalMap->zoom >1){
        m_normalMap->zoom -= 1;
        m_normalMap->invalidate();
    }
}
/**
  *nastavi rozsah a prepočíta vykreslene trasy
  */
void LightMaps::updateMap(const QRect &r) {
    setRange();
    recalculateTrack();
    recalculateWpt();
    update(r);
}
/**
  *Zmení veľkosť mapy pri zmene veľkosti obrazovky
  */

void LightMaps::resizeEvent(QResizeEvent *) {
    m_normalMap->width = width();
    m_normalMap->height = height();
    m_normalMap->invalidate();
}
/**
  *Vykreslí hodnoty súradníc a samotný zoom (text na spodnej časti programu
  */
void LightMaps::paintEvent(QPaintEvent *event) {
    QPainter p;
    char data[255];
    p.begin(this);
    m_normalMap->render(&p, event->rect());
    p.setPen(Qt::black);
    if(m_normalMap->zoom <18){
        sprintf(data,"Lat: %f,  Long: %f, Zoom: %d ",
                m_normalMap->latitude,m_normalMap->longitude, m_normalMap->zoom);        
    }else sprintf(data,"Lat: %f,  Long: %f, Zoom: MAX",
                  m_normalMap->latitude,m_normalMap->longitude);
    p.drawText(rect(),  Qt::AlignBottom | Qt::TextWordWrap,"OSM Navigation");
    p.drawText(rect(),  Qt::AlignBottom |Qt::AlignRight | Qt::TextWordWrap,data);
    p.end();
    //p.~QPainter;
    if (invert) {
        QPainter p(this);
        p.setCompositionMode(QPainter::CompositionMode_Difference);
        p.fillRect(event->rect(), Qt::white);
        p.end();
    }

}
/**
  *Zapíše hodnotu kliknutia myši na mape
  *@param QMouseEvent
  */
void LightMaps::mousePressEvent(QMouseEvent *event) {
    if (event->buttons() != Qt::LeftButton)
        return;
    pressed = snapped = true;
    pressPos = dragPos = event->pos();
}
/**
  *Funckia sa vykonáva pri pohybe myši
  *@param QMouseEvent
  */
void LightMaps::mouseMoveEvent(QMouseEvent *event) {
    if (!event->buttons())
        return;
    if (!pressed || !snapped) {
        QPoint delta = event->pos() - pressPos;
        pressPos = event->pos();
        m_normalMap->pan(delta);
        return;
    } else {
        const int threshold = 10; //tolerancia pohybu
        QPoint delta = event->pos() - pressPos;
        if (snapped) {
            snapped &= delta.x() < threshold;
            snapped &= delta.y() < threshold;
            snapped &= delta.x() > -threshold;
            snapped &= delta.y() > -threshold;
        }
    }
    update();
}
/**
  *Pri uvoľnení tlačidla
  *@param QMouseEvent
  */
void LightMaps::mouseReleaseEvent(QMouseEvent *) {
    update();
}
/**
  *Pri dvojkliku
  *@param QMouseEvent
  */
void LightMaps::mouseDoubleClickEvent(QMouseEvent *event){
    if(event->buttons() == Qt::LeftButton){
        setZoom_p();
        update();
    }
    if(event->buttons() == Qt::RightButton){
        setZoom_m();
        update();
    }
}
/**
  *Rolovanie kolečka myši
  *@param QWheelEvent
  */
void LightMaps::wheelEvent(QWheelEvent *event){
    int numDegrees = event->delta() / 8;
    int numSteps = numDegrees / 15;
    if( Qt::Vertical == event->orientation()){
        if (numSteps > 0) setZoom_p();
        if (numSteps < 0) setZoom_m();
    }
}
/**
  *Pri stlačení klávesu na myši
  *@param QKeyEvent
  */
void LightMaps::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Left)
        m_normalMap->pan(QPoint(20, 0));
    if (event->key() == Qt::Key_Right)
        m_normalMap->pan(QPoint(-20, 0));
    if (event->key() == Qt::Key_Up)
        m_normalMap->pan(QPoint(0, 20));
    if (event->key() == Qt::Key_Down)
        m_normalMap->pan(QPoint(0, -20));
    if (event->key() == Qt::Key_Z || event->key() == Qt::Key_Select) {
        dragPos = QPoint(width() / 2, height() / 2);
    }
}
/**  
  *Výpočet pozície na mape
  *@param zmemepisná súradnica
  *@return hodnota v pixloch
  *
  */
QPoint LightMaps::getPixelFROMlatitude(QPointF pos){
    QPoint pos_in_pix;
    QPointF pom;
    pom.setX(pos.y());
    pom.setY(pos.x());
    qreal xsur = m_normalMap->width - (m_normalMap->width /(min.x()-max.x())*(pom.x()-max.x()));
    pos_in_pix.setX(xsur);
    //nastavime X
    qreal ysur =m_normalMap->height - (m_normalMap->height/(min.y()-max.y())*(pom.y()-max.y()));
    pos_in_pix.setY(ysur);
    //nastavime Y
    return pos_in_pix;
}
/**
  *Nastaví rozsah na ktorej sa mapa nachádza
  *hodnoty min a max
  */
void LightMaps::setRange(){
    QPoint position(0,0);//in pixel
    min = getPositionData(position); //return lat and lon
    position.setX(m_normalMap->width);
    position.setY(m_normalMap->height);
    max = getPositionData(position);
}
/**
  *Vráti hodnoty zmepisných súradníc z honoty pixloch na mape
  *@param pozícia myši
  *@return zemepisná súradnica
  */
QPointF LightMaps::getPositionData(QPoint pos){
    QPointF dx = QPointF(pos);
    dx.setX(m_normalMap->width/2 - dx.x());
    dx.setY(m_normalMap->height/2- dx.y());
    dx = dx/ qreal(tdim);
    QPointF center = tileForCoordinate(m_normalMap->latitude,m_normalMap->longitude, m_normalMap->zoom) - dx;
    actual_coor.setY(latitudeFromTile(center.y(), m_normalMap->zoom));
    actual_coor.setX(longitudeFromTile(center.x(), m_normalMap->zoom));    
    return actual_coor;
}
/**
  *Vloží trať do premennej trk
  *@param všetky trasy načítané zo súboru
  */
void LightMaps::setTrack(QVector<QPointF> track){
    if (!track.isEmpty()){
        trk.clear();
        for(int i=0;i < track.size();i++){
            trk.append(track.at(i));
        }
        recalculateTrack();
        update();
    }
}
/**
  *Funkcia ktorá prepočítava trasy, na vykreslenie na mapu
  *Tieto trasy sú rozdelené na mešie časti ak sú niektoré časti mimo plochu
  *je to z toho dôvodu aby sme neprepočítavali a nevykresľovali zbytočné časti mapy.
      *Každá časť trasy je zapísaná v polygóne. Dôležité je pridať na zažiatok a na koniec polygónu      
      *jeden bod ktorý je mimo trasy aby nedošlo k skresleniu trasy.
  */
void LightMaps::recalculateTrack(){
    QPointF controlPoint(999999.9,999999.9);
    QPolygon pg;
    QVector<QPolygon> setOfPg; //set of polygon contains one track
    QPoint p,pl;
    int track =1;    
    allTracks.clear();
    trk.begin();
    for(int i=0;i < trk.size();i++){
        if(trk.at(i) != controlPoint){ //ak je to kontrolný bod musíme vytvoriť novú trasu
            pl =p;
            p = getPixelFROMlatitude(trk.at(i));
            if(rangeIn(p,0)){
                if(i>0)
                    if((trk.at(i-1) != controlPoint)){
                        //      pl = getPixelFROMlatitude(trk.at(i-1));
                        pg.append(pl);
                    }
                pg.append(p);
                    //printf("Track: %d, point X: %d, Y: %d \n",track,p.x(),p.y());                
            }else{//ak sme našli kontrolný bod musime vytvoriť dalšiu trasu
                if(!rangeIn(pl,0)){ //ak je posledny bod mimo rozsah zapripocita este jeden
                    if(!pg.isEmpty()){
                        setOfPg.append(pg);
                        pg.clear();
                        track++;
                    }                    
                }else{
                    pg.append(p);
                    pl = p;
                }
            }
        }else{
            if(!pg.isEmpty()){
            }else pg.append(p);
            setOfPg.append(pg);
            pg.clear();
            allTracks.append(setOfPg);
            setOfPg.clear();
        }
        if((i == trk.size()-1) && (!pg.isEmpty())){
            setOfPg.append(pg);
            if(allTracks.isEmpty()) allTracks.append(setOfPg);
        }
    }
    m_normalMap->setTrack(allTracks);//setOfPg);
}
/**
  *Nastaví významné body do premennej waypointCoor
  */
void LightMaps::setWayPoints(QVector<QPointF>wpt){
    if(!wpt.isEmpty()){
        waypointCoor.clear();
        for(int i=0;i < wpt.size();i++){
            waypointCoor.append(wpt.at(i));
        }
        recalculateWpt();
        update();
    }
}
/**
  *Prepočítava význmné body do hodnoty v pixloch
  */
void LightMaps::recalculateWpt(){
    QVector<QPoint> waypoints;
    QPoint p;
    int lim = 30;
    for(int i=0;i < waypointCoor.size();i++){
        p = getPixelFROMlatitude(waypointCoor.at(i));
        if(rangeIn(p,lim)){
            waypoints.append(p);
            //printf("Track: %d, point X: %d, Y: %d \n",track,p.x(),p.y());
        }
    }
    m_normalMap->setWpt(waypoints);
}
/**
  *Zmení status navigácie a tým spustí vykresľovanie aktuálnej pozície
  */
void LightMaps::drawActualPosition(bool status){
    m_normalMap->navigate = status;
}
/**
  *Funkcia kontroluje ci sa daný bod nachádza na rozsahu mapy
  */
bool LightMaps::rangeIn(QPoint p,int lim){
    if((p.x()>-lim && p.x()<m_normalMap->width+lim) && (p.y()>-lim ) && (p.y()< m_normalMap->height+lim)){
        return true;
    }else return false;
}

LightMaps::~LightMaps(){
}
