#ifndef TRACKS_H
#define TRACKS_H
#include <QFileDialog>
#include <QApplication>
#include <QXmlStreamReader>

/**
 * @author Dušan Ščensný
 * @brief Táto trieda komunikuje s GUI častou aplikácie
 *
 */

class Tracks        
{    
private:
    QString dir;
    QFileDialog file;
    QVector <QPointF> waypoint;

    QXmlStreamReader xml;
    QPointF parseXml(QXmlStreamAttributes atr);
public:
    Tracks();
    QVector <QPointF> open();
    QVector <QPointF> getWaypoints();
};

#endif // TRACKS_H
