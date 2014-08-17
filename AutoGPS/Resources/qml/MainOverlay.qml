import QtQuick 1.1

import "CompositeControls"
import "PanelContainers"
import "Controls"

LayoutItem
{
    signal basemapChanged(string name)
    signal homeClicked()
    signal zoomInClicked()
    signal zoomOutClicked()
    signal panClicked(string direction)

    function updateLocation(newLocation)
    {
        statusBar.currentLocation = "Location: " + newLocation
    }

    function updateSpeed(newSpeed)
    {
        //TODO we should use the actual speed, as follows:
        //statusBar.currentSpeed = "   Speed: " + (Math.round(10.0 * newSpeed) / 10.0);
        //However, the app provides speeds that are unrealistically fast and highly variable.
        //Therefore, fake it as follows:
        //        var rand = Math.random();
        //        if (rand < 0.005)
        //            lastFakeSpeed--;
        //        else if (rand > 0.995)
        //            lastFakeSpeed++;
        statusBar.currentSpeed = "      Speed: " + newSpeed;
    }


    function updateHeading(newHeading)
    {
        statusBar.currentHeading = " Heading: " + newHeading
    }

    function updateMapRotation(newRotation)
    {
        northArrow.rotation = 360 - newRotation;
    }

    function updateTime(newTime)
    {
        statusBar.currentTime = "Time: " + newTime
    }

    //    function updateBasemapList(newList)
    //    {
    //        btnBasemaps.scrollList = newList
    //    }



    id: window
    width: 1024
    height: 768
    minimumSize: "600x400"
    preferredSize: "800x600"

    ClassificationBar
    {
        id: classificationBar
        width: parent.width
        lblClassification: "AutoGPS"
        classificationColor: Qt.rgba(0, 1, 0, 1)
    }

    ToggleButton
    {
        id: btnMenu
        buttonDefaultIcon: "../../icons/Menu-Normal.png"
        buttonActiveIcon: "../../icons/Menu-Pressed.png"
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.margins: 3
        Component.onCompleted:
        {
            itemClicked.connect(menu.invoke)
        }
        z: 2

        Menu
        {
            id: menu
            windowHeight: window.height - classificationBar.height - btnMenu.height - 15
            windowWidth: window.width / 4
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: parent.bottom
            z: 2
            onMenuClosed:
            {
                while (stack.count() > 1)
                {
                    stack.removePanel()
                }
            }

            Stack
            {
                id: stack
            }

            MainMenu
            {
                id: mainMenu
                stack: stack
                anchors.fill: parent
                onCloseMenu: btnMenu.setToggled(false)
            }
        }
    }

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
            itemClicked.connect(mainMenu.clicked)
        }
    }



    // Layer toggling, add back in if desired:
    ScrollButton
    {
        id: btnBasemaps
        scrollButtonDefaultIcon: "../../icons/Basemap-Normal.png"
        anchors.top: classificationBar.bottom
        anchors.topMargin: 10
        anchors.right: parent.right
        anchors.rightMargin: 5
        visible: menu.state != "open"
        Component.onCompleted:
        {
            itemChanged.connect(window.basemapChanged)
        }
    }

    StatusBar
    {
        id: statusBar
        width: 400
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        anchors.margins: 3
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
    Image {
        id: northArrow
        source: "qrc:/Resources/icons/North-Arrow.png"
        anchors.bottomMargin: 25
        anchors.bottom: navControl.top
        anchors.horizontalCenter: navControl.horizontalCenter
        width: navControl.width * .75
        height: navControl.width * .75
        smooth: true
        fillMode: Image.PreserveAspectFit
    }


}
