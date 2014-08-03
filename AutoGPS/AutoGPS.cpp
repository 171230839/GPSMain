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

#include "AutoGPS.h"

#include "MapGraphicsView.h"
#include "ArcGISRuntime.h"

// Uncomment if needed
//#include "Graphic.h"
//#include "SimpleMarkerSymbol.h"
//#include "Point.h"
//#include "ServiceInfoTask.h"
//#include "GeodatabaseFeatureServiceTable.h"
#include <QtWidgets>

AutoGPS::AutoGPS(QWidget *parent)
    : QMainWindow(parent)
{


    // set to openGL rendering
    EsriRuntimeQt::ArcGISRuntime::setRenderEngine(EsriRuntimeQt::RenderEngine::OpenGL);

    m_mapGraphicsView = EsriRuntimeQt::MapGraphicsView::create(m_map, this);
    setCentralWidget(m_mapGraphicsView);

    createActions();
    qDebug()<<"1";
    createMenus();
    qDebug()<<"2";
    createToolBars();
    qDebug()<<"3";
    createStatusBar();
    qDebug()<<"4";
    createDockWidget();
    qDebug()<<"5";
    readSettings();
    qDebug()<<"6";

    m_map.setWrapAroundEnabled(false);

    // show Esri logo by default
    m_map.setEsriLogoVisible(true);

//    QString path = EsriRuntimeQt::ArcGISRuntime::installDirectory();
//    path.append("/sdk/samples/data");
//    QDir dataDir(path); // using QDir to convert to correct file separator
//    QString pathSampleData = dataDir.path() + QDir::separator();
//    qDebug()<<"pathSampleData"<<pathSampleData;
    //// ArcGIS Online Tiled Basemap Layer
    //    m_tiledServiceLayer = EsriRuntimeQt::ArcGISTiledMapServiceLayer("http://services.arcgisonline.com/ArcGIS/rest/services/World_Topo_Map/MapServer");
    //    m_map.addLayer(m_tiledServiceLayer);

    //// Local Tiled Basemap Layer using: sdk/samples/data/tpks/Topographic.tpk
    // QString tiledBaseMapLayer = pathSampleData + "tpks" + QDir::separator() + "Topographic.tpk";
//    QString tiledBaseMapLayer = QCoreApplication::applicationDirPath() + QDir::separator() + "ok.tpk";
//    m_tiledLayer = EsriRuntimeQt::ArcGISLocalTiledLayer(tiledBaseMapLayer);
//    m_map.addLayer(m_tiledLayer);

    //// ArcGIS Online Dynamic Map Service Layer
    //m_dynamicServiceLayer = EsriRuntimeQt::ArcGISDynamicMapServiceLayer("http://sampleserver1.arcgisonline.com/ArcGIS/rest/services/Specialty/ESRI_StateCityHighway_USA/MapServer");
    //m_map.addLayer(m_dynamicServiceLayer);

    //Local Dynamic Layer using: sdk/samples/data/mpks/USCitiesStates.mpk
    /*
  QString dataPath = pathSampleData + "mpks" + QDir::separator() + "USCitiesStates.mpk";
  m_localMapService = EsriRuntimeQt::LocalMapService(dataPath);

  // connect to signal that is emitted when the service is created successfully
  connect(&m_localMapService, SIGNAL(serviceCreationSuccess(const QString&, const QString&)), this, SLOT(onLocalServiceCreationSuccess(const QString&, const QString&)));

  // connect to signal that is emitted when the service failed
  connect(&m_localMapService, SIGNAL(serviceCreationFailure(const QString&)), this, SLOT(onLocalServiceCreationFailure(const QString&)));

  // start the service and the Local Server
  m_localMapService.start();
  */

    // Feature service using: sdk/samples/data/mpks/USCitiesStates.mpk
    /*
  QString localFeatureService = pathSampleData + "mpks" + QDir::separator() + "USCitiesStates.mpk";
  m_localFeatureService = EsriRuntimeQt::LocalFeatureService(localFeatureService);
  m_localFeatureService.setMaximumRecords(3000);

  // connect to signal that is emitted when the service is created successfully
  connect(&m_localFeatureService, SIGNAL(serviceCreationSuccess(const QString&, const QString&)), this, SLOT(onFeatureServiceCreationSuccess(const QString&, const QString&)));

  // connect to signal that is emitted when the service failed
  connect(&m_localFeatureService, SIGNAL(serviceCreationFailure(const QString&)), this, SLOT(onFeatureServiceCreationFailure(const QString&)));

  // start the service and the Local Server
  m_localFeatureService.start();
  */

    // Graphics Layer
    /*
  EsriRuntimeQt::Point point1(0, 0);
  EsriRuntimeQt::SimpleMarkerSymbol redCircle(Qt::red, 8, EsriRuntimeQt::SimpleMarkerSymbolStyle::Circle);
  EsriRuntimeQt::Graphic graphic1(point1, redCircle);
  m_graphicsLayer.addGraphic(graphic1);
  m_map.addLayer(m_graphicsLayer);
  */

    //// connect to signal that is emitted when the map is ready
    //// the mapReady signal is emitted when the Map has obtained a
    //// spatial reference from an added layer
    // connect(&m_map, SIGNAL(mapReady()), this, SLOT(onMapReady()));
}

