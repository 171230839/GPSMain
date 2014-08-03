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

namespace EsriRuntimeQt
{
class MapGraphicsView;
}

#include "Map.h"

//Uncommented layer(s) needed
#include "ArcGISLocalTiledLayer.h"
#include "ArcGISTiledMapServiceLayer.h"
//#include "LocalMapService.h"
//#include "ArcGISDynamicMapServiceLayer.h"
//#include "LocalFeatureService.h"
//#include "ArcGISFeatureLayer.h"
//#include "FeatureLayer.h"
//#include "GraphicsLayer.h"

#include <QMainWindow>

QT_BEGIN_NAMESPACE
class QAction;
class QMenu;
class QTreeView;
class QDirModel;
QT_END_NAMESPACE




class AutoGPS : public QMainWindow
{
    Q_OBJECT
public:
    AutoGPS (QWidget *parent = 0);
    ~AutoGPS ();

public slots:
    //  void onMapReady();
    //  void onLocalServiceCreationSuccess(const QString& url, const QString& name);
    //  void onLocalServiceCreationFailure(const QString& name);
    //  void onFeatureServiceCreationSuccess(const QString& url, const QString& name);
    //  void onFeatureServiceCreationFailure(const QString& name);
private slots:
    void newFile();
    void open();
    bool save();
    bool saveAs();
    void about();

private:
    EsriRuntimeQt::Map m_map;
    EsriRuntimeQt::MapGraphicsView* m_mapGraphicsView;
    EsriRuntimeQt::ArcGISLocalTiledLayer m_tiledLayer;
    //     EsriRuntimeQt::ArcGISTiled MapServiceLayer m_tiledServiceLayer;
    //  EsriRuntimeQt::ArcGISDynamicMapServiceLayer m_dynamicServiceLayer;
    //  EsriRuntimeQt::LocalMapService m_localMapService;
    //  EsriRuntimeQt::ArcGISDynamicMapServiceLayer m_dynamicLocalServiceLayer;
    //  EsriRuntimeQt::LocalFeatureService m_localFeatureService;
    //  EsriRuntimeQt::ArcGISFeatureLayer m_localFeatureLayer;
    //  EsriRuntimeQt::GraphicsLayer m_graphicsLayer;
    //  EsriRuntimeQt::FeatureLayer m_featureLayer;

    void createActions();
    void createMenus();
    void createToolBars();
    void createStatusBar();
    void readSettings();
    void writeSettings();
    bool maybeSave();
    void createDockWidget();
    void setCurrentDir(const QString &DirName);
    bool saveDir(const QString &DirName);
    QString strippedName(const QString &fullDirName);
    void loadDir(const QString dirName);
    QMenu *fileMenu;

    QMenu *helpMenu;
    QToolBar *fileToolBar;

    QAction *newAct;
    QAction *openAct;
    QAction *saveAct;
    QAction *saveAsAct;
    QAction *exitAct;
    QAction *cutAct;
    QAction *copyAct;
    QAction *pasteAct;
    QAction *aboutAct;
    QAction *aboutQtAct;
    QDockWidget *dockWidget;
    QTreeView *treeView;
    QDirModel *model;

    QString curDir;
protected:
    void closeEvent(QCloseEvent *event);
};

#endif // AUTOGPS_H

