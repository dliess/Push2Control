import QtQuick 2.5
import push2.enums 1.0
import MappingCurveDrawing 1.0
import "ListViewHelper.js" as ListViewHelper

Rectangle {
    id: display
    width: 960
    height: 160
    color: "black"

    Component.onCompleted:{
        push2Device.onSetLedOfBtn("#AA0000", Button.E_BtnT, 0)
        push2Device.onSetLedOfBtn("#AA0000", Button.E_BtnT, 1)
        push2Device.onSetLedOfBtn("#AA0000", Button.E_BtnT, 2)
    }
    Component.onDestruction:{
        push2Device.onSetLedOfBtn("#000000", Button.E_BtnT, 0)
        push2Device.onSetLedOfBtn("#000000", Button.E_BtnT, 1)
        push2Device.onSetLedOfBtn("#000000", Button.E_BtnT, 2)
    }
    Connections {
        target: push2Device
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
                    var _x = x + ListViewHelper.getListViewStartIndex(controllerDeviceParameterListView)
                    if(0 <= _x && _x < controllerDeviceParameterListView.count)
                    {
                        controllerDeviceParameterListView.currentIndex = _x
                    }
                    break
                }
            }
        }
        onEncoderIncrementChanged: {
            switch(id)
            {
                case Encoder.E_EncoderTempo:
                {
                    ListViewHelper.incrementListViewStartIndex(controllerDeviceParameterListView, increment)
                    break
                }
                case Encoder.E_Encoder:
                {
                    switch(x)
                    {
                        case 0:
                        {
                            controllerDeviceParameter.incrementValueMapOutAtMinAmount(controllerDeviceParameterListView.currentIndex, increment * 0.01)
                            break
                        }
                        case 1:
                        {
                            controllerDeviceParameter.incrementValueMapOutAtMaxAmount(controllerDeviceParameterListView.currentIndex, increment * 0.01)
                            break
                        }
                        case 2:
                        {
                            controllerDeviceParameter.incrementValueMapBendAmount(controllerDeviceParameterListView.currentIndex, increment * 0.01)
                            break
                        }
                    }
                    break
                }
            }
        }
    }


    GridView {
        id: parameterCategoryGridView
        width: display.width
        height: display.height
        cellWidth: display.width / 8
        cellHeight: display.height / 2

        flow: GridView.FlowLeftToRight

        model: controllerDeviceParameter.parameterCategoryModel
        delegate: parameterCategoryDelegate
        highlightFollowsCurrentItem: true
        currentIndex: -1
        onCurrentIndexChanged:{
            controllerDeviceParameter.setCurrentCategoryIndex(currentIndex)
        }
        Component.onCompleted:{
            currentIndex = controllerDeviceParameter.currentCategoryIndex
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
        id: myMappingCurve
        MappingCurveDrawing {
            width: 70
            height: 70

            backgroundColor: "black"
            axisColor: "white"
            curveColor: "#222200"
            activePointColor: "blue"
        }
    }

    Component{
        id: notMappedText
        Text{
            y: 30
            text: "Not\n\rmapped"
            color: "yellow"
            font.pointSize:12
             font.italic : true
        }
    }


    ListView {
        id: controllerDeviceParameterListView
        width: display.width
        height: display.height * 3 / 4
        y: 40
        model: controllerDeviceParameter.parameterModel
        orientation: Qt.Horizontal
        spacing: 0
        delegate: controllerDeviceParameterDelegate
        currentIndex: 0
        onCurrentIndexChanged:{
            controllerDeviceParameter.setCurrentParameterInCategoryIndex(currentIndex)
        }
        Component.onCompleted:{
            //currentIndex = controllerDeviceParameter.currentParameterInCategoryIndex
        }
    }
    Component{
        id: controllerDeviceParameterDelegate
        Rectangle {
            id: theRect
            //height: display.height / 2
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            width: display.width / 8
            color: "black"
            //border.color: ListView.isCurrentItem ? "green" : "black"
            //border.width: ListView.isCurrentItem ? 2 : 0
            radius: 7
            gradient: Gradient {
                GradientStop { position: 0.7; color: theRect.ListView.isCurrentItem ? "black" : "black" }
                GradientStop { position: 1.0; color: theRect.ListView.isCurrentItem ? "blue" : "black" }
            }
            Column
            {
                anchors.fill: parent
                anchors.margins: 3
                spacing: 3
                Text {
                    text: name
                    color: "white"
                    font.pointSize: 8
                }
                Text {
                    text: destinationParameter ? " "+destinationParameter : ""
                    color: "yellow"
                    font.pointSize: 10
                }
                /*
                Text {
                    text: type == "continous" ? value.toFixed(2) : value
                    color: "blue"
                    height: type == "continous" ? 10 : 90
                    font.pointSize: type == "continous" ? 10 : 10
                    horizontalAlignment: type == "continous" ? Text.AlignLeft : Text.AlignHCenter
                    verticalAlignment: type == "continous" ? Text.AlignTop : Text.AlignVCenter
                }
                */
                Loader {
                    id: mappingCurveLoader
                    property real bendValue: mappingCurve ? mappingCurve.bendValue : 0
                    property real outAtMax: mappingCurve ? mappingCurve.outAtMax : 0
                    property real outAtMin: mappingCurve ? mappingCurve.outAtMin : 0
                    property real inValue : mappingCurve ? value : 0
                    x: 20
                    sourceComponent: {
                        if(mappingCurve)
                        {
                            return myMappingCurve
                        }
                        else
                        {
                            return notMappedText
                        }
                    }
                    onLoaded:{
                        if(mappingCurve && item)
                        {
                            item.setMappingCurve(mappingCurve)
                            item.setInValue(inValue)
                        }
                    }
                    onBendValueChanged:{
                        if(mappingCurve && item)
                        {
                            item.setMappingCurve(mappingCurve)
                        }
                    }
                    onOutAtMaxChanged:{
                        if(mappingCurve && item)
                        {
                            item.setMappingCurve(mappingCurve)
                        }
                    }
                    onOutAtMinChanged:{
                        if(mappingCurve && item)
                        {
                            item.setMappingCurve(mappingCurve)
                        }
                    }
                    onInValueChanged: {
                        if(mappingCurve && item)
                        {
                            item.setInValue(inValue)
                        }
                    }
                }
            }
        }
    }
    Text {
        x: 40
        y: 60
        visible: controllerDeviceParameterListView.count == 0
        text: "No Controller-device parameters available for setup"
        color: "white"
        wrapMode: Text.WordWrap
        font.pointSize: 20
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignTop
    }
}