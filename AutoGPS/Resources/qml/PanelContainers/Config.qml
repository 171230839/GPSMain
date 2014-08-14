import QtQuick 1.1
import "../Panels/Config"
import "../Controls"


Item{
    property Stack stack
    property Panel panel

     id: config
     panel: configPanel

     ConfigPanel
     {
         id: configPanel
         stack: config.stack
         anchors.fill: parent
         serialConfig: serialConfig.panel
     }
     SerialConfig
     {
         id: serialConfig
         stack: config.stack
         anchors.fill:  parent
     }
}
