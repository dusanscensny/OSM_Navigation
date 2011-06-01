#ifndef MAPZOOM_H
#define MAPZOOM_H
#include <slippymap.h>
#include <lightmaps.h>
#include <tracks.h>
#include <navigation.h>


#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTimer>
#include <QProcess>

/**
 * @author Dušan Ščensný
 * @brief Táto trieda vytvára GUI hlavného menu
 * V hlavnom menu sú spúštané podprocesy pomocou triedy QProcess na pripájanie zariadení potrebných na navigáciu
 */
class MapZoom : public QMainWindow
{
    Q_OBJECT

private:
    LightMaps *map;
    QHBoxLayout *control_layout;
    QVBoxLayout *main_layout;
    //Tracks *trk;

    QProcess *procConnect;
    QProcess *proc;
    Tracks *track;//trieda na vykreslenie tratí

    Navigation nav; //pripojenie zariadenia, nastavenie aktuálnej pozície, identifikácia dát

    QPointF actualPos;//aktálne pozícia na mape získaná z nav. prístroja

    QPushButton *navigate;
    QPushButton *connectBTN;    
    QPushButton *waypoints;
    /**
*Pri zmene názvu sa použije táto premenná
*/
    bool wptBtn;
    /**
      *Potrebná pri zmene názvu tlačidla connect.
      */
    bool conBtn;
    /**
      *Pri pipojení zariadenia sa nastaví na true.
      */
    bool conDev;

    /**
    * Voľba vstupu USB/Bluetooth
    */
    bool usb;
    /**
    * používa sa pri podprocese spuštajúcom aplikáciu gebabbel
    */
    bool conBab;//
    /**
    * podproces gebabbel
    */
    QProcess *babel;
    /**
    * timer na nastavovanie aktuálne pozície zísakenj zo zariadenia
    */
    QTimer *timer;

public:
    MapZoom();
    virtual ~MapZoom();
    /**sloty potrebné v hlavnom menu
    */
private slots:
    void runGebabel();
    void aboutOsm();
    void zoom_plus_click();
    void zoom_minus_click();    
    void set_home();
    void draw_tracks();
    void draw_waypoints();
    void actual_position();
    void control_devices();//slot na kontrolu pripojenia zariadenia
    void readStdout();
    void connectToDevice();//slot na pripojenei zariadenia na navigaciu
    void infoAboutDevice();
};
#endif // MAPZOOM_H


