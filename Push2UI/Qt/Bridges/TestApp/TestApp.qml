import QtQuick 2.12
import QtQuick.Controls 2.5

//window containing the application
import QtQuick.Layouts 1.3
ApplicationWindow {
    //title of the application
    id: appWindow
    title: qsTr("Hello World")
    visible: true
    width: 640
    height: 480

    Rectangle {
        anchors.fill: parent

        RowLayout {
            id: rowLayout
            anchors.fill: parent
            spacing: 2

            Rectangle {
                id: listViewCarsRectangle
                Layout.fillHeight: true
                width: appWindow.width / 2
                border.color: "black"
                border.width: 3
                color: "white"
                ListView {
                    id: listViewCars
                    anchors.fill: parent
                    spacing: 2
                    focus: true
                    model: cars
                    delegate: Rectangle {
                        id: wrapper
                        width: listViewCarsRectangle.width - 6
                        height: mainText.implicitHeight + 4
                        color: ListView.isCurrentItem ? "grey" : "white"
                        Text {
                            id: mainText
                            text: name
                            font.family: "Helvetica"
                            font.pointSize: 24
                            color: wrapper.ListView.isCurrentItem ? "white" : "black"
                        }
                        /*
                        ListView {
                            x: 140
                            anchors.margins: 2
                            width: 70
                            height: 30
                            model: parameter
                            delegate: Rectangle {
                                width: ListView.width
                                height: 30
                                color: "green"
                                border.color: "black"
                                border.width: 1
                                Label{ text : name + " " + value}
                            }
                        }
                        */
                    }
                    onCurrentIndexChanged:{
                        listViewParameter.model = cars.getParameterModel(currentIndex)
                    }

                }
            }

            Rectangle {
                id: listViewParameterRectangle
                Layout.fillHeight: true
                width: appWindow.width / 2
                border.color: "black"
                border.width: 3
                color: "white"

                ListView {
                    id: listViewParameter
                    anchors.fill: parent
                    delegate: Rectangle {
                                id: wrapper2
                                width: listViewParameterRectangle.width - 6
                                height: parameterText.implicitHeight + 4
                                //color: ListView.isCurrentItem ? "grey" : "white"
                                Text {
                                    id: parameterText
                                    text: name + " " + value
                                    font.family: "Helvetica"
                                    font.pointSize: 24
                                    //color: wrapper2.ListView.isCurrentItem ? "white" : "black"
                                    color: "black"
                                }
                            }
                }
            }
        }
    }
}

/*##^##
Designer {
    D{i:9;anchors_height:160;anchors_width:110}
}
##^##*/
