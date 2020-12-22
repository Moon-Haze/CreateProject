import QtQuick 2.15
import QtQuick.Controls 2.15

Button{
    background: Rectangle {
        implicitWidth: 100
        implicitHeight: 25
        border.width: 0
        radius: 4
        gradient: Gradient {
            GradientStop { position: 0 ; color: parent.pressed ? "#ccc" : "#eee" }
            GradientStop { position: 1 ; color: parent.pressed ? "#aaa" : "#ccc" }
        }
    }
}
