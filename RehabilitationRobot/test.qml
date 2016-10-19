import QtQuick 2.0

//矩形
Rectangle{
    width: 400
    height: 300
    color: "yellow"

    //组件
    Component{
        id: circular

        //圆形
        Rectangle{
            id: rect
            //通过设置别名来实现属性双向绑定
            property alias radius: rect.radius //半径
            property alias color: rect.color //颜色

            width: radius * 2
            height: radius * 2
            radius: 100 //默认
            color: "blue" //默认蓝色
        }
    }

    //1.通过Loader来引用组件
    Loader{
        sourceComponent: circular
        //组件载入完成后,设置属性
        Component.onCompleted: {
            item.radius = 10 //设置半径
            item.color = "red" //设置颜色
        }

        //定位-左
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
    }

    //2.通过Loader来引用组件
    Loader{
        sourceComponent: circular
        Component.onCompleted: {
            item.radius = 25
            item.color = "gray"
        }

        //定位-中
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
    }

    //3.通过Loader来引用组件-另一种方式
    Loader{
        id: loader
        //width: 200
        //height: 200

        //定位-右
        anchors.verticalCenter: parent.verticalCenter
        anchors.right: parent.right
    }

    Component.onCompleted: {
        loader.sourceComponent = circular
        loader.item.radius = 50
        loader.item.color = "green"
    }

}