AutoGPS::~AutoGPS()
{
    // stop the Local Map Service
    /*
  if(m_localMapService.status() == EsriRuntimeQt::LocalServiceStatus::Running)
     m_localMapServ67yy7ice.stopAndWait();
  */

    // stop the Local Feature Service
    /*
  if(m_localFeatureService.status() == EsriRuntimeQt::LocalServiceStatus::Running)
    m_localFeatureService.stopAndWait();
  */

    // stop the Local Server Instance for local service
    /*
  if (EsriRuntimeQt::LocalServer::instance().isRunning())
    EsriRuntimeQt::LocalServer::instance().shutdownAndWait();
  */

    //// disconnect signals for Local Map Service
    //disconnect(&m_localMapService, SIGNAL(serviceCreationSuccess(const QString&, const QString&)), this, SLOT(onLocalServiceCreationSuccess(const QString&, const QString&)));
    //disconnect(&m_localMapService, SIGNAL(serviceCreationFailure(const QString&)), this, SLOT(onLocalServiceCreationFailure(const QString&)));

    //// disconnect signals for Feature Service
    //disconnect(&m_localFeatureService, SIGNAL(serviceCreationSuccess(const QString&, const QString&)), this, SLOT(onFeatureServiceCreationSuccess(const QString&, const QString&)));
    //disconnect(&m_localFeatureService, SIGNAL(serviceCreationFailure(const QString&)), this, SLOT(onFeatureServiceCreationFailure(const QString&)));

    // disconnect signal for Map
    //disconnect(&m_map, SIGNAL(mapReady()), this, SLOT(onMapReady()));

    m_map.dispose();
    delete m_mapGraphicsView;
}

/*
void AutoGPS::onMapReady()
{
  // set the map extent
  //m_map.setExtent(EsriRuntimeQt::Envelope(xmin, ymin, xmax, ymax));

  //// Feature Layer
  //// Initialize the FeatureLayer with a GeodatabaseFeatureServiceTable after the Map has
  //// obtained a spatial reference so its features can be projected correctly

  //// specify the URL to create a Service Info Task to get a specific layer by name or id
  //EsriRuntimeQt::ServiceInfoTask serviceTaskInfo("http://sampleserver1.arcgisonline.com/ArcGIS/rest/services/Demographics/ESRI_Census_USA/MapServer/");
  //EsriRuntimeQt::FeatureServiceInfo featureServiceInfo = serviceTaskInfo.createFeatureServiceInfo();
  //QList<EsriRuntimeQt::FeatureLayerInfo> layerInfoList= featureServiceInfo.layers("states");

  //if(layerInfoList.size() > 0 && !layerInfoList.at(0).url().isEmpty())
  //{
  //  // give the feature service table the URL to the feature service layer
  //  EsriRuntimeQt::GeodatabaseFeatureServiceTable featureServiceTable(layerInfoList.at(0).url());
  //  // initialize the feature layer with the feature service table as its feature source
  //  m_featureLayer = EsriRuntimeQt::FeatureLayer(featureServiceTable);
  //  m_map.addLayer(m_featureLayer);
  //}
}
*/

/*
void AutoGPS::onLocalServiceCreationSuccess(const QString& url, const QString& name)
{
  Q_UNUSED(url);
  Q_UNUSED(name);

   // create the ArcGISDynamicMapServiceLayer using the LocalMapService's url
   m_dynamicLocalServiceLayer = EsriRuntimeQt::ArcGISDynamicMapServiceLayer(m_localMapService.urlMapService());
   m_map.addLayer(m_dynamicLocalServiceLayer);
}
*/

/*
void AutoGPS::onLocalServiceCreationFailure(const QString& name)
{
  qWarning() << name + " failed to start";
  qWarning() << m_localMapService.error().what();
}
*/

/*
void AutoGPS::onFeatureServiceCreationSuccess(const QString& url, const QString& name)
{
  Q_UNUSED(url);
  Q_UNUSED(name);

  QString serviceUrl;
  EsriRuntimeQt::FeatureServiceInfo featureServiceInfo = m_localFeatureService.featureServiceInfo();
  QList<EsriRuntimeQt::FeatureLayerInfo> layerInfoList= featureServiceInfo.layers("Cities");

  if(layerInfoList.size() > 0)
    serviceUrl = layerInfoList.at(0).url();

  if (serviceUrl.isEmpty())
  {
    qWarning() << "The required layer does not exist";
    return;
  }

  m_localFeatureLayer = EsriRuntimeQt::ArcGISFeatureLayer(serviceUrl);
  m_map.addLayer(m_localFeatureLayer);
}
*/

/*
void AutoGPS::onFeatureServiceCreationFailure(const QString& name)
{
  qWarning() << name + " failed to start";
  qWarning() << m_localFeatureService.error().what();
}
*/

