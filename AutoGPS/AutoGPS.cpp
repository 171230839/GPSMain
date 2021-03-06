#include "AutoGPS.h"
#include <QApplication>

//#include <ArcGISTiledMapServiceLayer.h>
//#include <QtDeclarative/QDeclarativeItem>
#include <QtDeclarative/QDeclarativeComponent>
#include <QtDeclarative/QDeclarativeContext>
#include <QMessageBox>
#include <QGraphicsLinearLayout>
#include <QtDeclarative/QDeclarativeEngine>
#include <QGraphicsWidget>
//#include <MapGraphicsView.h>

static const QString UI_OVERLAY_PATH("qrc:/Resources/qml/MainOverlay.qml");

AutoGPS::AutoGPS (QWidget *parent):
    QMainWindow(parent),
    mapGraphicsView(NULL),
    mapController(NULL),
    engine(NULL),
    overlayWidget(NULL),
    overlayUI(NULL),
    mainMenuUI(NULL)
{

    this->setWindowFlags(Qt::CustomizeWindowHint);
    this->showMaximized();
    EsriRuntimeQt::ArcGISRuntime::setRenderEngine(EsriRuntimeQt::RenderEngine::OpenGL);

    mapGraphicsView = EsriRuntimeQt::MapGraphicsView::create(map, this);
    if (!mapGraphicsView)
    {
        qCritical() << "Unable to create map.";
        return;
    }
    map.setWrapAroundEnabled(false);
    map.setEsriLogoVisible(false);
    this->setCentralWidget(mapGraphicsView);
    QString tpkPath = QApplication::applicationDirPath() + QDir::separator() + "AutoGPS.tpk";
    tiledLayer.setPath(tpkPath);
    tiledLayer.setName("tiledLayer");
    map.addLayer(tiledLayer);
    //    ArcGISTiledMapServiceLayer tiledLayer("http://services.arcgisonline.com/ArcGIS/rest/services/NatGeo_World_Map/MapServer");
    //    map.addLayer(tiledLayer);
    //    ArcGISLocalTiledLayer tiledLayer("D:/Qt/QtSampleApplication_10.2.3_win32/sdk/samples/data/tpks/Topographic.tpk");
    //    map.addLayer(tiledLayer);

    mapController = new MapController(&map, mapGraphicsView, this);
    mapController->init();

    engine = new QDeclarativeEngine(this);
    context = new QDeclarativeContext(engine->rootContext());
    context->setContextProperty("serialPortThread", &thread);
    //      connect(&thread, SIGNAL(portListChanged), this, SLOT(onPortListChanged));
    //      qDebug()<<"portList()"<<QVariant::fromValue(thread.portList());
    //      context->setContextProperty("serialPortNo", QVariant::fromValue(thread.portList()));

    QDeclarativeComponent component(engine, QUrl(UI_OVERLAY_PATH), engine);
    overlayUI = component.create(context);
    //    thread.setContext(context);
    if (!overlayUI)
    {
        qDebug() << "Failed to load UI overlay";
        qDebug() << component.errorString();
        QMessageBox box;
        box.setText("Error:\n" + component.errorString());
        box.exec();
        return;
    }



    // Create a container for the UML UI and add it to the scene
    overlayWidget = new QGraphicsWidget();
    QGraphicsLinearLayout* layout = new QGraphicsLinearLayout(overlayWidget);
    layout->setContentsMargins(0, 0, 0, 0);
    QGraphicsLayoutItem* qmlUILayout = qobject_cast<QGraphicsLayoutItem*>(overlayUI);
    layout->addItem(qmlUILayout);
    overlayWidget->setLayout(layout);
    mapGraphicsView->scene()->addItem(overlayWidget);

    connect(overlayUI, SIGNAL(homeClicked()), mapController, SLOT(handleHomeClicked()));
    connect(overlayUI, SIGNAL(zoomInClicked()), mapController, SLOT(handleZoomIn()));
    connect(overlayUI, SIGNAL(zoomOutClicked()), mapController, SLOT(handleZoomOut()));
    connect(overlayUI, SIGNAL(panClicked(QString)), mapController, SLOT(handlePan(QString)));
    connect(mapController, SIGNAL(positionChanged(QVariant)), overlayUI, SLOT(updateLocation(QVariant)));
    connect(mapController, SIGNAL(speedChanged(QVariant)), overlayUI, SLOT(updateSpeed(QVariant)));
    connect(mapController, SIGNAL(headingChanged(QVariant)), overlayUI, SLOT(updateHeading(QVariant)));
    connect(&thread, SIGNAL(positionChanged(QVariant)), overlayUI, SLOT(updateLocation(QVariant)));
    connect(&thread, SIGNAL(avaliblePosition(double, double, double)), mapController, SLOT(onAvaliblePosition(double, double, double)));
    connect(&thread, SIGNAL(timeChanged(QVariant)), overlayUI, SLOT(updateTime(QVariant)));
    connect(&thread, SIGNAL(speedChanged(QVariant)), overlayUI, SLOT(updateSpeed(QVariant)));
    connect(&thread, SIGNAL(headingChanged(QVariant)), overlayUI, SLOT(updateHeading(QVariant)));
    connect(overlayUI, SIGNAL(basemapChanged(QString)), this, SLOT(handleBasemapChanged(QString)));

    mainMenuUI = overlayUI->findChild<QObject*>("mainMenu");
    if (mainMenuUI)
    {
        connect(mainMenuUI, SIGNAL(exitClicked()), this, SLOT(close()));
    }

    QObject * serialConfig = overlayUI->findChild<QObject*>("serialConfig");
    if (serialConfig)
    {
        connect(serialConfig, SIGNAL(readyOpenSerialPort(QVariant)), &thread, SLOT(onReadyOpenSerialPort(QVariant)));
    }

    QObject *config = overlayUI->findChild<QObject*>("config");
    if (config)
    {
        connect(config, SIGNAL(showMeToggled(bool)), mapController, SLOT(handleToggleShowMe(bool)));
        connect(config, SIGNAL(followMeToggled(bool)), mapController, SLOT(handleToggleFollowMe(bool)));
        connect(config, SIGNAL(resetMapClicked()), mapController, SLOT(handleResetMap()));
    }
    QObject *geometryPanel = overlayUI->findChild<QObject*>("geometryPanel");
    if (geometryPanel)
    {
        connect(geometryPanel, SIGNAL(pointsToggled(bool)), mapController, SLOT(handlePointsToggled(bool)));
        connect(geometryPanel, SIGNAL(toLinesClicked()), mapController, SLOT(handleToLinesClicked()));
        connect(geometryPanel, SIGNAL(okClicked()), mapController, SLOT(handleOkClicked()));
        connect(geometryPanel, SIGNAL(toPolygonClicked()), mapController, SLOT(handleToPolygonClicked()));
        connect(geometryPanel, SIGNAL(clearClicked()), mapController, SLOT(onClearClicked()));
    }

    QObject *workerPanel = overlayUI->findChild<QObject*>("workerPanel");
    if (workerPanel)
    {
        //        connect(workerPanel, SIGNAL(selectPointToggled(bool)), mapController, SLOT(handleSelectPointToggled(bool)));
    }
    QObject *pathsPanel = overlayUI->findChild<QObject*>("pathsPanel");
    if (pathsPanel)
    {
        connect(pathsPanel, SIGNAL(selectPointToggled(bool)), mapController, SLOT(handleSelectPointToggled(bool)));
        connect(pathsPanel, SIGNAL(getPathClicked()), mapController, SLOT(handleGetPathClicked()));
              connect(pathsPanel, SIGNAL(unSelectClicked()), mapController, SLOT(handleUnSelectClicked()));
    }

    //    thread.init();
    //    connect(&thread, SIGNAL(response(QString)), mapController, SLOT(onResponse(QString)));
    //    connect(&thread, SIGNAL(timeout(QString)), mapController, SLOT(onTimeout(QString)));
    //    connect(&thread, SIGNAL(error(QString)), mapController, SLOT(onError(QString)));
    thread.start();

    connect(&map, SIGNAL(mapReady()), mapController, SLOT(onMapReady()));

    QTimer* timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateNorthArrow()));
    timer->start(1000 / 24);

    // hook up the handler for the mouse click
    connect(&map, SIGNAL(mousePress(QMouseEvent)), mapController, SLOT(mousePress(QMouseEvent)));

}

