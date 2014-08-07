import QtQuick 2.3


import "Controls"


LayoutItem
{
    id: window
    width: 1024
    height: 768
    minimumSize: "600x400"
    preferredSize: "800x600"

    ClassificationBar
    {
        id: classificationBar
        width: parent.width
        lblClassification: parent.width
        classificationColor: Qt.rgba(0, 1, 0, 1)
    }
}
