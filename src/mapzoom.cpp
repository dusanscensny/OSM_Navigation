#include "mapzoom.h"
#include "iostream"
/**
 *
 * Vytvorenie hlavného menu
 *
 * V hlavnom menu sa inicializuje mapa a vytvoria všetky tlačidlá a akcie potrebné pri práci
 * V hlavnom menu sú spúštané podprocesy pomocou triedy QProcess na pripájanie zariadení potrebných na navigáciu
 */
    MapZoom::MapZoom(): QMainWindow(0) {
        map = new LightMaps(this);
        QWidget *main = new QWidget;
        setCentralWidget(main);        

        QToolBar *toolbar = new QToolBar(tr("Tool"), 0);
        toolbar->setMaximumHeight(30);
        QPushButton *zoom_plus = new QPushButton(QIcon("img/plus.png"),tr("&+"));
        QPushButton *zoom_minus = new QPushButton(QIcon("img/minus.png"),tr("&-"));
        QPushButton *home = new QPushButton(QIcon("img/home.png"),tr("&Žilina"));        
        QPushButton *drawTracks = new QPushButton(QIcon("img/open.png"),tr("Load T&racks"));
        navigate = new QPushButton(QIcon("img/find.png"),tr("Find position"));
        connectBTN = new QPushButton(QIcon("img/connect.png"),tr("Connect"));
        waypoints = new QPushButton(QIcon("img/draw.png"),tr("Draw waypoints"));

        toolbar -> addWidget(zoom_plus);
        toolbar -> addWidget(zoom_minus);
        toolbar -> addSeparator();
        toolbar -> addWidget(home);
        toolbar -> addSeparator();
        toolbar -> addWidget(drawTracks);
        toolbar -> addWidget(waypoints);
        toolbar -> addSeparator();
        toolbar -> addWidget(navigate);        
        toolbar -> addWidget(connectBTN);


        control_layout = new QHBoxLayout;
        control_layout->addWidget(toolbar);

        main_layout = new QVBoxLayout;
        main_layout -> addLayout(control_layout);
        main_layout -> addWidget(map);

        main->setLayout(main_layout);
        map->setFocus();        
        set_home();

        QAction *nightModeAction = new QAction(QIcon("img/night.png"),tr("Night Mode"), this);
        nightModeAction->setCheckable(true);
        nightModeAction->setChecked(false);
        QAction *osmAction = new QAction(QIcon("img/osm.png"),tr("About OpenStreetMap"), this);
        QAction *zilinaAction = new QAction(QIcon("img/home.png"),tr("Ž&ilina"),this);

        connect(nightModeAction, SIGNAL(triggered()), map, SLOT(toggleNightMode()));
        connect(zoom_plus, SIGNAL(clicked()), SLOT(zoom_plus_click()));
        connect(zoom_minus, SIGNAL(clicked()), SLOT(zoom_minus_click()));
        connect(home, SIGNAL(clicked()),SLOT(set_home()));
        connect(zilinaAction, SIGNAL(triggered()),SLOT(set_home()));
        connect(osmAction, SIGNAL(triggered()), SLOT(aboutOsm()));

        //trasy a zaujímavé body
        connect(drawTracks, SIGNAL(clicked()),SLOT(draw_tracks()));
        connect(waypoints, SIGNAL(clicked()),SLOT(draw_waypoints()));
        waypoints->setDisabled(true);//Zablokuje talčidlo pretože bez načítania dát zo súboru je nepoužiteľné
        track = new Tracks();
        //trasy a zaujímavé body

        //pripojenie gps prístroja
        connect(connectBTN, SIGNAL(clicked()),SLOT(connectToDevice()));
        procConnect = new QProcess(this);
        procConnect->setReadChannel(QProcess::StandardError);
        connect(procConnect, SIGNAL(readyReadStandardOutput()), this, SLOT(infoAboutDevice()));
        conDev = false;//pripojenie pristroja  použijeme pri zmene stavu
        //pripojenie gps prístroja

        //cast pre timer a nastavenie aktuálnej pozície
        navigate->setDisabled(true);//Zablokuje talčidlo pretože bez pripojenia prístroja je nepoužiteľné
        connect(navigate, SIGNAL(clicked()),SLOT(actual_position()));
        conBtn = true; //nastaví stav (dôležitý pri zmene názvu tlačidla)
        timer = new QTimer(this);
        connect(timer, SIGNAL(timeout()), this, SLOT(control_devices()));
        proc = new QProcess(this);
        proc->setReadChannel(QProcess::StandardOutput);        
        connect(proc, SIGNAL(readyReadStandardOutput()), this, SLOT(readStdout()));
        //cast pre timer

        QMenu *menu = menuBar()->addMenu(QIcon("img/options.png"),tr("&Options"));
        menu->addAction(nightModeAction);
        menu->addSeparator();
        menu->addAction(zilinaAction);
        menu->addAction(osmAction);
        menu = menuBar()->addMenu(tr("Gebabbel"));

        //spustenie programu gebabbel
        conBab = true;
        babel = new QProcess(this);
        QAction *gbabel = new QAction(QIcon("img/gebabbel.png"),tr("Run Gebabel"),this);
        menu->addAction(gbabel);
        connect(gbabel, SIGNAL(triggered()),SLOT(runGebabel()));
        //spustenie programu gebabbel
    }
    /**
      *Spustí program Gebabbel.
      *Pri pokuse o ukončenie programu je možnosť výberu či progam naozaj ukončiť.
      */
    void MapZoom::runGebabel(){        
        if(conBab){        
        babel->start("gebabbel");        
        conBab = false;
    }else{        
        if(QMessageBox::information(this,
                              "End the application?",
                              "OK to END",
                              QMessageBox::Ok, QMessageBox::Cancel) == 0x400){
        babel->terminate();
        babel->close();
        conBab = true;
    }
    }
    }

    void MapZoom::aboutOsm() {
        QDesktopServices::openUrl(QUrl("http://www.openstreetmap.org"));
    }
    /**
      *Priblíži zoom.
    */
    void MapZoom::zoom_plus_click(){
        map->setZoom_p();
        map->setFocus();
    }

    /**
      *Vzdiali zoom.
    */

    void MapZoom::zoom_minus_click(){
        map->setZoom_m();
        map->setFocus();
    }

    /**
      *Nastaví mapu na prednastavenú GPS súradnicu.
    */
    void MapZoom::set_home(){//zilina
        map->setCenter(49.223377,18.739277);
        map->setFocus();
    }
    /**
      *Vyzve k výberu súboru obsahujúceho gps dáta.
    */
    void MapZoom::draw_tracks(){                
        map->setTrack(track->open());
        map->setFocus();
        waypoints->setDisabled(false);
    }    
    /**
      *Vykreslí zaujímavé body.
    */
    void MapZoom::draw_waypoints(){
        map->setWayPoints(track->getWaypoints());
        map->setFocus();
    }
    /**
      *Nastaví aktuálnu pozíciu na ktorej sa nachádzame.
      *
    */
    void MapZoom::actual_position(){
        if (conBtn){
        navigate->setText(tr("Stop"));
        timer->start(1000);
        conBtn = false;
        map->drawActualPosition(true);
        }else {
            timer->stop();
            conBtn = true;
            navigate->setText(tr("Find position"));
            map->drawActualPosition(false);
        }
        map->setFocus();
    }

    /**
      *Ak proces nie je otvorený spustí načítavanie dát
      *
    */
    void MapZoom::control_devices(){              
    if(!conBtn){
        if(!proc->isOpen()){
            if(usb){
                proc->start("cat /dev/ttyUSB0");
            }else proc->start("cat /dev/rfcomm0");


        }else proc->close();
    }
    }      
    /**
      *Spustenie nastavenia aktuálnej pozície.
      *V prípade že vstup dát z prístroja nie je čitateľný nastaví aktuálnu pozíciu.
    */
    void MapZoom::readStdout(){
        if (proc->isReadable()){
            QByteArray data = proc->readAllStandardOutput();            
            actualPos = nav.findDevices(data);
            if((actualPos.x() != 0.0) or (actualPos.y()!= 0.0)){
                map->setCenter(actualPos.x(),actualPos.y());
            }else actual_position();

        }
        proc->terminate();
        proc->close();

    }      
    /**
      *Pripojí aktuálne nastavené zariadenia podľa MAC adresy prístroja.
      *Nastavuje názov tlačidla.
      *
    */
    void MapZoom::connectToDevice(){
        if(!conDev){//ak nie je pripojený prístroj
        QByteArray data;
        QDialog *choosedialog = new QDialog(this);
        QVBoxLayout *chooselayout = new QVBoxLayout;
        QRadioButton *radio1 = new QRadioButton(tr("&USB"));
        QRadioButton *radio2 = new QRadioButton(tr("&Bluetooth"));
        radio1->setChecked(true);
        chooselayout->addWidget(radio1);
        chooselayout->addWidget(radio2);
        QDialogButtonBox *btn = new QDialogButtonBox(QDialogButtonBox::Ok|QDialogButtonBox::Cancel);
        chooselayout->addWidget(btn);
        connect(btn, SIGNAL(accepted()), choosedialog, SLOT(accept()));
        connect(btn, SIGNAL(rejected()), choosedialog, SLOT(reject()));
        choosedialog->setLayout(chooselayout);

        if(choosedialog->exec() == QDialog::Accepted){
            if(radio1->isChecked()){
                if(!conDev){
                    map->setFocus();
                    navigate->setDisabled(false);
                    connectBTN->setText(tr("Disconnect"));
                    conDev = true;
                    usb = true;
                }

            }else{
                usb = false;//bolo označebé Bluetooth

                    QDialog *dialog = new QDialog(this);
                    QVBoxLayout *layout = new QVBoxLayout;
                    dialog->setLayout(layout);
                    QLineEdit *line = new QLineEdit;
                    line->setInputMask("HH:HH:HH:HH:HH:HH");
                    layout->addWidget(line);
                    QDialogButtonBox *buttons = new QDialogButtonBox(QDialogButtonBox::Ok|QDialogButtonBox::Cancel);
                    layout->addWidget(buttons);
                    connect(buttons, SIGNAL(accepted()), dialog, SLOT(accept()));
                    connect(buttons, SIGNAL(rejected()), dialog, SLOT(reject()));
                    line->setText("00:0B:0D:95:70:D1");
                    if (dialog->exec() == QDialog::Accepted){
                        procConnect->start("rfcomm connect rfc0 " + line->text());
                        if(procConnect->isReadable()){
                            data = procConnect->readAllStandardOutput();
                            std::cerr<<"Data: "<<data.size()<<procConnect->exitCode()<< std::endl;
                            map->setFocus();
                            navigate->setDisabled(false);
                            connectBTN->setText(tr("Disconnect"));
                            conDev = true;
                        }else {
                            QMessageBox::warning(this,
                                                 procConnect->errorString(),
                                                 "OK to END",
                                                 QMessageBox::Ok, QMessageBox::Cancel);
                        }
                    }                
            }
        }
        }else{
                    if(!conBtn) actual_position();
                    navigate->setDisabled(true);
                    connectBTN->setText(tr("Connect"));
                    procConnect->terminate();
                    procConnect->kill();
                    procConnect->close();
                    conDev= false;
                }
    }
    /**
      *Vráti dáta z výstupu stdout po pripojení zariadenia.
      *
    */
    void MapZoom::infoAboutDevice(){
        if(proc->isReadable()){
            QByteArray data = procConnect->readAllStandardOutput();
            std::cerr<<"Data: "<<data.constData()<< std::endl;
            //printf("%s \n",data.constData());
        }else std::cerr<<"No redeable process: "<< std::endl;
    }

    /**
      *Deštruktor zruší procesy ktoré neboli ukončené.
      */
    MapZoom::~MapZoom(){
        babel->terminate();
        babel->close();
        if (babel != NULL){
            delete babel;
            babel = NULL;
        }
        if(timer != NULL){
            delete timer;
            timer = NULL;
        }        
        if(proc != NULL){
            delete proc;
            proc = NULL;
        }
        if(procConnect != NULL){
            delete procConnect;
            procConnect = NULL;
        }
        if(track != NULL){
            delete track;
            track = NULL;
        }

    }
