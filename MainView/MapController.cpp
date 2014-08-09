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
    qDebug()<<"map controller.cpp"<<mapGraphicsView;
    simulator.setTimerInterval(40); // Warning: jumpy at some intervals (ex. 100)
    simulator.setPlaybackMultiplier(25);
    connect(&simulator, SIGNAL(positionUpdateAvailable(QPointF, double)), this, SLOT(handlePositionAvailable(QPointF, double)));

    readAppConfig();

    udpSocket = new QUdpSocket(this);
    udpSocket->bind(broadcastPort, QUdpSocket::DontShareAddress | QUdpSocket::ReuseAddressHint);
    //    connect(udpSocket, SIGNAL(readyRead()), this, SLOT(processPendingDatagrams()));

    positionReportTimer = new QTimer(this);
    positionReportTimer->setInterval(1000);
    //    connect(positionReportTimer, SIGNAL(timeout()), this, SLOT(sendPositionMessage()));
    handleToggleBroadcastPosition(true);
    handleToggleShowMe(true);
    handleToggleFollowMe(true);

    chemLightColorStr = "None";
}

MapController::~MapController()
{
    if (appConfigDialog != NULL)
        delete appConfigDialog;
    if (drawingOverlay != NULL)
        delete drawingOverlay;
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
                        userData.name = attrs.value("name").toString();
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
    QSettings settings("AutoGPS", "AutoGPS");
    userData.id = settings.value("userId", defaultValue).toString();
    userData.name = settings.value("username", defaultValue).toString();
    userData.code = settings.value("userSIC", defaultValue).toString();
    broadcastPort = settings.value("messagingPort", defaultValue).toInt();
    broadcastInterval = settings.value("messagingInterval", defaultValue).toInt();

    if (0 < broadcastPort && NULL != udpSocket)
        udpSocket->bind(broadcastPort, QUdpSocket::DontShareAddress | QUdpSocket::ReuseAddressHint);
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

    Point mapPoint = GeometryEngine::project(pos.x(), pos.y(), map->spatialReference());

    if (0) // if debug simulation data
    {
        qDebug() << "Simulator Data: " << QString::number(pos.x()) << ", " << QString::number(pos.y()) << ", Orientation: " << orientation;
        qDebug() << "         Delta: " << QString::number(mapPoint.x()) << ", " << QString::number(mapPoint.y());
    }
    double angle = (double)((static_cast<int>(orientation + 90.0)) % 360);

    showHideMe(showOwnship, mapPoint, angle);

    if (followOwnship)
    {
        map->setRotation(angle);
        map->panTo(mapPoint);
    }

    if (!myPreviousLocation.isNull())
    {
        // calculate speed here if desired


        emit speedChanged(QVariant(""));
    }
    myPreviousLocation = pos;


    QString lon = decimalDegreesToDMS(pos.x());
    QString lat = decimalDegreesToDMS(pos.y());
    QString latlon = lat + "," + lon;
    //     Q_UNUSED(latlon)


    //    QString mgrs = mapPointToMGRS(mapPoint);
    //        qDebug() << "mgrs" << mgrs;
    emit headingChanged(QVariant(QString::number(angle) + (char)0x00B0));
    emit positionChanged(QVariant(latlon));


}


void MapController::mapReady()
{
    isMapReady = true;
    map->setScale(5000000.0);

    //below no use
    SpatialReference sr = map->spatialReference();
    qDebug() << "MapReady, Spatial Reference = " << sr.id();
    const bool DEFAULT_GRID_ON = true;
    // if (DEFAULT_GRID_ON)
    if (false)
    {
        map->grid().setType(GridType::Mgrs);
        map->grid().setVisible(true);
    }
}

