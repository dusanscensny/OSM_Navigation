#include "tracks.h"
#include <QtGui>
#include <QDir>
/**
 * Trieda ktorá pracuje s traťami
 * Načíta dáta zo súboru a rodelí ich na trate a významné body
 * V hlavnom menu sa inicializuje mapa a vytvoria všetky tlačidlá a akcie potrebné v hlavnom menu
 *
 */
Tracks::Tracks()
{

}
/**
  *Spustí filechooser na výber súboru následne súbor otvorí.
  *Po rozdelení dát vráti zoznam tratí.
  *V prípade neúspechu oznámi výstrahou naúspech , inak vráti oznámenie o počte trás.
  */
QVector <QPointF> Tracks::open(){
    QWidget *parent=0;    
    dir = QFileDialog::getOpenFileName(parent,"Open GPX File",
                                                 QDir::currentPath(), "GPX files (*.gpx)");
    QXmlStreamAttributes attributes;    
    int seg = 0;
    int track_num = 0; //number of track
    QVector <QPointF> track;

    QPointF point;
    track.clear();
    waypoint.clear();//Zrusi predosle data typu waypoint

    if (dir.isEmpty())
        return track;

    QFile* file = new QFile(dir);
    if (!file->open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(parent, "GPX Files",
                             "Cannot read file" + dir + " :" + file->errorString(),
                             QMessageBox::Ok);
        return track;
    }

    QXmlStreamReader xml(file);
    while(!xml.atEnd() &&
            !xml.hasError()) {
        QXmlStreamReader::TokenType token = xml.readNext();
        /* If token is just StartDocument, we'll go to next.*/
        if(token == QXmlStreamReader::StartDocument) {
            continue;
        }
        /* If token is StartElement, we'll see if we can read it.*/
        if(token == QXmlStreamReader::StartElement) {            
            /*if(xml.name() == "gpx") {
                if (xml.attributes().value("version") != "1.0"){
                    QMessageBox::critical(parent,
                              "Version of gpx",
                              "Bad xml version",
                              QMessageBox::Ok);
                    break;
        }
        }     */
            /* If it's named persons, we'll go to the next.*/
            if(xml.name() == "wpt") {
                    if(!(point = parseXml(xml.attributes())).isNull()){
                        waypoint.append(point);
                    }
            }            
            if(seg == 1){
                if(xml.name() == "trkpt") {
                    if(!(point = parseXml(xml.attributes())).isNull()){
                        track.append(point);
                    }
                }                
            }
        //printf("XML:: %s",xml.name().toString().toAscii());
        }
        if(token == QXmlStreamReader::StartElement) {
                if(xml.name() == "trkseg"){
                    seg++;
                    track_num++;

                }
            }
            if(token == QXmlStreamReader::EndElement){
                if(xml.name() == "trkseg"){
                    seg--;
                    track.append(QPointF(999999.9,999999.9));
                }
            }
    }
    printf("FILE XML:: %d , Seg: %d \n",track.size(),seg);//,xml.name().toString().toAscii());
    if(xml.hasError()) {
        QMessageBox::critical(parent,
                              "QXSRExample::parseXML",
                              xml.errorString(),
                              QMessageBox::Ok);
    }else{
        QString num;
        num.setNum(track_num);
        QMessageBox::information(parent,
                              "Track status",
                              "Number of tracks: "+num,
                              QMessageBox::Ok);
    }
    /* Removes any device() or data from the reader
     * and resets its internal state to the initial state. */
    xml.clear();
    file->close();
    if (file != NULL){
        delete file;
        file = NULL;
    }
    return track;
}
/**
  *Funkcia rozdelí XML atribúty a vráti hodnoty súradníc
  *@param atribúty z xml súboru daného riadku
  *@return zemepisné súradnice
  *
 */
QPointF Tracks::parseXml(QXmlStreamAttributes atr){
    qreal lat =0.0;
    qreal lon =0.0;     
    if(!atr.isEmpty()){
        lat = atr.value("lat").toString().toFloat();
        lon = atr.value("lon").toString().toFloat();

        if((lat != 0.0 )&& (lon != 0.0)){
                    //printf("Latitude from file: %s \n",atr.value("lat").toString().toAscii().data());
        //printf("Latitude from file: %f \n",lat);    
        }//else
                    //printf("Latitude or longitude is incorrect \n");

    }
    return QPointF(lat,lon);
}
QVector <QPointF> Tracks::getWaypoints(){
    return waypoint;
}
