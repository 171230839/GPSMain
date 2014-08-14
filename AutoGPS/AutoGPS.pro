#-------------------------------------------------
#  Copyright 2014 ESRI
#
#  All rights reserved under the copyright laws of the United States
#  and applicable international laws, treaties, and conventions.
#
#  You may freely redistribute and use this sample code, with or
#  without modification, provided you include the original copyright
#  notice and use restrictions.
#
#  See the Sample code usage restrictions document for further information.
#-------------------------------------------------

TARGET = AutoGPS 
TEMPLATE = app

QT += core gui opengl xml network declarative

greaterThan(QT_MAJOR_VERSION, 4) {
    QT += widgets serialport
}

# After installing the Runtime SDK for Qt you'll need to copy the
# esri_runtime_qt_10_2_3.prf file from your qt10.2.3/sdk/ideintegration folder
# to your Qt SDK's mkspecs/features folder in order for Qt Creator to locate 
# the ArcGIS Runtime SDK for Qt header files and libraries.
#
CONFIG += c++11 esri_runtime_qt_10_2_3 

win32:CONFIG += \
  embed_manifest_exe


SOURCES += \
	main.cpp \
        AutoGPS.cpp \ 
    mapcontroller.cpp \
    simplegraphicoverlay.cpp \
    masterthread.cpp

HEADERS += \
	AutoGPS.h \
    mapcontroller.h \
    simplegraphicoverlay.h \
    masterthread.h

FORMS +=

RESOURCES += \
    AutoGPSResources.qrc

OTHER_FILES += \
    Resources/qml/CompositeControls/NavigationControl2.qml \
    Resources/qml/CompositeControls/StatusBar.qml\
    Resources/qml/Controls/ClassificationBar.qml\
    Resources/qml/Controls/Menu.qml\
    Resources/qml/Controls/ModelObject.qml\
    Resources/qml/Controls/MultiDelegate.qml\
    Resources/qml/Controls/Panel.qml\
    Resources/qml/Controls/PushButton.qml\
    Resources/qml/Controls/Stack.qml\
    Resources/qml/Controls/StackObject.js\
    Resources/qml/Controls/ToggleButton.qml\
    Resources/qml/MainOverlay.qml\
    Resources/qml/PanelContainers/MainMenu.qml\
    Resources/qml/Panels/MainMenu/MainPanel.qml \
    Resources/qml/PanelContainers/SerialConfig.qml \
    Resources/qml/PanelContainers/Config.qml \
    Resources/qml/Panels/Config/ConfigPanel.qml \
    Resources/qml/Panels/Config/SerialConfigPanel.qml \
    Resources/qml/Panels/Config/PortNoPanel.qml \
    Resources/qml/Controls/ConDelegate.qml \
    Resources/qml/Panels/Config/BaudRatePanel.qml \
    Resources/qml/Panels/Config/DataBitsPanel.qml \
    Resources/qml/Panels/Config/StopBitsPanel.qml \
    Resources/qml/Panels/Config/ParityPanel.qml
