import QtQuick 2.5
import push2.enums 1.0
import "ObjectCreator.js" as ObjectCreator

Item {
    id: display
    width: 960
    height: 160


    Push2DeviceOverlay {
        id: push2DeciceOverlay
        Component.onCompleted:{
            push2DeciceOverlay.onReserveLedOfBtn("white", Button.E_BtnShift)

            push2DeciceOverlay.onReserveButton(Button.E_BtnT, FpWidget.E_All)
            push2DeciceOverlay.onReserveLedOfBtn("black", Button.E_BtnT, FpWidget.E_All);
            push2DeciceOverlay.onReserveButton(Button.E_BtnB, FpWidget.E_All)
            push2DeciceOverlay.onReserveLedOfBtn("black", Button.E_BtnB, FpWidget.E_All);

            push2DeciceOverlay.onReserveEncoder(Encoder.E_EncoderTempo)
            push2DeciceOverlay.onReserveEncoder(Encoder.E_EncoderMetronome)
        }
        onButtonPressed: {
            switch(id)
            {
		        case Button.E_BtnT:
                {
                    var _x = x + theInputPortListView.indexAt(0,0)
                    if(0 <= _x && _x < theInputPortListView.count)
                    {
                        theInputPortListView.currentIndex = _x
                        btnBOverlay.onReserveButton(Button.E_BtnB, FpWidget.E_All)
                    }
                    break
                }
                case Button.E_BtnB:
                {
                    var _x = x + theOutputPortListView.indexAt(0,0)
                    if(0 <= _x && _x < theOutputPortListView.count)
                    {
                        midiOpenedOutputPortsModel.toggleRouted(_x)
                    }
                    break
                }
            }
        }
        onButtonReleased: {
            switch(id)
            {
		        case Button.E_BtnT:
                {
                    btnBOverlay.onReleaseButton(Button.E_BtnB, FpWidget.E_All)
                }
            }
        }
        onEncoderIncrementChanged: {
            switch(id)
            {
                case Encoder.E_EncoderTempo:
                {
                    var idx = theInputPortListView.indexAt(theInputPortListView.contentX,1)
                    theInputPortListView.positionViewAtIndex(idx + increment, ListView.Beginning);
                    break
                }
                case Encoder.E_EncoderMetronome:
                {
                    var idx = theOutputPortListView.indexAt(theOutputPortListView.contentX,1)
                    theOutputPortListView.positionViewAtIndex(idx + increment, ListView.Beginning);
                    break
                }
            }
        }
    }
    Push2DeviceOverlay {
        id: btnBOverlay
        onButtonPressed: {
	       switch(id)
           {
               case Button.E_BtnB:
               {
                    var _x = x + theOutputPortListView.indexAt(0,0)
                    if(0 <= _x && _x < theOutputPortListView.count)
                    {
                        ObjectCreator.createSelfDestroyingObject("MidiRouteSpecializingScene.qml",
                                                                 display,
                                                                 {x: 0, y: 0, sourceDeviceIndex: theInputPortListView.currentIndex,
                                                                              destinationDeviceIndex: _x})
                    }
                    break
               }
           }
        }
    }

    Rectangle {
        anchors.fill: parent
        color: "black"

        ListView {
            id: theInputPortListView
            anchors.fill: parent
            model: midiOpenedInputPortsModel
            orientation: Qt.Horizontal
            spacing: 4
            currentIndex: -1
            highlightFollowsCurrentItem: true
            delegate: midiInPortDelegate

            onCurrentIndexChanged:{
                midiOpenedOutputPortsModel.setCurrentIndex(currentIndex)
            }
            Component.onCompleted:{
                currentIndex = midiOpenedOutputPortsModel.currentIndex
            }
        }
        Component{
            id: midiInPortDelegate
            Rectangle {
                id: wrapper
                height: display.height / 2
                width: (display.width / 8) - ListView.view.spacing
                color: ListView.isCurrentItem?"green":"black"
                radius: 5
                
                Text {
                    anchors.fill: parent
                    text: name
                    color: wrapper.ListView.isCurrentItem?"white":"grey"
                    font.pointSize: 10
                    wrapMode: Text.WordWrap
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
                Rectangle{
                    color: "black"
                    anchors.fill: parent
                    anchors.margins: 3
                    Image {
                        source: picturepath
                        anchors.fill: parent
                        opacity: wrapper.ListView.isCurrentItem ? 1.0 : 0.1
                    }
                }
            }
        }
        ListView {
            id: theOutputPortListView
            width: parent.width
            height: display.height / 2
            anchors.bottom: parent.bottom
            model: midiOpenedOutputPortsModel
            orientation: Qt.Horizontal
            spacing: 4
            currentIndex: -1
            delegate: midiOutPortDelegate
        }
        Component{
            id: midiOutPortDelegate
            Rectangle {
                id: wrapper
                height: display.height / 2
                width: (display.width / 8) - ListView.view.spacing
                color: selected?"green":"black"
                radius: 5
                Text {
                    anchors.fill: parent
                    text: name
                    color: selected?"white":"grey"
                    font.pointSize: 10
                    wrapMode: Text.WordWrap
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
                Rectangle{
                    color: "black"
                    anchors.fill: parent
                    anchors.margins: 3
                    Image {
                        source: picturepath
                        anchors.fill: parent
                        opacity: selected ? 1.0 : 0.1
                    }
                }           
                Rectangle{
                    visible: hasSpecializedChannelMapping
                    x: 10
                    y: 10
                    width: 20
                    height: 20
                    radius: 5
                    color: selected?"green":"darkgreen"
                    Text {
                        text : "S"
                        font.pointSize: 10
                        color: selected?"white":"gray"
                        anchors.fill: parent
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                }
            }
        }
    }
}
