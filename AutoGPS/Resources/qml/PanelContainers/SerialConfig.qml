import QtQuick 1.1
import "../Controls"
import "../Panels/Config"

Item{
    property Panel panel
    property Stack stack
    id: serialConfig
    panel: serialConfigPanel
    resources:
    [
        QtObject
        {
            id: config
            property string portNo: portNoPanel.selectedContent
//            property string baudRate: baudRatePanel.selectedContent
//            property string dataBits: dataBitsPanel.selectedContent
//            property  string stopBits:  stopBits.selectedContent
//            property string parity: parity.selectedContent
        }

    ]

    SerialConfigPanel
    {
        id: serialConfigPanel
        stack: serialConfig.stack
        portNoPanel: portNoPanel
//        portNoPanelModel: portNoPanel.model
//        baudRatePanel: baudRatePanel
//        dataBitsPanel: dataBitsPanel
//        stopBitsPanel: stopBitsPanel
//        parityPanel: parityPanel

    }

    PortNoPanel
    {
        id: portNoPanel
        stack: serialConfig.stack

    }

//    BaudRatePanel
//    {
//        id: baudRatePanel
//        stack: serialConfig.panel
//    }

//    DataBitsPanel
//    {
//        id: dataBitsPanel
//        stack: serialConfig.panel
//    }

//    StopBitsPanel
//    {
//        id: stopBitsPanel
//        stack: serialConfig.panel
//    }

//    ParityPanel
//    {
//        id: parityPanel
//        stack: serialConfig.panel
//    }

//    TimeOutPanel
//    {

//    }
}
