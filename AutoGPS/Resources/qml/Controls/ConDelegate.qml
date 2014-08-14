import QtQuick 1.1

Item{
    id: conDelegate


    width: childrenRect.width
    height: childrenRect.height
    anchors.horizontalCenter: parent.horizontalCenter

    PushButton
    {
        id: btn
        buttonText: model.modelData
        buttonDefaultIcon: "../../icons/Menu-Button-Normal.png"
        buttonActiveIcon: "../../icons/Menu-Button-Pressed.png"
        width: 150
        textColor: "black"
        buttonColor: "dimgrey"
        //            Component.onCompleted: itemClicked.connect(model.modelData.noArgModelSignal)
        onItemClicked:
        {
            //                multiDelegate.clicked()
            //                model.modelData.modelSignal(model.modelData.text)
        }
    }



}
