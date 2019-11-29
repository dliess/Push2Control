import QtQuick 2.5
import push2.enums 1.0

Item {
    id: display
    width: 960
    height: 160

    Component.onCompleted:{
        if(transportControl.started)
        {
            push2Device.onSetLedOfBtn("#00FF00", Button.E_BtnT, 0)
            push2Device.onSetLedOfBtn("#00AA00", Button.E_BtnT, 1)
            push2Device.onSetLedOfBtn("#007700", Button.E_BtnT, 2)
        }
        else
        {
            push2Device.onSetLedOfBtn("#FFFFFF", Button.E_BtnT, 0)
            push2Device.onSetLedOfBtn("#AAAAAA", Button.E_BtnT, 1)
            push2Device.onSetLedOfBtn("#777777", Button.E_BtnT, 2)
        }
    }
    Component.onDestruction:{
        push2Device.onSetLedOfBtn("black", Button.E_BtnT, 0)
        push2Device.onSetLedOfBtn("black", Button.E_BtnT, 1)
        push2Device.onSetLedOfBtn("black", Button.E_BtnT, 2)
    }

    Timer {
        id: timer
    }

    function delay(delayTime, cb) {
        timer.interval = delayTime;
        timer.repeat = false;
        timer.triggered.connect(cb);
        timer.start();
    }
    
    Connections {
        target: tempoHandler
        onBpmChanged: {
            bpmText.opacity = 1.0
            delay(1000, function() { bpmText.opacity = 0 })
        }
    }

    Connections {
        target: transportControl
        onStartedChanged: {
            if(transportControl.started)
            {
                push2Device.onSetLedOfBtn("#00FF00", Button.E_BtnT, 0)
                push2Device.onSetLedOfBtn("#00AA00", Button.E_BtnT, 1)
                push2Device.onSetLedOfBtn("#007700", Button.E_BtnT, 2)
            }
            else
            {
                push2Device.onSetLedOfBtn("#FFFFFF", Button.E_BtnT, 0)
                push2Device.onSetLedOfBtn("#AAAAAA", Button.E_BtnT, 1)
                push2Device.onSetLedOfBtn("#777777", Button.E_BtnT, 2)
            }
        }
    }

    Connections {
        target: push2Device
        onButtonPressed: {
            switch(id)
            {
                case Button.E_BtnTapTempo:
                {
                    tapTempoHandler.tap()
                    break
                }
                case Button.E_BtnT:
                {
                    transportControl.toggleEnabled(x);
                    break
                }
                case Button.E_BtnB:
                {
                    transportControl.toggleEnabled(x + 8);
                    break
                }
            }
        }
        onEncoderIncrementChanged: {
            switch(id)
            {
                case Encoder.E_Encoder:
                {
                    switch(x)
                    {
                        case 0: tempoHandler.incBpm(1.0  * increment); break
                        case 1: tempoHandler.incBpm(0.1  * increment); break
                        case 2: tempoHandler.incBpm(0.01 * increment); break
                    }
                    break
                }
            }
        }
        onEncoderTouchStateChanged: {
            switch(id)
            {
                case Encoder.E_Encoder:
                {
                    if(EncoderTouchState.Touched == state)
                    {
                        switch(x)
                        {
                            case 0: bpmText.opacity = 1.0; break
                            case 1: bpmText.opacity = 1.0; break
                            case 2: bpmText.opacity = 1.0; break
                        }
                    }
                    else
                    {
                        switch(x)
                        {
                            case 0: bpmText.opacity = 0; break
                            case 1: bpmText.opacity = 0; break
                            case 2: bpmText.opacity = 0; break
                        }
                    }
                    break
                }
            }
        }
    }
    Rectangle{
        anchors.fill: parent
        color: "black"
        GridView {
            id: startableDevicesGridView
            anchors.fill: parent
            cellWidth: display.width / 8
            cellHeight: display.height / 2
            flow: GridView.FlowLeftToRight
            model: transportControl.startableDevicesModel
            delegate: startableDevicesGridViewDelegate
        }
        Component {
            id: startableDevicesGridViewDelegate
            Rectangle {
                id: startableDevicesGridViewDelegateRect
                color: transportControl.started && deviceenabled ? "green" : "black"
                Behavior on color { PropertyAnimation { properties: "color"; easing.type: Easing.InOutQuad } }
                height: startableDevicesGridView.cellHeight
                width: startableDevicesGridView.cellWidth
                radius: 5
                Image{
                    id: deviceThumbnail
                    source: picturepath
                    anchors.fill: parent
                    anchors.margins: 3
                    opacity: deviceenabled ? 1.0 : 0.1
                }
                Text {
                    visible: { return !deviceenabled || picturepath == ""}
                    anchors.fill: parent
                    text: name
                    color: "white"
                    wrapMode: Text.WordWrap
                    font.pointSize: 8
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignTop
                }
            }
        }
    }
    Rectangle{
        opacity: bpmText.opacity
        color: "black"
        width: bpmText.implicitWidth
        height: bpmText.implicitHeight
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        radius: 5
        Text {
            id: bpmText
            opacity: 0
            anchors.fill: parent
            text: tempoHandler.bpm
            color: transportControl.started ? "green" : "white"
            font.pointSize: 20
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignTop
            Behavior on opacity { PropertyAnimation { properties: "opacity"; easing.type: Easing.InOutQuad } }
        }
    }
}