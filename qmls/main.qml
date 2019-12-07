
import QtQuick 2.5
import push2.enums 1.0

Item {
    width: 960
    height: 160

    Loader { id: pageLoader }

    Component.onCompleted:{
        push2Device.onSetLedOfBtn("white", Button.E_BtnSetup)
        push2Device.onSetLedOfBtn("black", Button.E_BtnAddDevice)
        push2Device.onSetLedOfBtn("black", Button.E_BtnAddTrack)
        push2Device.onSetLedOfBtn("black", Button.E_BtnLayout)
        push2Device.onSetLedOfBtn("white", Button.E_BtnDevice)
        push2Device.onSetLedOfBtn("white", Button.E_BtnBrowse)
        push2Device.onSetLedOfBtn("white", Button.E_BtnScale)
        push2Device.onSetLedOfBtn("black", Button.E_BtnClip)
        push2Device.onSetLedOfBtn("white", Button.E_BtnOctaveUp)
        push2Device.onSetLedOfBtn("white", Button.E_BtnOctaveDown)
        push2Device.onSetLedOfBtn("white", Button.E_BtnPlay)
        push2Device.onSetLedOfBtn("white", Button.E_BtnTapTempo)
        push2Device.onSetLedOfBtn("white", Button.E_BtnPageRight)
        push2Device.onSetLedOfBtn("white", Button.E_BtnPageLeft)
        push2Device.onSetLedOfBtn("black", Button.E_BtnT, 0)
        push2Device.onSetLedOfBtn("black", Button.E_BtnT, 1)
        push2Device.onSetLedOfBtn("black", Button.E_BtnT, 2)
        push2Device.onSetLedOfBtn("black", Button.E_BtnT, 3)
        push2Device.onSetLedOfBtn("black", Button.E_BtnT, 4)
        push2Device.onSetLedOfBtn("black", Button.E_BtnT, 5)
        push2Device.onSetLedOfBtn("black", Button.E_BtnT, 6)
        push2Device.onSetLedOfBtn("black", Button.E_BtnT, 7)

        push2Pads.activate();
        pageLoader.source = "qrc:///qmls/MidiPortMappingScene.qml"
    }
    Connections {
        target: push2Device
        onButtonPressed: {
            switch(id)
            {
                case Button.E_BtnSetup:
                {
                    pageLoader.source = "qrc:///qmls/MidiPortMappingScene.qml"
                    break
                }
                case Button.E_BtnScale:
                {
                    pageLoader.source = "qrc:///qmls/PadScaleConfigurationScene.qml"
                    break
                }
                case Button.E_BtnDevice:
                {
                    pageLoader.source = "qrc:///qmls/ControllerDeviceSelectionScene.qml"
                    break
                }
                case Button.E_BtnBrowse:
                {
                    pageLoader.source = "qrc:///qmls/SoundDeviceSelectionScene.qml"
                    break
                }
                case Button.E_BtnTapTempo:
                {
                    pageLoader.source = "qrc:///qmls/BpmAndTransportHandlingScene.qml"
                    break
                }
                case Button.E_BtnOctaveUp:
                {
                    push2Pads.incOctave()
                    break
                }
                case Button.E_BtnOctaveDown:
                {
                    push2Pads.decOctave()
                    break
                }
                case Button.E_BtnPlay:
                {
                    transportControl.toggleStartAllEnabled()
                    break
                }
                case Button.E_BtnPageRight:
                {
                    tempoHandler.setNudgeBpm(5.0);
                    break
                }
                case Button.E_BtnPageLeft:
                {
                    tempoHandler.setNudgeBpm(-5.0);
                    break
                }
                default:
                {
                    break
                }
            }
        }
        onButtonReleased: {
            switch(id)
            {
                case Button.E_BtnDevice:
                {
                    pageLoader.source = "qrc:///qmls/ControllerDeviceParameterScene.qml"
                    break
                }
                case Button.E_BtnBrowse:
                {
                    pageLoader.source = "qrc:///qmls/SoundDeviceParameterScene.qml"
                    break
                }
                case Button.E_BtnPageRight:
                {
                    tempoHandler.setNudgeBpm(0);
                    break
                }
                case Button.E_BtnPageLeft:
                {
                    tempoHandler.setNudgeBpm(0);
                    break
                }
            }
        }
    }
    Connections {
        target: transportControl
        onStartedChanged: {
            var btnLedColor = transportControl.started ? "green" : "white"
            push2Device.onSetLedOfBtn(btnLedColor, Button.E_BtnPlay)
        }
    }

    Connections{
        target: soundDeviceAndVoiceSelection
        onCurrentDeviceIndexChanged:{
            soundDeviceParameter.onCurrentDeviceIndexChanged(soundDeviceAndVoiceSelection.currentDeviceIndex)
        }
        onCurrentVoiceIndexChanged:{
            soundDeviceParameter.onCurrentVoiceIndexChanged(soundDeviceAndVoiceSelection.currentVoiceIndex)
        }
    }
    Connections{
        target: controllerDeviceAndPresetSelection
        onCurrentDeviceIndexChanged:{
            controllerDeviceParameter.onCurrentDeviceIndexChanged(controllerDeviceAndPresetSelection.currentDeviceIndex)
        }
        onCurrentPresetIndexChanged:{
            controllerDeviceParameter.onCurrentPresetIndexChanged(controllerDeviceAndPresetSelection.currentPresetIndex)
        }
    }
}