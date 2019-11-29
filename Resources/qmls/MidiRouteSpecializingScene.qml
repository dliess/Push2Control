import QtQuick 2.5
import push2.enums 1.0

Item {
    id: root
    width: 960
    height: 160
    property int sourceDeviceIndex
    property int destinationDeviceIndex

    Push2DeviceOverlay {
        id: push2DeciceOverlay
        Component.onCompleted:{
            midiChannelMappingBridge.activate(sourceDeviceIndex, destinationDeviceIndex)
            midiDumperModel.setMidiInputDeviceIdx(sourceDeviceIndex)
            push2DeciceOverlay.onReserveLedOfBtn("black", Button.E_BtnShift)
            push2DeciceOverlay.onReserveButton(Button.E_BtnT, FpWidget.E_All)
            push2DeciceOverlay.onReserveLedOfBtn("black", Button.E_BtnT, FpWidget.E_All);
            push2DeciceOverlay.onSetLedOfBtn("blue", Button.E_BtnT, 7)
            push2DeciceOverlay.onReserveButton(Button.E_BtnB, FpWidget.E_All)
            push2DeciceOverlay.onReserveLedOfBtn("black", Button.E_BtnB, FpWidget.E_All);
            push2DeciceOverlay.onReserveEncoder(Encoder.E_Encoder, FpWidget.E_All)
        }
        Component.onDestruction:{
            midiDumperModel.setMidiInputDeviceIdx(-1)
            midiChannelMappingBridge.deActivate();
        }
        onButtonPressed: {
            switch(id)
            {
		        case Button.E_BtnT:
                {
                    switch(x)
                    {
                        case 7: root.destroy(); break
                        case 6: midiChannelMappingBridge.toggleChannelMappingActive(); break;
                    }
                }
            }
        }
        onEncoderIncrementChanged: {
            switch(id)
            {
                case Encoder.E_Encoder:
                {
                    if(3 == x)
                    {
                        var newIdx = midiDumpListView.currentIndex + increment
                        if(newIdx < 0 || newIdx >= midiDumpListView.count) return
                        midiDumpListView.currentIndex = newIdx
                    }
                }
            }
        }
    }

    Item {
        id: channelMappigHandler
        property int currentSourceChannel
        Component.onCompleted:{ currentSourceChannel = -1 }
        Connections{
            id: channelMappigHandlerConnections
            target: midiChannelMappingBridge
            onChannelMappingActiveChanged: {
                if(midiChannelMappingBridge.channelMappingActive)
                {
                    push2DeciceOverlay.onReserveButton3d(Button3d.E_BtnSil, FpWidget.E_All, 0)
                    push2DeciceOverlay.onReserveLed("black", Led.E_BtnSilLed, FpWidget.E_All, 0)
                    push2DeciceOverlay.onReserveButton3d(Button3d.E_BtnSil, FpWidget.E_All, 1)
                    push2DeciceOverlay.onReserveLed("black", Led.E_BtnSilLed, FpWidget.E_All, 1)
                    push2DeciceOverlay.onReserveButton3d(Button3d.E_BtnSil, FpWidget.E_All, 2)
                    push2DeciceOverlay.onReserveLed("black", Led.E_BtnSilLed, FpWidget.E_All, 2)
                    push2DeciceOverlay.onReserveButton3d(Button3d.E_BtnSil, FpWidget.E_All, 3)
                    push2DeciceOverlay.onReserveLed("black", Led.E_BtnSilLed, FpWidget.E_All, 3)
                    channelMappigHandler.currentSourceChannel = 1 // What a workaround :) gotta make that better
                    channelMappigHandler.currentSourceChannel = 0
                }
                else
                {
                    push2DeciceOverlay.onReleaseButton3d(Button3d.E_BtnSil, FpWidget.E_All, 0)
                    push2DeciceOverlay.onReleaseLed(Led.E_BtnSilLed, FpWidget.E_All, 0)
                    push2DeciceOverlay.onReleaseButton3d(Button3d.E_BtnSil, FpWidget.E_All, 1)
                    push2DeciceOverlay.onReleaseLed(Led.E_BtnSilLed, FpWidget.E_All, 1)
                    push2DeciceOverlay.onReleaseButton3d(Button3d.E_BtnSil, FpWidget.E_All, 2)
                    push2DeciceOverlay.onReleaseLed(Led.E_BtnSilLed, FpWidget.E_All, 2)
                    push2DeciceOverlay.onReleaseButton3d(Button3d.E_BtnSil, FpWidget.E_All, 3)
                    push2DeciceOverlay.onReleaseLed(Led.E_BtnSilLed, FpWidget.E_All, 3)
                }
            }
            onChannelMappingSpecializationChanged: {
                var mapping = midiChannelMappingBridge.getMappingFor(channelMappigHandler.currentSourceChannel)
                var i;
                for (i = 0; i < 16; i++)
                {
                    var color = (mapping & (1 << i)) ? "green" : "black"
                    push2DeciceOverlay.onSetLed(color, Led.E_BtnSilLed, i % 8, 2 + (i / 8))
                }             
            }
        }
        onCurrentSourceChannelChanged: {
            var i
            for (i = 0; i < 16; i++)
            {
                var mapping = midiChannelMappingBridge.getMappingFor(i)
                var color = mapping ? "#111111" : "black"
                push2DeciceOverlay.onSetLed(color, Led.E_BtnSilLed, i % 8, (i / 8))
            }             
            push2DeciceOverlay.onSetLed("green", Led.E_BtnSilLed, currentSourceChannel % 8, currentSourceChannel / 8)
            midiChannelMappingBridge.channelMappingSpecializationChanged()
        }
        Connections{
            target: push2DeciceOverlay
            onButton3dPressed: {
                switch(id)
                {
                    case Button3d.E_BtnSil:
                    {
                        switch(y)
                        {
                            case 0: 
                            {
                                channelMappigHandler.currentSourceChannel = x
                                break
                            } 
                            case 1:
                            {
                                channelMappigHandler.currentSourceChannel = x + 8
                                break
                            }
                            case 2:
                            {
                                midiChannelMappingBridge.toggleMappingForChannelIdx(channelMappigHandler.currentSourceChannel, x)
                                break
                            }
                            case 3:
                            {
                                midiChannelMappingBridge.toggleMappingForChannelIdx(channelMappigHandler.currentSourceChannel, x + 8)
                                break
                            } 
                        }
                        break
                    }
                }
            }
        }
    }


    Rectangle{
        anchors.fill: parent
        color: "black"
    }

    Row {
        layoutDirection : Qt.RightToLeft
        anchors.top : parent.top
        anchors.right : parent.right
        Rectangle {
            height: 40
            width: (root.width / 8)
            color: "black"
            border.color: "blue"
            border.width: 2
            radius: 5
            Canvas {
                id: backArrow
                anchors.fill: parent
                onPaint: {
                    var ctx = getContext("2d")
                    ctx.lineWidth = 3
                    ctx.strokeStyle = "blue"
                    ctx.fillStyle = "blue"
                    ctx.beginPath()
                    ctx.moveTo(40, height * 0.8)
                    ctx.lineTo(width - 50, height * 0.8)
                    ctx.arc(width - 50, height * 0.5, height * 0.3,  Math.PI / 2, -Math.PI / 2, true)
                    ctx.lineTo(40, height * 0.2)
                    ctx.stroke()
                    ctx.beginPath()
                    ctx.moveTo(40, height * 0.2)
                    ctx.lineTo(52, height * 0.35)
                    ctx.lineTo(52, height * 0.05)
                    ctx.fill();
                }
            }
        }
        Rectangle {
            height: 40
            width: (root.width / 8)
            color: "black"
            border.color: "blue"
            border.width: 2
            radius: 5
            Text {
                anchors.fill: parent
                text: "channel-mapping"
                color: "grey"
                font.pointSize: 8
                wrapMode: Text.WordWrap
                horizontalAlignment: Text.AlignHCenter
                anchors.top : parent.top
            }
            Rectangle {
                anchors.horizontalCenter : parent.horizontalCenter
                y: 16
                width: 19
                height: 19
                color: midiChannelMappingBridge.channelMappingActive ? "blue" : "black"
                border.color: "blue"
                border.width: 2
            }
        }
    }

    Row{       
        anchors.verticalCenter : parent.verticalCenter
        spacing: 0
        Image {
            height: root.height / 2
            width: root.width / 8
            anchors.verticalCenter : parent.verticalCenter
            source: midiOpenedInputPortsModel.getDevPicturePath(root.sourceDeviceIndex)
        }
        Rectangle {
            width: root.width / 8
            height: root.height
            color: "black"

            Canvas {
                id: arrow
                width: 60
                height: 20
                anchors.verticalCenter : parent.verticalCenter
                anchors.horizontalCenter : parent.horizontalCenter

                onPaint: {
                    var ctx = getContext("2d")
                    ctx.lineWidth = 3
                    ctx.strokeStyle = "blue"
                    ctx.beginPath()
                    ctx.moveTo(0,10)
                    ctx.lineTo(50,10)
                    ctx.lineTo(30,14)
                    ctx.moveTo(50,10)
                    ctx.lineTo(30,6)
                    ctx.stroke()
                }
            }
        }
        Image {
            height: root.height / 2
            width: root.width / 8
            anchors.verticalCenter : parent.verticalCenter
            source: midiOpenedOutputPortsModel.getDevPicturePath(root.destinationDeviceIndex)
        }
        Rectangle {
            height: root.height * 0.9
            width: root.width / 8
            anchors.verticalCenter : parent.verticalCenter
            //border.color: "white"
            //border.width: 2
            color: "black"
            
            ListView {
                id: midiDumpListView
                anchors.fill: parent
                model: midiDumperModel
                delegate: midiDumpListViewDelegate
                highlightFollowsCurrentItem: true
                //focus: true
                clip:true
                /*
                highlight: Rectangle { color: "lightsteelblue"; radius: 5 }
                highlightMoveDuration: 1000
                highlightMoveVelocity: -1
                */

                onCountChanged: {
                    var newIndex = count - 1 // last index
                    positionViewAtEnd()
                    currentIndex = newIndex
                }


                Component {
                    id: midiDumpListViewDelegate
                    Rectangle {
                        id: wrapper
                        width: midiDumpListView.width
                        height: midiDumpText.implicitHeight
                        //border.color: "white"
                        //border.width: 1
                        color: ListView.isCurrentItem ? "gray" : "black"
                        radius: 3
                        Text {
                            anchors.horizontalCenter: parent.horizontalCenter
                            id: midiDumpText
                            text: name
                            //color: "white"
                            color: wrapper.ListView.isCurrentItem ? "black" : "gray"
                            font.pointSize: wrapper.ListView.isCurrentItem ?  10 : 8
                            font.weight: wrapper.ListView.isCurrentItem ? Font.ExtraBold : Font.Thin
                            horizontalAlignment: Text.AlignHCenter
                        }
                    }

                }       
            }
            Rectangle{
                visible: midiDumperModel.midiClockReceived
                anchors.top: parent.top
                anchors.right: parent.right
                width: 20
                height: 20
                radius: 5
                color: "green"
                Text {
                    text : "C"
                    font.pointSize: 10
                    color: "white"
                    anchors.fill: parent
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
            }
        }
    }
}