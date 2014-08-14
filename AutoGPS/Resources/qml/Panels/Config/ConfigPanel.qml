import QtQuick 1.1
import "../../Controls"

Panel
{
    property Panel serialConfig

    id: configPanel
    title: "serialPort"
    visible: false;
    onGoBack: stack.removePanel()
    delegate: MultiDelegate{}
    model:
    [
        ModelObject { text: "SerialPort Conifg"; onNoArgModelSignal: stack.addPanel(serialConfig); }
    ]
}
