#include "navigation.h"

#include <iostream>
#include <QFile>
#include <QIODevice>

/**
  *Funkcia ktorá kovertuje zo stupňov a minut do stupňov
  *@param Vstupné dáta z prijímača
  */
float degmin2deg(QByteArray in){
    int i,j;
    QByteArray deg;
    QByteArray min;    
    i = 0;
    while (in[i] != '.'){
        i++;
    }
    for (j=0; j<=(i-3); j++){
        deg.append(in[j]);
    }
    for (j=(i-2); j <= in.length(); j++){
        min.append(in[j]);
    }
    return deg.toFloat()+min.toFloat()/60;
}


Navigation::Navigation()
{

}

/**
  *Funkcia ktorá zo vstpných dát oddelí potrebné dáta.
  *@param Vstupné dáta z prijímača
  *@return Hodnota zemepisnej súradnice.
  */
QPointF Navigation::findDevices(QByteArray data){     
    int ciarky=0;    
    int i,j;
    QPointF sur;//= new QPointF(0.0,0.0);
    QByteArray vs =data;
    QByteArray outlt = 0;// = new QByteArray(30);
    QByteArray outln = 0;
    QByteArray sat;

    for(i = data.length()-5; i>=0 ; i--){//nájde retazce kde sa nachádza gpgga
        if ((vs[i] == 'G')and (vs[i+1]== 'P')and (vs[i+2] == 'G') and (vs[i+3] == 'G') and (vs[i+4] == 'A')){
            if(ciarky ==0){
            //std::cerr<<"Test: "<<vs[i]<<vs[i+1]<<vs[i+2]<<vs[i+3] << std::endl;
            for(j=0;j<60;j++){
                if((ciarky == 2) and (vs[i+j] !=',')){
                    outlt.append(vs[i+j]);
                    //std::cerr<<"outlt: "<<outln[i] << std::endl;
                }
                if((ciarky == 4) and (vs[i+j] !=',')){
                    outln.append(vs[i+j]);
                }
                if((ciarky == 7) and (vs[i+j] !=',')){
                    sat.append(vs[i+j]);
                }

                if (vs[i+j] == ',') ciarky++;
                if (ciarky > 7) i=0;
            }
            //std::cerr<<"Ciarky: "<<toascii(ciarky) << std::endl;

        }
        }

    }

    ciarky = 0;
    QByteArray speed;
    for(i = data.length()-5; i>=0 ; i--){//nájde retazce kde sa nachádza gpvtg
        if ((vs[i] == 'G')and (vs[i+1]== 'P')and (vs[i+2] == 'V') and (vs[i+3] == 'T') and (vs[i+4] == 'G')){
            if(ciarky ==0){
            //std::cerr<<"Test: "<<vs[i]<<vs[i+1]<<vs[i+2]<<vs[i+3] << std::endl;
            for(j=0;j<60;j++){
                if((ciarky == 7) and (vs[i+j] !=',')){
                    speed.append(vs[i+j]);
                    //std::cerr<<"outlt: "<<outln[i] << std::endl;
                }
                if (vs[i+j] == ',') ciarky++;
                if (ciarky > 8) i=0;
            }


        }
        }

    }
    std::cerr<<"Speed: "<<speed.constData()<<"  Sat: "<< sat.constData() << std::endl;//vypíše rýchlosť a počet satelitov

    sur.setX(degmin2deg(outlt));
    sur.setY(degmin2deg(outln));

    return sur;


    //printf("LT: %f, LN: %f",degmin2deg(outlt),degmin2deg(outln));
    //std::cerr<<"Data: "<<data.constData()<< std::endl;
    //std::cerr<<"Lt: "<<<< std::endl;
}

QPointF Navigation::getCoordinates(){
    QPointF actual(0.0,0.0); //koordinaty ktorymi vratime aktulanu poziciu
  return actual;
}

void Navigation::readStdout(){

}
