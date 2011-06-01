#ifndef NAVIGATION_H
#define NAVIGATION_H

#include <QString>
#include <QPointF>
#include <QProcess>

/**
 * @author Dušan Ščensný
 * @brief Táto trieda konvertuje dáta pri vykresľovaní aktuálnej pozície
 */
class Navigation
{
public:
    QProcess *proc;

    Navigation();
    QPointF findDevices(QByteArray data);
    QPointF getCoordinates();
    static void setPath(const QString &newPath);
    static QString getPath();
    int pocet;

private slots:
    void readStdout();

};

#endif // NAVIGATION_H
