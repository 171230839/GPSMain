import QtQuick 2.2


LayoutItem
{

    Text {
        text: qsTr("Hello World")
        anchors.centerIn: parent
    }
    id: window
    width: 1024
    height: 768
    minimumSize: "600x400"
    preferredSize: "800x600"
}
