import QtQuick 2.15
import QtQuick.Controls 2.15

CheckBox {
    indicator: Rectangle {
        implicitWidth: parent.height*0.5; implicitHeight: parent.height*0.5
        x: parent.leftPadding; y: parent.height / 2 - height / 2
        radius: 2; border.color: parent.checked ? "gray" : "black"
        Rectangle {
            width: parent.width*0.6; height: parent.height*0.6
            anchors.top: parent.top;anchors.left: parent.left
            anchors.topMargin: parent.height*0.20;
            anchors.leftMargin: parent.width*0.20; radius: 2
            color: "gray"
            visible: parent.parent.checked
        }
    }

    contentItem: Text {
        text: parent.text;
        elide: Text.ElideRight
        opacity: enabled ? 1.0 : 0.3
        color: parent.checked ? "gray" : "black"
        fontSizeMode: Text.VerticalFit;
//        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        leftPadding: parent.indicator.width + parent.spacing
    }
}