void MapController::showHideMe(bool show, Point atPoint, double withHeading)
{

    if ((!isMapReady) || (mapGraphicsView == 0))
        return;

    if (ownshipStartingMapPoint.isEmpty())
    {
        originalExtent = map->extent();
        originalScale = map->scale();
        ownshipStartingMapPoint = atPoint;
    }

    if (drawingOverlay == 0)
    {
        drawingOverlay = new SimpleGraphicOverlay();
        QPixmap ownshipPixmap(":/Resources/icons/Ownship.png");
        QImage ownshipImage = ownshipPixmap.toImage();
        drawingOverlay->setImage(ownshipImage);
        drawingOverlay->setGraphicsView(mapGraphicsView);
    }

    drawingOverlay->setVisible(show);
    if (show)
    {
        drawingOverlay->setPosition(atPoint);
        drawingOverlay->setAngle(withHeading);
    }

    lastOwnshipPoint = atPoint;
    lastHeading = withHeading;
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
        qDebug()<<"simulator start ";
        simulator.startSimulation();
    }
}

void MapController::handleToggleFollowMe(bool state)
{
    followOwnship = state;
}

QString MapController::decimalDegreesToDMS(double coord)
{
    int lonDegrees = floor(coord);
    double degreesRemainder = coord - static_cast<double>(lonDegrees);
    int lonMinutes = floor(degreesRemainder * 60.0);
    double minutesRemainder = (degreesRemainder * 60) - static_cast<double>(lonMinutes);
    int lonSeconds = floor(minutesRemainder * 60);
    return QString::number(lonDegrees) + (char)0x00B0 + " " + QString::number(lonMinutes) + "' " + QString::number(lonSeconds) + "\"";
}

