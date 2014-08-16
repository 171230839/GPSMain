#include "mapcontroller.h"
#include <GeometryEngine.h>
#include <SimpleMarkerSymbol.h>
#include <Polyline.h>
#include <Polygon.h>
#include <SimpleFillSymbol.h>

MapController::MapController(Map* inputMap,
                             MapGraphicsView* inputGraphicsView,
                             QObject* parent):
    map(inputMap),
    mapGraphicsView(inputGraphicsView),
    QObject(parent),
    showOwnship(true),
    followOwnship(false),
    isMapReady(false),
    drawingOverlay(0),
    bPoints(false),
    readyPointList(false),
    graphicId(0)
{


}

MapController::~MapController()
{

}

void MapController::onMapReady()
{
    isMapReady = true;
}




void MapController::handleHomeClicked()
{
    if (!ownshipStartingMapPoint.isEmpty())
    {
        handleToggleFollowMe(false); // or it will just snap right back in simulation

        map->setExtent(originalExtent);
        map->setScale(originalScale);
        map->setRotation(0);
        map->panTo(ownshipStartingMapPoint);
    }
}

void MapController::handleToggleShowMe(bool state)
{
    qDebug()<<"handleToggleShowMe"<<state;
    showOwnship = state;
}

void MapController::handleToggleFollowMe(bool state)
{
    qDebug()<<"handleToggleFollowMe:"<<state;
    followOwnship = state;
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

void MapController::handleResetMap()
{
    qDebug()<<"handleResetMap()";
    handleToggleFollowMe(false); // or it will just snap right back in simulation

    map->setExtent(originalExtent);
    map->setScale(originalScale);
    map->setRotation(0);
}

void MapController::onAvaliblePosition(double lat, double lon, double heading)
{
    if (!isMapReady ||  mapGraphicsView == 0)
        return;
    Point mapPoint = GeometryEngine::project(lon, lat, map->spatialReference());
    currentMapPoint = mapPoint;
    if (ownshipStartingMapPoint.isEmpty())
    {
        originalExtent = map->extent();
        originalScale = map->scale();
        ownshipStartingMapPoint = mapPoint; // originalExtent.center();
    }

    if (drawingOverlay == 0)
    {
        qDebug()<<"new SimpleGraphicOverlay";
        drawingOverlay = new SimpleGraphicOverlay();

        QPixmap ownshipPixmap(":/Resources/icons/Ownship.png");
        QImage ownshipImage = ownshipPixmap.toImage();
        drawingOverlay->setImage(ownshipImage);
        drawingOverlay->setGraphicsView(mapGraphicsView);
    }
    drawingOverlay->setVisible(showOwnship);
    if (showOwnship)
    {
        qDebug()<<"on showOwnship";
        drawingOverlay->setPosition(mapPoint);
        drawingOverlay->setAngle(heading);
    }

    if (followOwnship)
    {
        qDebug()<<"on     followOwnShip";
        map->setRotation(heading);
        map->panTo(mapPoint);
    }
}

void MapController::handlePointsToggled(bool state)
{
    bPoints = state;
    if (!state && (graphicId != 0))
    {
        pointList.pop_back();
        pointsLayer.removeGraphic(graphicId);
    }
}

void MapController::handleToLinesClicked()
{
    qDebug()<<"handleToLinesClicked()"<< pointList.size();
    if (isMapReady)
    {
        //        pointList.append(pointList.at(0));
        if (pointList.size() <= 1)
            return;
        if (!readyPointList)
        {
//            pointList.pop_back();
            pointList.append(pointList.first());
            readyPointList = true;
        }
        else
        {
//            pointList.pop_back();
            pointsLayer.removeAll();
        }
        QList<QList<EsriRuntimeQt::Point> > tmpList;
        tmpList.append(pointList);
        EsriRuntimeQt::Polyline line1(tmpList);
        EsriRuntimeQt::SimpleLineSymbol lineSym1(QColor(0,0,255), 3);
        EsriRuntimeQt::Graphic graphic1(line1, lineSym1);
        pointsLayer.addGraphic(graphic1);

    }
}

void MapController::handleOkClicked()
{
    qDebug()<<"handleOkClicked()";
    map->removeLayer("tiledLayer");
}

void MapController::handleToPolygonClicked()
{
    if (isMapReady)
    {
        if (pointList.size() <= 1)
            return;
        if (!readyPointList)
        {
//            pointList.pop_back();
            pointList.append(pointList.first());
            readyPointList = true;
        }
        else
        {
//            pointList.pop_back();
            pointsLayer.removeAll();
        }
        QList<QList<EsriRuntimeQt::Point> > tmpList;
        tmpList.append(pointList);
        EsriRuntimeQt::Polygon polygon(tmpList);
        pointsLayer.addGraphic(EsriRuntimeQt::Graphic(polygon, EsriRuntimeQt::SimpleFillSymbol(QColor("Green"))));
    }
}

void MapController::mousePress(QMouseEvent mouseEvent)
{
    if (bPoints && isMapReady)
    {
        QPointF mousePoint = QPointF(mouseEvent.pos().x(), mouseEvent.pos().y());
        Point mapPoint = map->toMapPoint(mousePoint.x(), mousePoint.y());
        pointList.append(mapPoint);
        SimpleMarkerSymbol smsSymbol(Qt::red, 5, SimpleMarkerSymbolStyle::Circle);
         Graphic mouseClickGraphic(mapPoint, smsSymbol);
       graphicId = pointsLayer.addGraphic(mouseClickGraphic);
    }
}

void MapController::init()
{
    if (map)
    {
        map->addLayer(pointsLayer);
    }

}

void MapController::onClearClicked()
{
    pointList.clear();
    pointsLayer.removeAll();
    readyPointList = false;
}
