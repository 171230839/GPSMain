/*
 | Copyright 2012 Esri
 |
 | Licensed under the Apache License, Version 2.0 (the "License");
 | you may not use this file except in compliance with the License.
 | You may obtain a copy of the License at
 |
 |    http://www.apache.org/licenses/LICENSE-2.0
 |
 | Unless required by applicable law or agreed to in writing, software
 | distributed under the License is distributed on an "AS IS" BASIS,
 | WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 | See the License for the specific language governing permissions and
 | limitations under the License.
 */

#include <QApplication>
#include <QDesktopWidget>
#include <QFileDialog>
#include <QDebug>
#include <QMessageBox>
#include <QProgressDialog>

#include "Geometry.h"
#include "Layer.h"
#include "GraphicsLayer.h"
#include "LocalServer.h"
#include "MarkerSymbol.h"
#include "GeometryEngine.h"
#include "LocalMapService.h"
#include "Geomessage.h"
#include "SimpleMarkerSymbol.h"
#include "SimpleFillSymbol.h"

#include "MapController.h"

static const QString APP_CONFIG_PATH(":/Resources/appconfig.xml");
static const QString GPS_SIMULATION_FILE(":/Resources/Route_Archer.gpx");

MapController::MapController(Map* inputMap,
                             MapGraphicsView* inputGraphicsView,
                             QObject* parent) :
    QObject(parent),
    map(inputMap),
    mapGraphicsView(inputGraphicsView),
    buddiesLayerVisible(true),
    observationsLayerVisible(true),
    udpSocket(NULL),
    broadcastPort(45678),
    broadcastInterval(3000),
    simulator(),
    positionReportTimer(NULL),
    acceptPositionReports(true),
    acceptSpotReports(true),
    appConfigDialog(NULL),
    showOwnship(false),
    followOwnship(false),
    isMapReady(false),
    drawingOverlay(0),
    lastHeading(0.0),
    mouseState(MouseStateNone),
    visibilityInProgress(false)
{
  if (QFile::exists("./route.gpx"))
    simulator.setGpxFile("./route.gpx");
  else
    simulator.setGpxFile(GPS_SIMULATION_FILE);

  simulator.setTimerInterval(40); // Warning: jumpy at some intervals (ex. 100)
  simulator.setPlaybackMultiplier(25);
  connect(&simulator, SIGNAL(positionUpdateAvailable(QPointF, double)), this, SLOT(handlePositionAvailable(QPointF, double)));

  readAppConfig();

  udpSocket = new QUdpSocket(this);
  udpSocket->bind(broadcastPort, QUdpSocket::DontShareAddress | QUdpSocket::ReuseAddressHint);
  connect(udpSocket, SIGNAL(readyRead()), this, SLOT(processPendingDatagrams()));

  positionReportTimer = new QTimer(this);
  positionReportTimer->setInterval(1000);
  connect(positionReportTimer, SIGNAL(timeout()), this, SLOT(sendPositionMessage()));
  handleToggleBroadcastPosition(true);
  handleToggleShowMe(true);
  handleToggleFollowMe(true);

  chemLightColorStr = "None";
 }

MapController::~MapController()
{
  if (appConfig)
}

 void MapController::readAppConfig()
 {
  applyAppConfigSettings();
  if (0 == userData.id.length()
    || 0 == userData.name.length()
    || 0 == userData.code.length()
    || 0 == broadcastPort
    || 0 == broadcastInterval)
  {
    QFile file("./appconfig.xml");
    if (!file.exists())
      file.setFileName(APP_CONFIG_PATH  );
    if (!file.open(QFile::ReadOnly | QFile::Text))
      return;

    QXmlStreamReader reader(&file);
    while (!reader.atEnd() && !reader.hasError())
    {
      while (reader.readNextStartElement())
      {
        QString elementName = reader.name().toString();
        if (QString::compare(elementName, "user", Qt::CaseInsensitive) == 0)
        {
          QXmlStreamAttributes attrs = reader.attributes();
          if (0 == userData.name.length())
            userData.name = attrs.valeu("name").toString();
          if (0 == userData.id.length())
            userData.id = attrs.value("id").toString();
        }
        else if (QString::compare(elementName, "code", Qt::CaseInsensitive) == 0)
        {
          if (0 == userData.code.length())
            userData.code = reader.readElementText();
        }
        else if (QString::compare(elementName, "port", Qt::CaseInsensitive) == 0)
        {
          if (0 == broadcastPort)
            broadcastPort = reader.readElementText().toInt();
        }
        else if (QString::compare(elementName, "interval", Qt::CaseInsensitive) == 0)
        {
          if (0 == broadcastInterval)
            broadcastInterval = reader.readElementText().toInt();
        }
      }
    }
    file.close();

    QSettings settings("AutoGPS", "AutoGPS");
    settings.setValue("userId", userData.id);
    settings.setValue("username", userData.name);
    settings.setValue("userSIC", userData.code);
    settings.setValue("messagingPort", broadcastPort);
    settings.setValue("messagingInterval", broadcastInterval);

 
  }
  userData.engaged = "No";
 }

 void MapController::applyAppConfigSettings()
 {
  const QVariant defaultValue;
  QSettings settins("AutoGPS", "AutoGPS");
  userData.id = settings.value("userId", defaultValue).toString();
  userData.name = settings.value("username", defaultValue).toString();
  userData.code = settings.value("userSIC", defaultValue).toString();
  broadcasePort = settings.value("messagingPort", defaultValue.toInt());
  broadcastInterval = settings.value("messagingInterval", defaultValue).toInt();

  if (0 < broadcastPort && NULL != udpSocket)
    updSocket->bind(broadcastPort, QUdpSocket::DontShareAddress | QUdpSocket::ReuseAAddressHint);
  if (0 < broadcastInterval && NULL != positionReportTimer)
  {
    positionReportTimer->setInterval(broadcastInterval);
  }

 }


