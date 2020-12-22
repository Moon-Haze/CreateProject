import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import Qt.labs.platform 1.1
import QtQuick.Window 2.2
import ListData 1.0

Window {
    id: mWindow;
    // @disable-check M16
    width:Screen.desktopAvailableWidth*0.6
    // @disable-check M16
    height:Screen.desktopAvailableHeight*0.6
    // @disable-check M16
    visible:true;
    // @disable-check M16
    title:"Create a Stm32 Project";

    signal createProject();
    signal openProject();
    property real isflag: -1
    Image {
        id: rocket;width: mWindow.width*0.2;
        height: width/192*160;
        anchors.left: parent.left;anchors.bottom: parent.bottom;
        source: 'qrc:/iamge/e.png'; anchors.bottomMargin: 0;anchors.leftMargin: 0
    }
    FileDialog{}
    FolderDialog {
        id: fileDialog;title: qsTr("Please choose a file");
        folder:{
            if(pathNameIn.text.replace(" ","")!==""){
                return StandardPaths.writableLocation(StandardPaths.DocumentsLocation);
            }else{
                return pathNameIn.text
            }
        }

        onAccepted: {
            pathNameIn.text=fileDialog.folder.toString().substring(8,fileDialog.folder.length);
        }
    }
    MessageDialog{
        id:messB
        onOkClicked: {
            switch(isflag){
            case 1:
                openProject();
                mWindow.close();
                break;
            case 0:
                mWindow.close();
                break;
            case 2:
                createProject();
                break;
            }
        }
        onCancelClicked: {
            mWindow.close();
        }
        onRejected: {
            mWindow.close();
        }
    }

    Column{
        id: column;y:mWindow.width*0.06;spacing: mWindow.width*0.02;width: mWindow.width*0.7
        anchors.horizontalCenter:  parent.horizontalCenter
        Row{
            spacing: parent.width*0.01;width: parent.width;height: width*0.05;
            Text {
                height: parent.height;width:parent.width*0.15;
                id: proNameText;text: qsTr("项目名:");
                fontSizeMode: Text.VerticalFit;
//                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                //                font.pixelSize: 14
                anchors.verticalCenter: parent.verticalCenter
            }
            TextField{
                id:proNameIn;
                height: parent.height; width:parent.width*0.84;focus: true;
                background: Rectangle{
                    radius: 2;border.width: 1;
                    implicitWidth: parent.width;implicitHeight: parent.height;
                }
                onAccepted: {
                    if((pathNameIn.text.replace(" ","")!=="")&&(proNameIn.text.replace(" ","")!=="")){
                        createButton.clicked();
                    }
                }
            }
        }

        Row{
            spacing: parent.width*0.01;width: parent.width ;height: width*0.05;
            Text {
                id: pathNameText;width:parent.width*0.15;height: parent.height;
                text: qsTr("项目路径: ");
                fontSizeMode: Text.VerticalFit;
//                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                //                font.pixelSize: 14
                anchors.verticalCenter: parent.verticalCenter
            }
            TextField{
                id:pathNameIn
                width:parent.width*0.78; height: parent.height;
                //                font.pixelSize: 12
                text: config.getChecked() ? config.getPath():""
                readOnly:bBox.checked;
                background: Rectangle {
                    implicitWidth: parent.width;implicitHeight: parent.height;
                    border.width: 1;radius: 2
                }
            }
            BeaButton {
                height: parent.height; width: parent.width*0.05;text: qsTr("...");
                onClicked: fileDialog.open();enabled: !bBox.checked
            }
        }

        Row{
            width: parent.width;height: width*0.04;
            BeaCheckBox{
                height: parent.height;width: parent.width
                id:bBox;text:qsTr("默认路径");
                checked: config.getChecked();
            }
        }

        Row {
            width: parent.width;
            height: parent.height*0.6;
            id: plib;spacing: parent.width*0.01
            Rectangle{
                width: parent.width*0.48;height: parent.height;border.width: 1
                anchors.left: parent.left;id:listRec1
                Text{
                    anchors.left: parent.left; anchors.leftMargin: 15;
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    fontSizeMode: Text.VerticalFit;
                    height: parent.height*0.12;
                    text: qsTr("项目设置");
                }
                ListView {
                    width: parent.width-20;height: parent.height
                    id: listView1; anchors.fill: parent; anchors.margins: 1
                    anchors.left: parent.left; anchors.leftMargin: 20;
                    anchors.top: parent.top;anchors.topMargin:  parent.height*0.12;
                    clip: true;
                    model: ListDataModel{
                        list:setList
                    }
                    delegate:Component{
                        BeaCheckBox{
                            height: listRec1.height*0.12;width: listRec1.width-20;
                            text: module;checked: selected;onCheckedChanged: selected=checked;
                        }
                    }
                }
            }

            Rectangle{
                width: parent.width*0.48;height: parent.height;border.width: 1
                anchors.right: parent.right;id:listRec2
                Rectangle{
                    width: parent.width;height: parent.height;border.width: 1
                    anchors.left: parent.left
                    Text{
                        anchors.left: parent.left; anchors.leftMargin: 15;
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        fontSizeMode: Text.VerticalFit;
                        height: parent.height*0.12;
                        text: qsTr("元件模块");
                    }
                    ListView {
                        width: parent.width-20;height: parent.height
                        anchors.left: parent.left; anchors.leftMargin: 20;
                        id: listView2; anchors.fill: parent; anchors.margins: 1
                        anchors.top: parent.top;anchors.topMargin:  parent.height*0.12;
                        clip: true;
                        model: ListDataModel{
                            list:moduleList
                        }
                        delegate:Component{
                            BeaCheckBox{
                                height: listRec2.height*0.12;width: listRec2.width-20;
                                text: module;checked: selected;onCheckedChanged: selected=checked;
                            }
                        }
                    }
                }
            }
        }
    }
    BeaButton{
        id:createButton;text: qsTr("生成")
        anchors.verticalCenter: rocket.verticalCenter
        width: mWindow.width*0.10; height: mWindow.height*0.08
        anchors.right: parent.right;anchors.rightMargin: mWindow.width*0.12
        onClicked: {
            if((pathNameIn.text.replace(" ","")!=="")&&(proNameIn.text.replace(" ","")!=="")){
                config.setName(proNameIn.text);
                config.setPath(pathNameIn.text);
                createProject();
            }
        }
    }
    Component.onCompleted: {
        config.createOver.connect(onProjectOver);
    }

    function onProjectOver(status){
        isflag=status;
        switch(status){
        case 0:
            messB.title=qsTr("Error");
            messB.text=qsTr("An error occurred.It could be that the program template does not exist.")
            messB.buttons= MessageDialog.Ok;
            messB.open();
            break;
        case 1:
            messB.title=qsTr("Create a Stm32 Project");
            messB.text=qsTr("A new project has been created. Do you want to open the project?");
            messB.buttons= MessageDialog.Ok | MessageDialog.Cancel;
            messB.open();
            break;
        case 2:
            messB.title=qsTr("Create a Stm32 Project");
            messB.text=qsTr("The project's folder already exists, whether to create a new one?");
            messB.buttons= MessageDialog.Ok | MessageDialog.Cancel;
            messB.open();
            break;
        }
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:600;width:640}D{i:1}
}
##^##*/
