import QtQuick 2.2
import QtQuick.Window 2.1
import QtQuick.Controls 1.2
import QtMultimedia 5.0
import CVEyeCamera 1.0
Window {
    id:root
    x: 0; y: 0;
    visible: true
    width: 1920
    height: 1010
    title: qsTr(" ")

    Loader {
            id: eyecontrl_l
            x: 0; y: 480;z:2
            //sourceComponent: rect
    }
    Loader {
            id: eyecontrl_r
            z:2
            anchors.left: eyecontrl_l.right
            anchors.top: eyecontrl_l.top
            //sourceComponent: rect
    }
    Component {
            id: rect
            Rectangle {
                width: 1280
                height: 1080
                color: "red"
                Text {
                    text: "Default Page"
                    anchors.fill: parent
                }
            }
      }
    //背景图片
    Rectangle {
        id: background
        x: 0; y: 0;z:1// 使用 parent 引用父元素
        width: parent.width
        height: parent.height
        opacity: 1
        color:"#1919a2"
        //source: 'bottom.jpg'
        //状态栏
//        Image {
//            source: 'xxx.png'
//            x: 0; y: 238;z:1

//        }
        Text{
            x:0
            y: 338
            width: tray.width
            height: 63
            color: "#ededed"
            text: "左眼"
            wrapMode: Text.NoWrap
            horizontalAlignment: Text.AlignHCenter
            z: 3
            font.bold: true
            font.family: "KaiTi"
            font.pointSize: 40
            //anchors.centerIn: parent
        }
        Text{
            x:640
            y: 338
            width: tray.width
            height: 63
            color: "#ededed"
            text: "右眼"
            wrapMode: Text.NoWrap
            horizontalAlignment: Text.AlignHCenter
            z: 3
            font.bold: true
            font.family: "KaiTi"
            font.pointSize: 40
            //anchors.centerIn: parent
        }

        Rectangle {
                id: statusbar
                anchors.bottom:background.bottom
                anchors.left:background.left
                width: 1280
                height: 50
                opacity:0.5
                color: "#1919a2"
                anchors.bottomMargin: 0
                Image {
                    id: anglelable_S
                    x: 0
                    y: 0
                    width: 109
                    height: 50
                    source: "lable.png"
                    Timer{
                        interval: 500; running:true; repeat:true
                        onTriggered: {
                            shoulangle.text = robot.showShoulAngle()//Date().toString()
                        }
                    }
                    Text {
                        z:2
                        id: shoulangle
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.centerIn: parent
                        color: "white"
                        font.family: "Microsoft YaHei"
                        font.pointSize: 17
                        width:80
                        height:40
                        }
                }

                Image {
                    id: switchdata_A
                    x: 345
                    y: 0
                    width: 109
                    height: 50
                    source: "lable.png"
                    Timer{
                        interval: 500; running:true; repeat:true
                        onTriggered: {
                            armswitch.text = robot.showArmORZ()//Date().toString()
                        }
                    }
                    Text {
                        z:2
                        id: armswitch
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.centerIn: parent
                        color: "white"
                        font.family: "Microsoft YaHei"
                        font.pointSize: 17
                        width:80
                        height:40
                        }
                }

                Image {
                    id: switchdata_S
                    x: 530
                    y: 0
                    width: 109
                    height: 50
                    source: "lable.png"
                    Timer{
                        interval: 500; running:true; repeat:true
                        onTriggered: shoulswitch.text = robot.showShoulORZ()//Date().toString()
                    }
                    Text {
                        z:2
                        id: shoulswitch
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.centerIn: parent
                        color: "white"
                        font.family: "Microsoft YaHei"
                        font.pointSize: 17
                        width:80
                        height:40
                        }
                }

                Image {
                    id: anglelable_A
                    x: 168
                    y: 0
                    width: 109
                    height: 50
                    source: "lable.png"
                    Timer{
                        interval: 500; running:true; repeat:true
                        onTriggered: armangle.text = robot.showArmAngle()//Date().toString()
                    }
                    Text {
                        z:2
                        id: armangle
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.centerIn: parent
                        color: "white"
                        font.family: "Microsoft YaHei"
                        font.pointSize: 17
                        width:80
                        height:40
                    }
                }
        }

         Rectangle {
             id: rectangle1
             x: 0
             y: 0
             //anchors.right: camera0.right
             width: 1280
             height: 262
             color: "#137513"
         }

    }

    //固定不变的部分
    Rectangle{
        id:tray
        anchors.top: background.top
        anchors.right: background.right
        width:640
        height:1010
        color: "#1919a2"
        z: 2
        opacity:0.8
        visible: true
    }
    //中文标题
    Text{
        id:title_c
        x:318
        y: 27
        width: tray.width
        height: 63
        color: "#ededed"
        text: "上肢运动功能康复治疗系统"
        wrapMode: Text.NoWrap
        horizontalAlignment: Text.AlignHCenter
        z: 3
        font.bold: true
        font.family: "Microsoft YaHei"
        font.pointSize: 75
        //anchors.centerIn: parent
    }
    //英文标题
    Text{
        id:title_e
        x:320
        y: 169
        width: tray.width
        height: 69
        color:"#eceaea"
        text:qsTr("Rehabilitation Robotic System For Upper Limb Motor Therapy")
        horizontalAlignment: Text.AlignHCenter
        z: 3
        font.bold: true
        font.family: "Microsoft YaHei"
        font.pointSize: 30
        //anchors.centerIn: parent
    }

    Rectangle{
        id:camera0
        x: 1275;y:0;z:3
        width:640
        height:480
        color:"black"
        radius: 0
        anchors.rightMargin: 0
        anchors.topMargin: 254
        opacity:0.8
        visible: true
        CVEyeCamera {
            id: overWatch
            device: 4
            size: "640x480"
        }
        VideoOutput {
          id: video0
          source: overWatch
        }
    }
    /*Rectangle{
        id:camera1
        x: 0;y:480;z:3
        width:640
        height:480
        color:"black"
        radius: 0
        anchors.rightMargin: 0
        anchors.topMargin: 254
        opacity:0.8
        visible: false
        CVEyeCamera {
            id: frontCamera
            device: 3
            size: "640x480"
        }
        VideoOutput {
          id: video1
          source: frontCamera
        }
    }*/
    //主动运动组合框
    Text {
        id:activetxt
        x: 1280
        width: 640
        height: 31
        z:4
        text: qsTr("人眼视觉跟踪运动")
        horizontalAlignment: Text.AlignHCenter
        anchors.verticalCenterOffset: 46
        anchors.horizontalCenterOffset: 640
        font.bold: true
        color: "white"
        font.family: "Microsoft YaHei"
        font.pointSize: 31
        anchors.centerIn: parent
    }
    Image {
        id: button0
        x: 1380; y: 656
        z: 4
        width: 200; height: 100;
        source: 'button.png'
        Text{
            y: 30
            text: qsTr("眼动控制")
            anchors.horizontalCenterOffset: 0
            font.bold: true
            color: "#085210"
            anchors.horizontalCenter: parent.horizontalCenter
            font.family: "Microsoft YaHei"
            font.pointSize: 16
        }

        Text {
            x: 6
            y: 56
            color: "#085210"
            text: qsTr("Eye Control")
            anchors.horizontalCenter: parent.horizontalCenter
            font.family: "Microsoft YaHei"
            font.pointSize: 11
            font.bold: true
            anchors.horizontalCenterOffset: 0
        }
        MouseArea {
            anchors.fill: parent
            onClicked: {
                eyecontrl_l.source = "eyecontrol_l.qml"
                eyecontrl_r.source = "eyecontrol_r.qml"
                //camera1.visible=true
            }
        }


    }
    //被动运动种类标签
    Image {
        id: button2
        x: 1612
        y: 656
        width: button0.width
        height: button0.height
        source: "button.png"
        z: 4
        Text{
            y: 30
            text: qsTr("标定")
            anchors.horizontalCenterOffset: 0
            font.bold: true
            color: "#085210"
            anchors.horizontalCenter: parent.horizontalCenter
            font.family: "Microsoft YaHei"
            font.pointSize: 16
        }

        Text {
            x: 6
            y: 56
            color: "#085210"
            text: qsTr("Calibrate")
            anchors.horizontalCenter: parent.horizontalCenter
            font.family: "Microsoft YaHei"
            font.pointSize: 11
            font.bold: true
            anchors.horizontalCenterOffset: 0
        }
        MouseArea{
            anchors.fill: parent
            onClicked: {
                robot.getCaliCenter()
            }
        }
    }
    //开始运动按钮
    Image {
        id: button3
        x: 1280
        y: 827
        width: button0.width
        height: button0.height
        z: 4
        source: "button.png"
        Text{
            y: 30
            text: qsTr("开始")
            anchors.horizontalCenterOffset: 0
            font.bold: true
            color: "#085210"
            anchors.horizontalCenter: parent.horizontalCenter
            font.family: "Microsoft YaHei"
            font.pointSize: 16
        }

        Text {
            x: 6
            y: 56
            color: "#085210"
            text: qsTr("Start")
            anchors.horizontalCenter: parent.horizontalCenter
            font.family: "Microsoft YaHei"
            font.pointSize: 11
            font.bold: true
            anchors.horizontalCenterOffset: 0
        }
        MouseArea{
            anchors.fill: parent
            onClicked: {
                robot.beginEyeContrl()
            }
        }
    }
    //复位运动按钮
    Image {
        id: button4
        x: 1713
        y: 827
        width: button0.width
        height: button0.height
        source: "button.png"
        z: 4
        Text{
            y: 30
            text: qsTr("复位")
            anchors.horizontalCenterOffset: 0
            font.bold: true
            color: "#085210"
            anchors.horizontalCenter: parent.horizontalCenter
            font.family: "Microsoft YaHei"
            font.pointSize: 16
        }

        Text {
            x: 6
            y: 56
            color: "#085210"
            text: qsTr("Reset")
            anchors.horizontalCenter: parent.horizontalCenter
            font.family: "Microsoft YaHei"
            font.pointSize: 11
            font.bold: true
            anchors.horizontalCenterOffset: 0
        }
        MouseArea{
            anchors.fill: parent
            onClicked: {
                robot.Reset2ORZ()
            }
        }
    }

    Image {
        id: button5
        x: 1500
        y: 827
        width: button0.width
        height: button0.height
        z: 4
        source: "button.png"
        Text{
            y: 30
            text: qsTr("停止")
            anchors.horizontalCenterOffset: 0
            font.bold: true
            color: "#085210"
            anchors.horizontalCenter: parent.horizontalCenter
            font.family: "Microsoft YaHei"
            font.pointSize: 16
        }

        Text {
            x: 6
            y: 56
            color: "#085210"
            text: qsTr("Stop")
            anchors.horizontalCenter: parent.horizontalCenter
            font.family: "Microsoft YaHei"
            font.pointSize: 11
            font.bold: true
            anchors.horizontalCenterOffset: 0
        }
        MouseArea{
            anchors.fill: parent
            onClicked: {
                robot.stopEyeContrl()
                robot.Stop2ORZ()
            }
        }
    }

    Text {
        id: activetxt1
        x: 1284
        y: -4
        width: 636
        height: 19
        color: "#ffffff"
        text: qsTr("Human Eye Tracking Motion")
        z: 4
        anchors.verticalCenterOffset: 100
        horizontalAlignment: Text.AlignHCenter
        font.family: "Microsoft YaHei"
        font.pointSize: 16
        anchors.centerIn: parent
        font.bold: true
        anchors.horizontalCenterOffset: 640
    }

}

