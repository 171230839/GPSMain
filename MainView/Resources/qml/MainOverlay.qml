import QtQuick 1.1


import "Controls"
import "CompositeControls"

LayoutItem
{

    signal homeClicked()
    signal chemLightSelected(string color)
    signal zoomInClicked()
    signal zoomOutClicked()
    signal panClicked(string direction)
//    signal emergencyClicked(bool state)
//    signal measureLineClicked(bool state)
//    signal measureAreaClicked(bool state)
//    signal measurePointAndRadiusClicked(bool state)

    function updateLocation(newLocation)
    {
        statusBar.currentLocation = "Location: " + newLocation;
    }

    function updateHeading(newHeading)
    {
        statusBar.currentHeading = "Heading: " + newHeading;
    }

        property int lastFakeSpeed: 60;
    function updateSpeed(newSpeed)
    {
        var rand = Math.random();
        if (rand < 0.005)
            lastFakeSpeed--;
        else if (rand > 0.995)
            lastFakeSpeed++;
        statusBar.currentSpeed = "        Speed: " + lastFakeSpeed;
    }

    function clearChemLightUI()
    {
        chemLightBar.clearChemLightUI()
    }

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

//    ToggleButton
//    {
//        id: btnMenu
//        buttonDefaultIcon: "../../icons/Menu-Normal.png"
//        buttonActiveIcon: "../../icons/Menu-Pressed.png"
//        anchors.bottom: parent.bottom
//        anchors.left: parent.left
//        anchors.margins: 3
//        Component.onCompleted:
//        {
//            itemClicked.connect(menu.invoke)
//        }
//        z: 2

//        Menu
//        {
//            id: menu
//            windowHeight: window.height - classificationBar.height - btnMenu.height -15
//            windowWidth: window.width / 4
//            anchors.horizontalCenter: parent.horizontalCenter
//            anchors.top: parent.bottom
//            z: 2
//            onMenuClosed:
//            {
//                while (stack.count() > 1)
//                {
//                    stack.removePanel()
//                }
//            }

//            Stack
//            {
//                id: stack
//            }

//            MainMenu
//            {
//                id: mainMenu
//                stack: stack
//                anchors.fill: parent
//                onCloseMenu: btnMenu.setToggled(false)
//            }

//        }
//    }

    PushButton
    {
        id: btnHome
        buttonDefaultIcon: "../../icons/Home-Normal.png"
        buttonActiveIcon: "../../icons/Home-Pressed.png"
        anchors.left: btnMenu.right
        anchors.bottom: parent.bottom
        anchors.margins: 3
        Component.onCompleted:
        {
            itemClicked.connect(window.homeClicked)
        }
    }

    StatusBar
    {
        id: statusBar
        width: 350
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        anchors.margins: 3
    }

    ChemLightBar
    {
        id: chemLightBar
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        anchors.margins: 3
        visible: menu.state !== "open"
        Component.onCompleted:
        {
            chemLightSelected.connect(window.chemLightSelected)
        }
    }

    NavigationControl2
    {
        id: navControl
        objectName: "navControl"
        anchors.bottom:parent.bottom
        anchors.right: parent.right
        anchors.margins: 3
        Component.onCompleted:
        {
            zoomInClicked.connect(window.zoomInClicked)
            zoomOutClicked.connect(window.zoomOutClicked)
            panClicked.connect(window.panClicked)
            clicked.connect(mainMenu.clicked)
        }
    }
    

}