AutoGPS::~AutoGPS()
{
}

void AutoGPS::updateNorthArrow()
{
    QMetaObject::invokeMethod(overlayUI, "updateMapRotation", Q_ARG(QVariant, map.rotation()));
}

void AutoGPS::resizeEvent(QResizeEvent* event)
{
    if (map.isInitialized())
    {
        QRectF sceneRect = mapGraphicsView->sceneRect();
        overlayWidget->setGeometry(sceneRect);
    }
    QMainWindow::resizeEvent(event);
}

void AutoGPS::closeEvent(QCloseEvent *event)
{

    thread.storeSerialConfig();
    event->accept();
}

void AutoGPS::handleBasemapChanged(QString basemap)
{
    if (basemap == "first")
    {
        setBasemapFirst();
    }
    else if ( basemap == "second")
    {
        setBasemapSecond();
    }
    else if (basemap == "third")
    {
        setBasemapThird();
    }
}

void AutoGPS::setBasemapFirst()
{
    QStringList layerNames = map.layerNames();
    if (!layerNames.contains("tiledLayer"))
    {
        map.addLayer(this->tiledLayer);
    }
}

void AutoGPS::setBasemapSecond()
{
    QStringList layerNames = map.layerNames();
    if (layerNames.contains("tiledLayer"))
    {
        map.removeLayer("tiledLayer");
    }
}

void AutoGPS::setBasemapThird()
{

}
