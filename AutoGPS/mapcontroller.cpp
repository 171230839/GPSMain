#include "mapcontroller.h"
#include <GeometryEngine.h>

MapController::MapController(Map* inputMap,
                             MapGraphicsView* inputGraphicsView,
                             QObject* parent):
    map(inputMap),
    mapGraphicsView(inputGraphicsView),
    QObject(parent),
    showOwnship(true),
    followOwnship(false),
    isMapReady(false),
    drawingOverlay(0)
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
    if (!currentMapPoint.isEmpty())
    {
        qDebug()<<"homeClicked";
        handleToggleFollowMe(false);
        map->setExtent(originalExtent);
        map->setScale(originalScale);
        map->setRotation(0);
        map->panTo(currentMapPoint);
    }
}

void MapController::handleToggleShowMe(bool state)
{
    showOwnship = state;
}

void MapController::handleToggleFollowMe(bool state)
{
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

void MapController::onAvaliblePosition(double lat, double lon, double heading)
{
    if (!isMapReady ||  mapGraphicsView == 0)
        return;
    qDebug()<<"onAvailblePosition-lat:"<<lat<<"  lon:"<<lon;
    Point mapPoint = GeometryEngine::project(lon, lat, map->spatialReference());
    currentMapPoint = mapPoint;
    qDebug()<<"currentMapPoint: x: "<<currentMapPoint.x()<<" y:"<<currentMapPoint.y();
    if (drawingOverlay == 0)
    {
        qDebug()<<"new SimpleGraphicOverlay";
        drawingOverlay = new SimpleGraphicOverlay();

        QPixmap ownshipPixmap(":/Resources/icons/Ownship.png");
        QImage ownshipImage = ownshipPixmap.toImage();
        drawingOverlay->setImage(ownshipImage);
        drawingOverlay->setGraphicsView(mapGraphicsView);

        originalScale = map->scale();
        originalExtent = map->extent();
    }
    drawingOverlay->setVisible(showOwnship);
    if (showOwnship)
    {
        drawingOverlay->setPosition(mapPoint);
        drawingOverlay->setAngle(heading);
    }

    if (followOwnship)
    {
        map->setRotation(heading);
        map->panTo(mapPoint);
    }
}

