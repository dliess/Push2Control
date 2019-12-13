import QtQuick 2.5
import push2.enums 1.0
import ValueCircle 1.0
import "ObjectCreator.js" as ObjectCreator

Rectangle {
    id: display
    width: 960
    height: 160
    color: "black"

    Push2DeviceOverlay {
        id: push2DeciceOverlay

        Component.onCompleted:{
            push2DeciceOverlay.onReserveLedOfBtn("white", Button.E_BtnConvert)
            push2DeciceOverlay.onReserveButton(Button.E_BtnT, FpWidget.E_All)
            push2DeciceOverlay.onReserveLedOfBtn("black", Button.E_BtnT, FpWidget.E_All)
            push2DeciceOverlay.onReserveButton(Button.E_BtnB, FpWidget.E_All)
            push2DeciceOverlay.onReserveLedOfBtn("black", Button.E_BtnB, FpWidget.E_All)
            push2DeciceOverlay.onReserveButton(Button.E_BtnRightArrow)
            push2DeciceOverlay.onReserveLedOfBtn("white", Button.E_BtnRightArrow)
            push2DeciceOverlay.onReserveButton(Button.E_BtnLeftArrow)
            push2DeciceOverlay.onReserveLedOfBtn("white", Button.E_BtnLeftArrow)

            push2DeciceOverlay.onReserveEncoder(Encoder.E_EncoderTempo)
            push2DeciceOverlay.onReserveEncoder(Encoder.E_EncoderMetronome)
            push2DeciceOverlay.onReserveEncoder(Encoder.E_Encoder, FpWidget.E_All)
            handleArrowColors()
        }
        function handleArrowColors(){
            var idx = soundDeviceParameterListView.indexAt(soundDeviceParameterListView.contentX,1)           
            if(idx >= soundDeviceParameterListView.count - 8){
                push2DeciceOverlay.onSetLedOfBtn("black", Button.E_BtnRightArrow)
            }
            else
            {
                push2DeciceOverlay.onSetLedOfBtn("white", Button.E_BtnRightArrow)
            }
            if(idx == 0){
                push2DeciceOverlay.onSetLedOfBtn("black", Button.E_BtnLeftArrow)
            }
            else{
                push2DeciceOverlay.onSetLedOfBtn("white", Button.E_BtnLeftArrow)
            }
        }
        onButtonPressed: {
            switch(id)
            {
                case Button.E_BtnT:
                {
                    if(0 <= x && x < parameterCategoryGridView.count)
                    {
                        parameterCategoryGridView.currentIndex = x
                    }
                    break
                }
                case Button.E_BtnB:
                {
                    var _x = x + 8
                    if(0 <= _x && _x < parameterCategoryGridView.count)
                    {
                        parameterCategoryGridView.currentIndex = x
                    }
                    break
                }
                case Button.E_BtnRightArrow:
                {
                    var idx = soundDeviceParameterListView.indexAt(soundDeviceParameterListView.contentX,1)
                    idx += 8
                    soundDeviceParameterListView.positionViewAtIndex(idx, ListView.Beginning);
                    break
                }
                case Button.E_BtnLeftArrow:
                {
                    var idx = soundDeviceParameterListView.indexAt(soundDeviceParameterListView.contentX,1)
                    idx = Math.max(0, idx - 8)
                    soundDeviceParameterListView.positionViewAtIndex(idx, ListView.Beginning);
                    break
                }
            }
        }
        onEncoderIncrementChanged: {
            switch(id)
            {
                case Encoder.E_EncoderMetronome:
                {
                    break
                }
                case Encoder.E_EncoderTempo:
                {
                    break
                }
                case Encoder.E_Encoder:
                {
                    var idx = soundDeviceParameterListView.indexAt(soundDeviceParameterListView.contentX,1)
                    soundDeviceParameter.incrementParameterValue(idx + x, increment);
                    break
                }
            }
        }
        onEncoderTouchStateChanged: {
            if(EncoderTouchState.Touched == state) {
                switch(id)
                {
                    case Encoder.E_EncoderTempo:
                    {
                        ObjectCreator.createSelfDestroyingObject("SoundPresetScene.qml",  display, {})
                        break
                    }
                    case Encoder.E_Encoder:
                    {
                        if(ButtonPressState.Pressed == push2Device.getButtonState(Button.E_BtnConvert))
                        {
                            var idx = soundDeviceParameterListView.indexAt(soundDeviceParameterListView.contentX,1)           
                            soundDeviceParameter.learnControl(idx + x)
                        }
                        break
                    }
                }

            }
        }
    }

    GridView {
        id: parameterCategoryGridView
        width: display.width * 6 / 8
        height: display.height
        cellWidth: display.width / 8
        cellHeight: display.height / 2

        flow: GridView.FlowLeftToRight

        model: soundDeviceParameter.parameterCategoryModel
        delegate: parameterCategoryDelegate
        highlightFollowsCurrentItem: true
        currentIndex: -1
        onCurrentIndexChanged:{
            soundDeviceParameter.setCurrentCategoryIndex(currentIndex)
        }
        Component.onCompleted:{
            currentIndex = soundDeviceParameter.currentCategoryIndex
        }
    }
    Component {
        id: parameterCategoryDelegate
        Rectangle {
            id: parameterCategoryDelegateRect
            color: "black"
            height: display.height / 2
            width: display.width / 8
            radius: 5
            Text {
                anchors.fill: parent
                text: name
                color: parameterCategoryDelegateRect.GridView.isCurrentItem ? "white" : "#222222"
                font.pointSize: parameterCategoryDelegateRect.GridView.isCurrentItem ? 12 : 10
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: index < 8 ? Text.AlignTop : Text.AlignBottom
            }
        }
    }

    Component{
        id: myValueCircle
        ValueCircle {
            width: 70
            height: 70

            // Determine the properties that Q_PROPERTY
            name: "clock"
            backgroundColor: "black"
            borderActiveColor: "red"
            borderNonActiveColor: "#220000"
        }
    }

    ListView {
        id: soundDeviceParameterListView
        width: display.width
        height: display.height / 2
        y: 30
        model: soundDeviceParameter.parameterModel
        orientation: Qt.Horizontal
        spacing: 0
        delegate: soundDeviceParameterDelegate
        onContentXChanged:{
            push2DeciceOverlay.handleArrowColors()
        }
        onCurrentItemChanged:{
            push2DeciceOverlay.handleArrowColors()
        }
    }
    Component{
        id: soundDeviceParameterDelegate
        Rectangle {
            height: display.height / 2
            width: display.width / 8
            color: "black"
            Column
            {
                anchors.fill: parent
                Text {
                    text: name
                    color: extControlLearnState ? "red" : "white"
                    font.pointSize: 8
                }
                Text {
                    text: type == "continous" ? value.toFixed(2) : value
                    color: extControlLearnState ? "red" : "blue"
                    height: type == "continous" ? 10 : 90
                    font.pointSize: type == "continous" ? 10 : 10
                    horizontalAlignment: type == "continous" ? Text.AlignLeft : Text.AlignHCenter
                    verticalAlignment: type == "continous" ? Text.AlignTop : Text.AlignVCenter
                }
                               
                Loader {
                    id: valueCircleLoader
                    property real angle: type == "continous" ? value : 0
                    x: 20
                    sourceComponent: {
                        switch(type) {
                            case "continous": return myValueCircle;
                            case "list":
                            default: break;
                        }
                    }
                    onAngleChanged: {
                        switch(type) {
                            case "continous":
                                if(item){
                                item.setAngle(value * 365);
                                }
                                break;
                            case "list":
                            default: break;
                        } 
                    }
                    onLoaded:{
                        switch(type) {
                            case "continous":
                                item.setAngle(value * 365)
                                break
                            case "list":
                            default: break;
                        } 
                    }
                }
            }
        }
    }
    
    Text {
        x: 40
        y: 60
        visible: soundDeviceParameterListView.count == 0
        text: "No Sound-device parameters available for control"
        color: "white"
        wrapMode: Text.WordWrap
        font.pointSize: 20
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignTop
    }
}