import QtQuick 2.12
import QtQuick.Controls 2.5

//window containing the application
ApplicationWindow {
    //title of the application
    title: qsTr("Hello World")
    visible: true
    width: 640
    height: 480

    Rectangle {
        width: 200
        height: 100
        color: "red"

        Text {
            anchors.centerIn: parent
            text: "Hello, World!"
        }

        ListView {
            //model: instrumentsBridge.melodicInstruments()
            model: instrumentsBridge.kitInstruments
            delegate: Rectangle {
                width: 40
                height: 10
                color: "blue"
            }
        }
    }

}