import QtQuick 1.1
import "../../Controls"

Panel{

    property Stack stack
    property Panel geometryPanel
    id: workerPanel
    title: "Start Work"
    visible: false;
    onGoBack: stack.removePanel()
    delegate: MultiDelegate{}
    model:[
        ModelObject { text: "get Geometry "; onNoArgModelSignal: { stack.addPanel(geometryPanel)}}
//        ModelObject { text: "TimeOut: " + timeOutanel.selectedContent; onNoArgModelSignal: { stack.addPanel(timeOutPanel)}},
//        ModelObject { text: "TimeOut: " + timeOutPanel.selectedContent; onNoArgModelSignal: { stack.addPanel(timeOutPanel)}}

    ]


}