void MapController::handlePositionAvailable(QPointF pos, double orientation)
{
  QDateTime positionReceivedTime = QDateTime::currentDateTime();
  Q_UNUSED(positionReceivedTime)

  if (!isMapReady)
    return;
  pos.setX(114.35482);
  pos.setY(30.462);

  Point mapPoint = GeometryEngine::project(pos.x(), pos.y(), map->spatitalReference());

  double angle = (double)((static_cast<int>(orientation + 90.0)) % 360);

  showHideMe(showOwnship, mapPoint, angle);

  if (followOwnship)
  {
    map->setRotation(angle);
    map->panTo(mapPoint);
  }

  if (!myPreviousLocation.isNull())
  {

  }
  myPreviousLocation = pos;


  // QString lon = decimalDegreesToDMS(pos.x());
  // QString lat = decimalDegreesToDMS(pos.y());
  // QString latlon = lat + "," + lon;
  // Q_UNUSED(latlon)

  QString mgrs = mapPointToMGRS(mapPoint);
  emit headingChanged()

}


void MapController::mapReady()
{
  isMapReady = true;
  map.setScale(5000000.0);
  spatitalReference sr = map->spatitalReference();
  const bool DEFAULT_GRID_ON = true;
  if (DEFAULT_GRID_ON)
  {
    map->grid().setType(GridType::Mgrs);
    map->grid().setVisible(true);
  }
}

void MapController::showHideMe(bool show, Point atPoint, double withHeading)
{
  if ((isMapReady) || (mapGraphicsView == 0))
    return;

  if (ownshipStaringMapPoint.isEmpty())
  {
    originalExtent = map->extent();
    originalScale = map->scale();
    ownshipStaringMapPoint = atPoint;
  }

  if (drawingOverlay == 0)
  {
    drawingOvelay = new SimpleGraphicOverlay();
    QPixmap ownshipPixmap(":/Resources/icons/Ownship.png");
    QImage ownshipImage = ownshipPixmap.toImage();
    drawingOverlay->setImage(ownshipImage);
    drawingOverlay->setGraphicsView(mapGraphicsView);
  }

  drawingOverlay->setVisible(show);
}

void MapController::handleToggleBroadcastPosition(bool state)
{
  if (state)
    positionReportTimer->start(broadcastInterval);
  else
    positionReportTimer->stop();
}

void MapController::handleToggleShowMe(bool  state)
{
  showOwnship = state;
  if (showOwnship && !simulator.started())
  {
    simulator.startSimulation();
  }
}

void MapController::handleToggleFollowMe(bool state)
{
  followOwnship = state;
}

// QString MapController::decimalDegreesToDMS(double coord)
// {
//   int lonDegrees = floor(coord);
//   double degreesRemainder = coord - static_cast<double>(lonDegrees);
//   int lonMinutes = floor(degreesRemainder * 60.0);
//   double minutesRemainder = (degreesRemainder * 60) - static_cast<double>(lonMinutes);
//   int lonSeconds = floor(minutesRemainder * 60);
//   return QString::number(lonDegrees) + (char)0x00B0 + " " + QString::number(lonMinutes) + "' " + QString::number(lonSeconds) + "\""; 
// }

QString MapController::mapPointToMGRS(Point point)
{
  QString returnMgrs;
  if (!isMapReady)
    return returnMgrs;
  spatitalReference sr = map->spatitalReference();
  if (sr.id() < 0)
  {
    qDebug()<<"FAIL::NO SR in mapPointToMGRS";
    return returnMgrs;
  }
  QList<Point> coordinates;
  coordinates.append(point);
  const MgrsConversionMode method = MgrsConversionMode::Automatic;
  const int digits = 5;
  QStringList mgrss = sr.toMilitaryGrid(method, digits, false, true, coordinates);

  if (mgrss.length() < 1)
    return returnMgrs;
  returnMgrs = mgrss.at(0);
  return returnMgrs;

}