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

TARGET = MainView 
TEMPLATE = app

QT += core gui opengl xml network declarative

greaterThan(QT_MAJOR_VERSION, 4) {
    QT += widgets
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
        MainView.cpp \ 
    DictionaryImageProvider.cpp \
    AppConfigDialog.cpp \
    GPSSimulator.cpp \
    MapController.cpp \
    simplegraphicoverlay.cpp \
    Geomessage.cpp

HEADERS += \
	MainView.h \
    DictionaryImageProvider.h \
    AppConfigDialog.h \
    GPSSimulator.h \
    MapController.h \
    simplegraphicoverlay.h \
    Geomessage.h

RESOURCES += MainViewResources.qrc

FORMS += \
    AppConfigDialog.ui \
    AppConfigDialog.ui

OTHER_FILES += \
    Resources/ArcGISRuntimeLicense.txt \
    Resources/qml/MainOverlay.qml \
    Resources/qml/CompositeControls/StatusBar.qml \
    Resources/qml/CompositeControls/NumberPad.qml \
    Resources/qml/CompositeControls/NavigationControl2.qml \
    Resources/qml/CompositeControls/NavigationControl.qml \
    Resources/qml/CompositeControls/ChemLightBar.qml \
    Resources/qml/Controls/StackObject.js \
    Resources/qml/Controls/ToggleButton.qml \
    Resources/qml/Controls/Stack.qml \
    Resources/qml/Controls/ScrollButton.qml \
    Resources/qml/Controls/PushButton.qml \
    Resources/qml/Controls/Panel.qml \
    Resources/qml/Controls/MultiDelegate.qml \
    Resources/qml/Controls/ModelObject.qml \
    Resources/qml/Controls/Menu.qml \
    Resources/qml/Controls/ClassificationBar.qml \
    Resources/qml/PanelContainers/SpotReport.qml \
    Resources/qml/PanelContainers/Search.qml \
    Resources/qml/PanelContainers/Reports.qml \
    Resources/qml/PanelContainers/MainMenu.qml \
    Resources/qml/PanelContainers/Keywords.qml \
    Resources/qml/PanelContainers/EquipmentSearch.qml \
    Resources/qml/PanelContainers/Browse.qml \
    Resources/qml/Panels/MainMenu/ReportsPanel.qml \
    Resources/qml/Panels/MainMenu/OverlaysPanel.qml \
    Resources/qml/Panels/MainMenu/OptionsPanel.qml \
    Resources/qml/Panels/MainMenu/ObservationsPanel.qml \
    Resources/qml/Panels/MainMenu/MainPanel.qml \
    Resources/qml/Panels/MainMenu/BuddiesPanel.qml \
    Resources/qml/Panels/Search/TextSearchPanel.qml \
    Resources/qml/Panels/Search/SearchPanel.qml \
    Resources/qml/Panels/Search/ResultsPanel.qml \
    Resources/qml/Panels/Search/RecentPanel.qml \
    Resources/qml/Panels/Search/KeywordsPanel.qml \
    Resources/qml/Panels/Search/CategoryPanel.qml \
    Resources/qml/Panels/Search/BrowsePanel.qml \
    Resources/qml/Panels/Search/AffiliationPanel.qml \
    Resources/qml/Panels/Search/AddKeywordsPanel.qml \
    Resources/qml/Panels/SpotReport/UnitPanel.qml \
    Resources/qml/Panels/SpotReport/TimePanel.qml \
    Resources/qml/Panels/SpotReport/SpotReportPanel.qml \
    Resources/qml/Panels/SpotReport/SizePanel.qml \
    Resources/qml/Panels/SpotReport/LocationPanel.qml \
    Resources/qml/Panels/SpotReport/EquipmentPanel.qml \
    Resources/qml/Panels/Identify/IdentifyResultsPanel.qml \
    Resources/qml/Panels/SpotReport/ActivityPanel.qml \
    Resources/qml/Panels/MainMenu/VisibilityPanel.qml \
    Resources/qml/myqml.qml


