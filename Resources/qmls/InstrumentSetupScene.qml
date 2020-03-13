import QtQuick 2.5
import push2.enums 1.0
//import "ListViewHelper.js" as ListViewHelper

Item {
    id: display
    width: 960
    height: 160

    Push2DeviceOverlay {
    id: push2DeciceOverlay
        Component.onCompleted:{
            push2DeciceOverlay.onReserveEncoder(Encoder.E_Encoder, FpWidget.E_All)
        }
        onEncoderIncrementChanged: {
            switch(id)
            {
                case Encoder.E_Encoder:
                {
                    switch(x)
                    {
                        case 0:
                        {
                            if(increment > 0)
                            {
                                listView1.incrementCurrentIndex()
                            }
                            else
                            {
                                listView1.decrementCurrentIndex()
                            }
                            break
                        }
                        case 1:
                        {
                            if(increment > 0)
                            {
                                listView2.incrementCurrentIndex()
                            }
                            else
                            {
                                listView2.decrementCurrentIndex()
                            }
                            break
                        }
                        case 2:
                        {
                            if(increment > 0)
                            {
                                listView3.incrementCurrentIndex()
                            }
                            else
                            {
                                listView3.decrementCurrentIndex()
                            }
                            break
                        }
                    }
                    break
                }
            }
        }
    }

    Rectangle {
        anchors.fill: parent
        color: "black"
  
        Row {
            anchors.fill: parent
            ListView {
                id: listView1
                width: parent.width / 8
                height: parent.height
                model: ListModel {
                    ListElement {
                        name: "Drum Kits"
                    }
                    ListElement {
                        name: "Melodic Instruments"
                    }
                }
                delegate: Text {
                    text: name
                    color: ListView.isCurrentItem ? "black" : "white"
                    font.pointSize: 10
                }
                highlight: Rectangle {
                    y: listView1.currentItem.y
                    width: listView1.width
                    height: 20
                    color: "lightsteelblue";
                    //Behavior on y { SpringAnimation { spring: 2; damping: 0.2 } }
                    radius: 3
                }
                highlightFollowsCurrentItem: false
                onCurrentIndexChanged: {
                    switch(currentIndex)
                    {
                        case 0: { listView2.model = instruments.kitInstruments(); break; }
                        case 1: { listView2.model = instruments.melodicInstruments(); break; }
                    }
                }
                Component.onCompleted:{
                    currentIndex = 1
                }

            }
            ListView {
                id: listView2
                width: parent.width / 8
                height: parent.height
                delegate: Text {
                    text: name
                    color: ListView.isCurrentItem ? "black" : "white"
                    font.pointSize: 10
                }
                highlight: Rectangle {
                    y: listView2.currentItem ? listView2.currentItem.y : 0
                    width: listView2.width
                    height: 20
                    color: "lightsteelblue";
                    //Behavior on y { SpringAnimation { spring: 2; damping: 0.2 } }
                    radius: 3
                }
                highlightFollowsCurrentItem: false
            }
        }
    }
}