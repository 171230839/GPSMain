#include "mapcontroller.h"


MapController::MapController(Map* inputMap,
                             MapGraphicsView* inputGraphicsView,
                             QObject* parent):
    map(inputMap),
    mapGraphicsView(inputGraphicsView),
    QObject(parent),
    showOwnship(false),
    followOwnship(false),
    isMapReady(false),
    drawingOverlay(0),
    lastHeading(0.0)
{


}

MapController::~MapController()
{

}

void MapController::onMapReady()
{

}




void MapController::handleHomeClicked()
{
    if (!ownshipStartingMapPoint.isEmpty())
    {
        handleToggleFollowMe(false);
//        map->setExtent(originalExtent);
//        map->setScale(originalScale);
        map->setRotation(0);
        map->panTo(ownshipStartingMapPoint);
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