void AutoGPS::createActions()
{
    newAct = new QAction(QIcon(":/images/new.png"), QStringLiteral("新建项目"), this);
    newAct->setShortcuts(QKeySequence::New);
    newAct->setStatusTip(QStringLiteral("新建项目"));
    connect(newAct, SIGNAL(triggered()), this, SLOT(newFile()));
    openAct = new QAction(QIcon(":/images/open.png"), QStringLiteral("打开项目"), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip( QStringLiteral("打开项目"));
    connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

    saveAct = new QAction(QIcon(":/images/save.png"), QStringLiteral("保存项目"), this);
    saveAct->setShortcuts(QKeySequence::Save);
    saveAct->setStatusTip(QStringLiteral("保存项目"));
    connect(saveAct, SIGNAL(triggered()), this, SLOT(save()));

    saveAsAct = new QAction(QStringLiteral("保存项目到..."), this);
    saveAsAct->setShortcuts(QKeySequence::SaveAs);
    saveAsAct->setStatusTip(QStringLiteral("保存项目到..."));
    connect(saveAsAct, SIGNAL(triggered()), this, SLOT(saveAs()));


    exitAct = new QAction(QStringLiteral("退出"), this);
    exitAct->setShortcuts(QKeySequence::Quit);
    exitAct->setStatusTip(QStringLiteral("退出程序"));
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));



    aboutAct = new QAction(QStringLiteral("&关于AutoGPS"), this);
    aboutAct->setStatusTip(QStringLiteral("关于AutoGPS"));
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

}



void AutoGPS::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newAct);
    fileMenu->addAction(openAct);
    fileMenu->addAction(saveAct);
    fileMenu->addAction(saveAsAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);


    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAct);

}


void AutoGPS::createToolBars()
{
    fileToolBar = addToolBar(tr("File"));
    fileToolBar->setContentsMargins(10, 10, 10, 10);

    fileToolBar->addAction(newAct);

    fileToolBar->addAction(openAct);
    fileToolBar->addAction(saveAct);
}


void AutoGPS::createStatusBar()
{
    statusBar()->showMessage(tr("Ready"));
}

bool AutoGPS::maybeSave()
{
    return true;
}

void AutoGPS::readSettings()
{
    QSettings settings("AutoGPSProject", "AutoGPS");
    QPoint pos = settings.value("pos", QPoint(200, 200)).toPoint();
    QSize size = settings.value("size", QSize(400, 400)).toSize();
    resize(size);
    move(pos);
}

void AutoGPS::writeSettings()
{
    QSettings settings("AutoGPSProject", "AutoGPS");
    settings.setValue("pos", pos());
    settings.setValue("size", size());
}


void AutoGPS::closeEvent(QCloseEvent *event)
{
    if (maybeSave()) {
        writeSettings();
        event->accept();
    } else {
        event->ignore();
    }
}

void AutoGPS::createDockWidget()
{
    dockWidget = new QDockWidget(this);
    dockWidget->setMinimumWidth(200);
    dockWidget->setWindowTitle(QStringLiteral("解决方案资源管理器"));
    dockWidget->hide();
    this->addDockWidget(Qt::RightDockWidgetArea, dockWidget);

    treeView = new QTreeView(dockWidget);

    model = new QDirModel(this);
//    treeView->setHeaderLabel(QStringLiteral("解决方案"));
   // treeView->setHeader();
    treeView->setModel(model);
    treeView->setColumnWidth(0, 150);
    treeView->resizeColumnToContents(1);
    treeView->setColumnHidden(1, true);
    treeView->setColumnHidden(2, true);
    treeView->setColumnHidden(3, true);
    treeView->setColumnHidden(4, true);
    dockWidget->setWidget(treeView);


}

void AutoGPS::newFile()
{

}

void AutoGPS::open()
{
        QString appPath = QCoreApplication::applicationDirPath();
        QString dirName = QFileDialog::getExistingDirectory(this, QStringLiteral("打开项目文件夹"), appPath );
        if (!dirName.isEmpty())
        {
            loadDir(dirName);
        }
}

void AutoGPS::about()
{

}

bool AutoGPS::saveAs()
{
    return true;
}

bool AutoGPS::saveDir(const QString &DirName)
{
    return true;
}

bool AutoGPS::save()
{
    if (curDir.isEmpty()) {
        return saveAs();
    } else {
        return saveDir( curDir);
    }
}

 void AutoGPS::setCurrentDir(const QString &DirName)
 {

 }

 QString AutoGPS::strippedName(const QString &fullDirName)
 {
     return QDir(fullDirName).dirName();
 }

 void AutoGPS::loadDir(const QString dirName)
 {

        treeView->setRootIndex(model->index(dirName));
        dockWidget->show();

        QDir dir(dirName);
        QStringList list = dir.entryList(QStringList()<<"*.tpk", QDir::Files, QDir::Time);
        if (list.size() == 0)
        {
            QMessageBox::information(this, QStringLiteral("缺失"), QStringLiteral("无法找到.tpk文件"));
        }
        QString tiledBaseMapLayer = dir.path() + QDir::separator() + list.at(0);
        m_tiledLayer = EsriRuntimeQt::ArcGISLocalTiledLayer(tiledBaseMapLayer);
        m_map.addLayer(m_tiledLayer);
 }
