import QtQuick 1.1
import "../../Controls"

Panel
{
     signal openSerialPort()

    property Panel portNoPanel
//    property list<QtObject> portNoPanelModel
    id: serialConfigPanel
    title: "SerialPort Config"
    anchors.fill:  parent
    visible: false
    onGoBack:  stack.removePanel()
    delegate: MultiDelegate{}

    function openSerialPortPanel()
    {
//        var modelData = new Array();
//        for(var i=0; i < serialPortNo.length; ++i)
//        {
//            var str = serialPortNo[i];
//            console.log("str:" + str);
//            var  componentTemp = Qt.createComponent("../../Controls/ModelObject.qml");

//             var object = componentTemp.createObject(serialConfigPanel, { text: "ssss"});
////            object.text = "sssss";
//                modelData[i] = object;
//        }
        console.log("model:" + model);
        var component = Qt.createComponent("PortNoPanel.qml");
         var panel = component.createObject(serialConfigPanel);
        stack.addPanel(panel);
    }

    model:
        [
//        function openPortNoPanel()
//        {
//          portNoPanel.setModelData();
//          stack.addPanel();
//        }
        ModelObject {  text: "SerialPort No: " + portNoPanel.selectedContent ; onNoArgModelSignal: {stack.addPanel(portNoPanel)} }
//         ModelObject { text: "SerialPort Number"; onNoArgModelSignal: stack.addPanel(serialPortThread.openPortNoPanel()) }
//     ModelObject { text: "SerialPort Number"; onNoArgModelSignal: openSerialPortPanel() }
    ]

    PushButton
    {
        id: btnOpen
        width: 150
        buttonText: "Open"
        buttonDefaultIcon: "../../icons/Menu-Button-Normal.png"
        buttonActiveIcon: "../../icons/Menu-Button-Pressed.png"
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        anchors.margins: 10
        onItemClicked: { serialConfigPanel.openSerialPort(); serialConfigPanel.goBack(); }
    }
}
