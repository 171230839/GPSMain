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

#ifndef GPSMAIN_H
#define GPSMAIN_H

namespace EsriRuntimeQt
{
class MapGraphicsView;
}

#include "Map.h"

//Uncommented layer(s) needed
//#include "ArcGISLocalTiledLayer.h"
//#include "ArcGISTiledMapServiceLayer.h"
//#include "LocalMapService.h"
//#include "ArcGISDynamicMapServiceLayer.h"
//#include "LocalFeatureService.h"
//#include "ArcGISFeatureLayer.h"
//#include "FeatureLayer.h"
//#include "GraphicsLayer.h"

#include <QMainWindow>

class GPSMain : public QMainWindow
{
    Q_OBJECT
public:
    GPSMain (QWidget *parent = 0);
    ~GPSMain ();

public slots:
    //  void onMapReady();
    //  void onLocalServiceCreationSuccess(const QString& url, const QString& name);
    //  void onLocalServiceCreationFailure(const QString& name);
    //  void onFeatureServiceCreationSuccess(const QString& url, const QString& name);
    //  void onFeatureServiceCreationFailure(const QString& name);

private:
    EsriRuntimeQt::Map m_map;
    EsriRuntimeQt::MapGraphicsView* m_mapGraphicsView;
    //  EsriRuntimeQt::ArcGISLocalTiledLayer m_tiledLayer;
    //  EsriRuntimeQt::ArcGISTiledMapServiceLayer m_tiledServiceLayer;
    //  EsriRuntimeQt::ArcGISDynamicMapServiceLayer m_dynamicServiceLayer;
    //  EsriRuntimeQt::LocalMapService m_localMapService;
    //  EsriRuntimeQt::ArcGISDynamicMapServiceLayer m_dynamicLocalServiceLayer;
    //  EsriRuntimeQt::LocalFeatureService m_localFeatureService;
    //  EsriRuntimeQt::ArcGISFeatureLayer m_localFeatureLayer;
    //  EsriRuntimeQt::GraphicsLayer m_graphicsLayer;
    //  EsriRuntimeQt::FeatureLayer m_featureLayer;
};

#endif // GPSMAIN_H

