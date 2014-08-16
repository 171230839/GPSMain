import QtQuick 1.1
import "../Panels/Worker"
import "../Controls"


Item{
    property Panel panel
    property Stack stack
    id: worker
    panel: workerPanel

    WorkerPanel
    {
        id: workerPanel
        stack: worker.stack
         anchors.fill: parent
        geometryPanel: geometryPanel
    }

    GeometryPanel
    {
        id: geometryPanel
        stack: worker.stack
         anchors.fill: parent
    }


}
