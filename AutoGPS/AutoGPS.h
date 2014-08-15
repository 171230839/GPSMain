// Copyright 2014 ESRI
//
// All rights reserved under the copyright laws of the United States
// and applicable international laws, treaties, and conventions.
//
// You may freely redistribute and use this sample code, with or
// without modification, provided you include the original copyright
// notice and use restrictions.
//
// See the Sample code usage restrictions document for further information.
//

#ifndef AUTOGPS_H
#define AUTOGPS_H



#include <Map.h>
#include <QMainWindow>
#include "mapcontroller.h"
#include "masterthread.h"


class QDeclarativeEngine;
class QGraphicsWidget;
class QDeclarativeContext;
namespace EsriRuntimeQt
{
class MapGraphicsView;

}
using namespace EsriRuntimeQt;


class AutoGPS : public QMainWindow
{
    Q_OBJECT
public:
    explicit  AutoGPS (QWidget *parent = 0);
    ~AutoGPS ();

private slots:
    void updateNorthArrow();
    //public slots:
    //    void onPortListChanged();
private:
    EsriRuntimeQt::Map map;
    EsriRuntimeQt::MapGraphicsView* mapGraphicsView;
    MapController* mapController;
    QDeclarativeEngine* engine;
    QGraphicsWidget* overlayWidget;
    QObject* overlayUI;
    QObject* mainMenuUI;
    QDeclarativeContext * context ;
    MasterThread thread;


protected:
    void resizeEvent(QResizeEvent* event);
    void closeEvent(QCloseEvent *event);

};

#endif // AUTOGPS_H