QString MapController::mapPointToMGRS(Point point)
{
    QString returnMgrs;
    if (!isMapReady)
        return returnMgrs;
    SpatialReference sr = map->spatialReference();
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

void MapController::processPendingDatagrams()
{
    while (udpSocket->hasPendingDatagrams())
    {
        QByteArray datagram;
        datagram.resize(udpSocket->pendingDatagramSize());
        udpSocket->readDatagram(datagram.data(), datagram.size());
        QString strMessage(datagram.data());
        if (filterMessages(strMessage))
            continue;
        setMessageStream(strMessage);
    }
}

bool MapController::filterMessages(const QString& strMessages)
{
    QXmlStreamReader reader(strMessages);
    QXmlStreamAttributes attrs;

    while (reader.readNextStartElement() && !reader.atEnd() && !reader.hasError())
    {
        attrs = reader.attributes();
        if (QStringRef::compare(reader.name(), "geomessage", Qt::CaseInsensitive) == 0)
        {
            attrs = reader.attributes();
            if (QStringRef::compare(attrs.value("id"), userData.id) == 0)
                return true;

            /////////  less use //////////
            // Next filter out unsupported message types
        }
    }
    return false;
}

//void MapController::setMessageStream(QFile& file)
//{
//    file.open(QFile::ReadOnly | QFile::Text);
//    QXmlStreamReader reader(&file);
//    setMessageStream(reader);
//}

void MapController::setMessageStream(const QString& stream)
{
    QXmlStreamReader reader(stream);
    setMessageStream(reader);
}


void MapController::setMessageStream(QXmlStreamReader& reader)
{
    while (!reader.atEnd() && !reader.hasError())
    {
        if (reader.isStartElement())
        {
            if ((QStringRef::compare(reader.name(), "geomessages", Qt::CaseInsensitive) == 0) ||
                    (QStringRef::compare(reader.name(), "messages", Qt::CaseInsensitive) ==0))
            {
                if (!readMessages(reader))
                {
                    readToElementEnd(reader);
                    continue;
                }

            }
            else
            {
                skipUnknownElement(reader);
            }
        }
        else
        {
            reader.readNext();
        }
    }
}

bool MapController::readMessages(QXmlStreamReader& reader)
{
    reader.readNext();
    while (!reader.atEnd())
    {
        //?? below like no use//
        if (reader.isEndElement())
        {
            reader.readNext();
            break;
        }
        //??//
        if (reader.isStartElement())
        {
            if ((QStringRef::compare(reader.name(), "geomessage", Qt::CaseInsensitive) == 0) ||
                    (QStringRef::compare(reader.name(), "message", Qt::CaseInsensitive) == 0))
            {
                if (!readMessage(reader))
                {
                    readToElementEnd(reader);
                    continue;
                }
            }
            else
            {
                skipUnknownElement(reader);
            }
        }
        else
        {
            reader.readNext();
        }
    }
    return true;
}

void MapController::readToElementEnd(QXmlStreamReader& reader)
{
    reader.readNext();
    while (!reader.atEnd())
    {
        if (reader.isEndElement())
        {
            reader.readNext();
            break;
        }
        reader.readNext();
    }
}

bool MapController::readMessage(QXmlStreamReader& reader)
{
    if (!isMapReady)
        return false;

    QVariantMap properties;
    QString name;
    QString value;
    reader.readNext();
    while(!reader.atEnd())
    {
        if (reader.isEndElement())
        {
            reader.readNext();
            break;
        }
        if (reader.isStartElement())
        {
            name = reader.name().toString();
            value = getReaderValue(reader);
            properties.insert(name, QVariant(value));
        }
        else
        {
            reader.readNext();
        }

    }
    if (properties.count() > 0)
    {
        Message message;
        // force every message type (except chemlight) to position_report for runtime
        if (QString::compare(properties["_type"].toString(), "chemlight", Qt::CaseInsensitive) == 0)
        {
            qDebug("Chemlight");
        }
        else
        {
            properties["_type"] = "position_report";
        }
        if (1)
        {
            qDebug() << "New Message Properties: ";
            for (QVariantMap::const_iterator i = properties.begin(); i != properties.end(); ++i)
            {
                QString key = i.key();
                QVariant value = i.value();
                qDebug()<<"key:"<<key<<", value:"<< value.toString();
            }
        }
        try
        {
            messageProcessor.processMessage(message);
        }
        catch (EsriRuntimeQt::Exception e)
        {
            qWarning() << "Process Message Failed: ";
            qWarning() << "Exception: " << e.what();
        }
    }
    return true;
}

void MapController::skipUnknownElement(QXmlStreamReader& reader)
{
    reader.readNext();
    while (!reader.atEnd())
    {
        if (reader.isEndElement())
        {
            reader.readNext();
            break;
        }
        if (reader.isStartElement())
        {
            skipUnknownElement(reader);
        }
        else
        {
            reader.readNext();
        }
    }
}

void MapController::sendPositionMessage()
{
    QByteArray report = createPositionReport("UPDATE");
    transmitMessages(report);
}

QByteArray MapController::createPositionReport(const QString& action)
{
    Q_UNUSED(action)
    Point location = lastOwnshipPoint;
    qDebug()<<"location -x:  "<< location.x() << "--y--: "<<location.y();
    QString strLocationControlPoint = QString("%1, %2").arg(location.x()).arg(location.y());
    double lat = location.y();
    double lon = location.x();

    QString mgrs = mapPointToMGRS(lastOwnshipPoint);

    QByteArray reportData;
    QXmlStreamWriter positionReport(&reportData);
    positionReport.writeStartDocument();
    positionReport.writeStartElement("geomessages");
    positionReport.writeStartElement("geomessage");

    positionReport.writeAttribute("id", userData.id);
    positionReport.writeTextElement("_type", "position_report");
    positionReport.writeTextElement("_action", "update");
    positionReport.writeTextElement("_id", userData.id);
    positionReport.writeTextElement("_control_points", strLocationControlPoint);
    positionReport.writeTextElement("_wkid", "3857");
    positionReport.writeTextElement("sic", userData.code);

    // TODO: evaluate these remaining properties to see if still valid
    // symbol text modifiers
    positionReport.writeTextElement("quantity", "5");
    positionReport.writeTextElement("reinforcedorreduced", "+");
    positionReport.writeTextElement("staffcomments", "Comments");
    positionReport.writeTextElement("additionalinformation", "Additional Info");
    positionReport.writeTextElement("evaluation_rating", "A1");
    positionReport.writeTextElement("combateffectiness", "Excellent");
    positionReport.writeTextElement("signatureequipment", "Gun");
    positionReport.writeTextElement("higherformation", "XX");
    positionReport.writeTextElement("iff_sif", "iff");
    positionReport.writeTextElement("uniquedesignation", userData.name);
    positionReport.writeTextElement("type", "vehicle");
    positionReport.writeTextElement("dtg", QDateTime::currentDateTimeUtc().toString("dd' 'hhmmss'Z 'MMM' 'yyyy"));
    positionReport.writeTextElement("altitude_depth", "surface");
    positionReport.writeTextElement("location", mgrs);
    positionReport.writeTextElement("speed", "75");

    // position report attributes
    positionReport.writeTextElement("message_sent_time", QDateTime::currentDateTimeUtc().toString("ddhhmm'Z 'MMM' 'yy"));
    positionReport.writeTextElement("reporting_unit", userData.id);
    positionReport.writeTextElement("position", QString::number(lat, 'g', 4) + ", " + QString::number(lon, 'g', 4));
    positionReport.writeTextElement("heading", QString::number(lastHeading));
    positionReport.writeTextElement("altitude", "100m");
    positionReport.writeTextElement("engaged", userData.engaged);
    positionReport.writeTextElement("number_and_type", "1 Vehicle");
    positionReport.writeTextElement("category", "Land");

    positionReport.writeEndElement(); // geomessage
    positionReport.writeEndElement(); // geomessages
    positionReport.writeEndDocument();

    return reportData;
}


void MapController::transmitMessages(QByteArray datagram)
{
    const bool SUPPRESS_MESSAGES = false;
    if (SUPPRESS_MESSAGES)
        return;
    if (!datagram.isEmpty() && !datagram.isNull())
        udpSocket->writeDatagram(datagram, QHostAddress::Broadcast, broadcastPort);
}


void MapController::initController()
{
    if (!map)
    {
        return;
    }

    //    if (!messageProcessor.isEmpty())
    //    {
    //        return;
    //    }

    //    messageGroupLayer = MessageGroupLayer(SymbolDictionaryType::Mil2525C);
    //    map->addLayer(messageGroupLayer);
    //    messageProcessor = messageGroupLayer.messageProcessor();
    //    dictionary = messageProcessor.symbolDictionary();
    //    QList<SymbolProperties> symbolPropList = dictionary.findSymbols();
    //    int symbolCount = symbolPropList.count();
    //    bool dictionaryWorks = (symbolCount > 0);
    //    if (!dictionaryWorks)
    //        QMessageBox::warning(mapGraphicsView, "Failure", "Dictionary Did not initialize", "Advanced Symbology will not work");

    map->setPanAnimationEnabled(false);
    //    map->addLayer(mouseClickGraphicLayer);
    map->addLayer(chemLightLayer);
    //    map->addLayer(viewshedGraphicLayer);
}

QString MapController::getReaderValue(QXmlStreamReader& reader)
{
    QString text = reader.readElementText();
    if (reader.isEndElement())
        reader.readNext();

    return text;
}

void MapController::mousePress(QMouseEvent mouseEvent)
{
    QPointF mousePoint = QPointF(mouseEvent.pos().x(), mouseEvent.pos().y());
    if (mouseEvent.button() == Qt::LeftButton)
    {
        handleMapMousePressLeft(mousePoint);
    }
    else if (mouseEvent.button() == Qt::RightButton)
    {
        handleMapMousePressRight(mousePoint);
    }
}

void MapController::handleMapMousePressLeft(QPointF mousePoint)
{
    if ((map) && (isMapReady))
    {
        Point mapPoint = map->toMapPoint(mousePoint.x(), mousePoint.y());
        previousMousePressPosScreen.setX(mousePoint.x());
        previousMousePressPosScreen.setY(mousePoint.y());
        previousMousePressPosMap.setX(mapPoint.x());
        previousMousePressPosMap.setY(mapPoint.y());
        qDebug()<<"Right Click, Map Point = " << mapPoint.x() << "," << mapPoint.y();
        if (mouseState == MouseStateMenuClicked)
            mouseState = MouseStateWaitingForMapPoint;
        else if (mouseState == MouseStateWaitingForMapPoint)
        {
                qDebug()<<"mousestate waiting for map point" << chemLightColorStr;
//            if (chemLightColorStr == "None")
//                returnPoint(previousMousePressPosMap);
//            else
                sendChemLightMessage(previousMousePressPosMap);
        }
    }
}

void MapController::handleMapMousePressRight(QPointF mousePoint)
{
    if ((map) && (isMapReady))
    {
        Point mapPoint = map->toMapPoint(mousePoint.x(), mousePoint.y());
        previousMousePressPosScreen.setX(mousePoint.x());
        previousMousePressPosScreen.setY(mousePoint.y());

        previousMousePressPosMap.setX(mapPoint.x());
        previousMousePressPosMap.setY(mapPoint.y());
        qDebug() << "Left Click, Map Point =" << mapPoint.x() << ", " << mapPoint.y();
        QList<Layer> layers = map->layers();

        //        foreach (Layer layer, layers)
        //        {
        //            QString name = layer.name();
        //            LayerType layerType  = layer.type();
        //            LayerStatus status = layer.status();
        //            qDebug() << "Layer Name: " << name << ", Type: " << int(layerType);
        //            }
    }
}

void MapController::handleHomeClicked()
{
    if (!ownshipStartingMapPoint.isEmpty())
    {
        handleToggleFollowMe(false);
        map->setExtent(originalExtent);
        map->setScale(originalScale);
        map->setRotation(0);
        map->panTo(ownshipStartingMapPoint);
    }
}

void MapController::handleChemLightSelected(QString color)
{
    mouseState = MouseStateMenuClicked;
    chemLightColorStr = color;
}

void MapController::sendChemLightMessage(Point pos)
{
    qDebug() << "sendChemLightMessage";
    Qt::GlobalColor chemLightColor = Qt::green;

    if (chemLightColorStr == "red")
        chemLightColor = Qt::red;
    else
        if (chemLightColorStr == "blue")
            chemLightColor = Qt::blue;
    if (chemLightColorStr == "yellow")
        chemLightColor = Qt::yellow;

    SimpleMarkerSymbol smsSymbol(chemLightColor, 20, SimpleMarkerSymbolStyle::Circle);
    Graphic mouseClickGraphic(pos, smsSymbol);

    int id = this->chemLightLayer.addGraphic(mouseClickGraphic);
    Q_UNUSED(id)

 // QString messageID;
 //    QByteArray report;

    // create a new chem light
    // messageID = QUuid::createUuid().toString();
    // report = createChemLightReport(pos, messageID, "update");

    // transmitMessages(report);
    emit clearChemLightUI();
    
    chemLightColorStr = "None";
    mouseState = MouseStateNone;

}

void MapController::handleZoomIn()
{
    map->zoom(0.5);
}

void MapController::handleZoomOut()
{
    map->zoom(2);
}

void MapController::handlePan(QString direction)
{
    if (followOwnship)
    {
        followOwnship = false;
        map->setRotation(0);
    }
    Envelope extent = map->extent();

    double width = extent.width();
    double height = extent.height();

    double centerX = extent.centerX();
    double centerY = extent.centerY();

    const double PAN_INCREMENT = 0.25;

    if (direction.compare("up") == 0)
        centerY += height * PAN_INCREMENT;
    else if (direction.compare("down") == 0)
        centerY -= height * PAN_INCREMENT;
    else if (direction.compare("left") == 0)
        centerX -= width * PAN_INCREMENT;
    else if (direction.compare("right") == 0)
        centerX += width * PAN_INCREMENT;

    Envelope newExtent(Point(centerX, centerY), width, height);
    map->panTo(newExtent);
}