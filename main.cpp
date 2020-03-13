#include <loguru.hpp>
#include "Base.h"
#include "HomeDir.h"
#include "UsbMidiInputPortListProvider.h"
#include "UsbMidiOutputPortListProvider.h"
#include "UsbMidiPortNotifier.h"
#include "Midi1Input.h"
#include "Midi1Output.h"
#include "MusicDeviceHolder.h"
#include "MidiOpenedInputPortsModel.h"
#include "MidiOpenedOutputPortsModel.h"
#include "Push2Pads.h"
#include "QmlSceneLoader.h"
#include "DeviceDescriptionLoader.h"
#include "SelectedSoundDevice.h"
#include "MidiRouter.h"
#include "MidiDumper.h"
#include "MidiDumperModel.h"
#include "MidiChannelMappingBridge.h"
#include "ParameterMapper.h"

#ifdef __SIMULATION__MODE__
#include "FpSimulation.h"
#endif
#include "Push2Device.h"
#include "Push2Topology.h"
#include "Push2LedOfWidget.h"

#include "FpButton.h"

#include "QuickViewOnFpRenderMedium.h"
#include "ValueCircle.h"
#include "MappingCurveDrawing.h"
#include "MappingCurveQt.h"

#include "Push2DeviceQtAdapter.h"
#include "Push2DeviceOverlay.h"

#include "InstrumentsBridge.h"
#include "MusicScalesModel.h"
#include "MusicScalesBaseNoteModel.h"
#include "Push2PadsBridge.h"
#include "SoundDeviceSelectionModel.h"
#include "VoiceSelectionModel.h"
#include "SoundDeviceAndVoiceSelection.h"
#include "ControllerDeviceAndPresetSelection.h"
#include "SoundDeviceParameter.h"
#include "ControllerDeviceParameter.h"
#include "TempoHandler.h"
#include "TempoHandlerBridge.h"
#include "TapTempoHandler.h"
#include "TapTempoHandlerBridge.h"
#include "TransportControl.h"
#include "TransportControlBridge.h"
#include "SettingsSaverBridge.h"
#include "sigwatch.h"
#include "ThreadedLoop.h"

#include <unistd.h>
#include <iostream>
#include <QGuiApplication>
#include <QTimer>
#include <cmath>

//TODO remove later
#include <QQmlContext>

/*
#include <csignal>
void signalHandler(int id)
{
    printf("*********- SIGNAL Caught ********* %d\n", id);
}
*/

int main(int argc, char *argv[])
{
    util::setAppName("push2Control");
    loguru::init(argc, argv);
    loguru::add_file("/tmp/push2Control.log", loguru::Truncate, loguru::Verbosity_MAX);
//    std::signal(SIGINT, signalHandler); // overwrite loggers signal handler
    if(!midi::PortNotifiers::instance().init())
    {
        return -1;
    }

#ifdef __SIMULATION__MODE__
    push2device::Push2Device push2Device("127.0.0.1:50051");
#else
    push2device::Push2Device push2Device;
#endif
    base::Base base;
    auto pPush2Pads = std::make_unique<push2::Pads>(push2Device);
    auto &push2Pads = *pPush2Pads.get();
    base.musicDeviceFactory.addMidiInputMedium(std::move(pPush2Pads));
#ifdef __INSERT_DUMMY_MIDI_DEVICES__
    base.musicDeviceFactory.insertMusicDeviceDummies();
#endif
    midi::PortNotifiers::instance().update();    
#ifndef __SIMULATION__MODE__
    push2Device.init(1000);
#endif

    InstrumentsBridge instrumentsBridge(base.instruments);

    util::ThreadedLoop midiOutThread([&base](){
        auto start = std::chrono::high_resolution_clock::now();
        for(auto& musicDevice : base.musicDeviceHolder.musicDevices){
            musicDevice.second->processMidiOutBuffers();
        }
        auto end = std::chrono::high_resolution_clock::now();
        const std::chrono::nanoseconds diff = end - start;
        const std::chrono::nanoseconds PERIOD(1000000);
        if(diff < PERIOD)
        {
            std::this_thread::sleep_for(PERIOD - diff);
        }
    });
    TempoHandler tempoHandler(base.musicDeviceHolder.musicDevices);
    TapTempoHandler tapTempoHandler(tempoHandler);
    util::ThreadedLoop tempoThread([&tempoHandler](){
        tempoHandler.nextTimeSlot();
        std::this_thread::sleep_for(std::chrono::microseconds(10));
    });   
    tempoHandler.start();
    TransportControl transportControl(base.musicDeviceHolder.transportCmdDrains);

    // ************************************************************************************************************
    // ******************** PUSH2::QT *****************************************************************************
    // ************************************************************************************************************

    QGuiApplication app(argc, argv);

    UnixSignalWatcher sigwatch;
    sigwatch.watchForSignal(SIGINT);
    sigwatch.watchForSignal(SIGTERM);
    QObject::connect(&sigwatch, SIGNAL(unixSignal(int)), &app, SLOT(quit()));

    qmlRegisterType<ValueCircle>("ValueCircle",1,0,"ValueCircle");
    qmlRegisterType<push2::qt::MappingCurveDrawing>("MappingCurveDrawing",1,0,"MappingCurveDrawing");
    qRegisterMetaType<push2::qt::MappingCurve>("MappingCurve");  

    app.setApplicationName("Push2");
    const auto displayId = fp::Push2Topology::Display::Id::eDisplay;
    //SelectedSoundDevice               selectedSoundDevice;
    QuickViewOnFpRenderMedium         fboQuickView(push2Device.getRenderMedium(fp::Widget(displayId)));

    fboQuickView.fboQuickView.rootContext()->setContextProperty("instruments", &instrumentsBridge);   

    push2::qt::Push2Device push2DeviceQtAdapter(push2Device, &app);
    push2DeviceQtAdapter.setObjectName("push2DeviceQtAdapter");
    fboQuickView.fboQuickView.rootContext()->setContextProperty("push2Device", &push2DeviceQtAdapter);
    qmlRegisterType<push2::qt::Push2DeviceOverlay>("push2.enums", 1, 0, "Push2DeviceOverlay");

    midi::Router                      midiRouter(base.musicDeviceHolder);
    midi::Dumper                      midiDumper(base.musicDeviceHolder);
    ParameterMapper                   parameterMapper(base.musicDeviceHolder);
    push2::qt::MidiChannelMappingBridge midiChannelMappingBridge(push2Device, midiRouter);
    push2::qt::MidiDumperModel midiDumperModel(midiDumper);

    utils::SettingsFnCollection settingsFnCollection;
    settingsFnCollection.addLoaderFn(std::bind(&TransportControl::load, &transportControl, "", "Settings.json", "transport_control"));
    settingsFnCollection.addSaverFn(std::bind(&TransportControl::save, &transportControl, "", "Settings.json", "transport_control"));
    settingsFnCollection.addLoaderFn(std::bind(&midi::Router::load, &midiRouter, "", "Settings.json", "midi_routing"));
    settingsFnCollection.addSaverFn(std::bind(&midi::Router::save, &midiRouter, "", "Settings.json", "midi_routing"));
    settingsFnCollection.addLoaderFn(std::bind(&ParameterMapper::load, &parameterMapper, "", "Settings.json", "parameter_mapping"));
    settingsFnCollection.addSaverFn(std::bind(&ParameterMapper::save, &parameterMapper, "", "Settings.json", "parameter_mapping"));
    settingsFnCollection.invokeLoaders();

    push2::qt::MidiOpenedInputPortsModel  openedInputPortsModel(base.musicDeviceHolder);
    push2::qt::MidiOpenedOutputPortsModel openedOutputPortsModel(base.musicDeviceHolder, midiRouter);
    fboQuickView.fboQuickView.rootContext()->setContextProperty("midiOpenedInputPortsModel", &openedInputPortsModel);
    fboQuickView.fboQuickView.rootContext()->setContextProperty("midiOpenedOutputPortsModel", &openedOutputPortsModel);
    fboQuickView.fboQuickView.rootContext()->setContextProperty("midiChannelMappingBridge", &midiChannelMappingBridge);
    fboQuickView.fboQuickView.rootContext()->setContextProperty("midiDumperModel", &midiDumperModel);

    push2::qt::SoundDeviceAndVoiceSelection soundDeviceAndVoiceSelection(base.musicDeviceHolder);
    fboQuickView.fboQuickView.rootContext()->setContextProperty("soundDeviceAndVoiceSelection", &soundDeviceAndVoiceSelection);

    push2::qt::ControllerDeviceAndPresetSelection controllerDeviceAndPresetSelection(base.musicDeviceHolder);
    fboQuickView.fboQuickView.rootContext()->setContextProperty("controllerDeviceAndPresetSelection", &controllerDeviceAndPresetSelection);   

    push2::qt::SoundDeviceParameter soundDeviceParameter(base.musicDeviceHolder, parameterMapper);
    fboQuickView.fboQuickView.rootContext()->setContextProperty("soundDeviceParameter", &soundDeviceParameter);   

    push2::qt::ControllerDeviceParameter controllerDeviceParameter(base.musicDeviceHolder, parameterMapper);
    fboQuickView.fboQuickView.rootContext()->setContextProperty("controllerDeviceParameter", &controllerDeviceParameter);   

    push2::qt::MusicScalesModel         musicScalesModel(push2Pads);
    push2::qt::MusicScalesBaseNoteModel musicScalesBaseNoteModel;
    push2::qt::PadsBridge               push2PadsBridge(push2Pads);
    push2::qt::TempoHandlerBridge       tempoHandlerBridge(tempoHandler);
    push2::qt::TapTempoHandlerBridge    tapTempoHandlerBridge(tapTempoHandler);
    push2::qt::TransportControlBridge   transportControlBridge(base.musicDeviceHolder.transportCmdDrains, transportControl);
    push2::qt::SettingsSaverBridge      settingsSaverBridge(settingsFnCollection);
    fboQuickView.fboQuickView.rootContext()->setContextProperty("musicScalesModel", &musicScalesModel);
    fboQuickView.fboQuickView.rootContext()->setContextProperty("musicScalesBaseNoteModel", &musicScalesBaseNoteModel);
    fboQuickView.fboQuickView.rootContext()->setContextProperty("push2Pads", &push2PadsBridge);
    fboQuickView.fboQuickView.rootContext()->setContextProperty("tempoHandler", &tempoHandlerBridge);
    fboQuickView.fboQuickView.rootContext()->setContextProperty("tapTempoHandler", &tapTempoHandlerBridge);
    fboQuickView.fboQuickView.rootContext()->setContextProperty("transportControl", &transportControlBridge);
    fboQuickView.fboQuickView.rootContext()->setContextProperty("settingsSaver", &settingsSaverBridge);

    qmlRegisterUncreatableMetaObject(push2::qt::FpWidget::staticMetaObject, "push2.enums", 1, 0, "FpWidget", "Not creatable as it is an enum type");
    qmlRegisterUncreatableMetaObject(push2::qt::ButtonPressState::staticMetaObject, "push2.enums", 1, 0, "ButtonPressState", "Not creatable as it is an enum type");
    qmlRegisterUncreatableMetaObject(push2::qt::Button3dPressState::staticMetaObject, "push2.enums", 1, 0, "Button3dPressState", "Not creatable as it is an enum type");
    qmlRegisterUncreatableMetaObject(push2::qt::EncoderTouchState::staticMetaObject, "push2.enums", 1, 0, "EncoderTouchState", "Not creatable as it is an enum type");
    qmlRegisterUncreatableMetaObject(push2::qt::TouchSurfaceTouchState::staticMetaObject, "push2.enums", 1, 0, "TouchSurfaceTouchState", "Not creatable as it is an enum type");
    qRegisterMetaType<push2::qt::FpWidget::Idx>("FpWidget::Idx");
    qRegisterMetaType<push2::qt::ButtonPressState::Value>("ButtonPressState::Value");
    qRegisterMetaType<push2::qt::Button3dPressState::Value>("Button3dPressState::Value");
    qRegisterMetaType<push2::qt::EncoderTouchState::Value>("EncoderTouchState::Value");
    qRegisterMetaType<push2::qt::TouchSurfaceTouchState::Value>("TouchSurfaceTouchState::Value");

    qmlRegisterUncreatableMetaObject(push2::qt::Led::staticMetaObject, "push2.enums", 1, 0, "Led", "Not creatable as it is an enum type");
    qmlRegisterUncreatableMetaObject(push2::qt::Button3d::staticMetaObject, "push2.enums", 1, 0, "Button3d", "Not creatable as it is an enum type");
    qmlRegisterUncreatableMetaObject(push2::qt::Button::staticMetaObject, "push2.enums", 1, 0, "Button", "Not creatable as it is an enum type");
    qmlRegisterUncreatableMetaObject(push2::qt::Encoder::staticMetaObject, "push2.enums", 1, 0, "Encoder", "Not creatable as it is an enum type");
    qmlRegisterUncreatableMetaObject(push2::qt::TouchSurface::staticMetaObject, "push2.enums", 1, 0, "TouchSurface", "Not creatable as it is an enum type");
    qRegisterMetaType<push2::qt::Button::Id>("Button::Id");
    qRegisterMetaType<push2::qt::Button3d::Id>("Button3d::Id");
    qRegisterMetaType<push2::qt::Encoder::Id>("Encoder::Id");
    qRegisterMetaType<push2::qt::Led::Id>("Led::Id");
    qRegisterMetaType<push2::qt::TouchSurface::Id>("TouchSurface::Id");

    qmlRegisterUncreatableMetaObject(push2::qt::MusicNote::staticMetaObject, "push2.enums", 1, 0, "MusicNote", "Not creatable as it is an enum type");
    qmlRegisterUncreatableMetaObject(push2::qt::MusicScales::staticMetaObject, "push2.enums", 1, 0, "MusicScales", "Not creatable as it is an enum type");
    qRegisterMetaType<push2::qt::MusicNote::Value>("MusicNote::Value");
    qRegisterMetaType<push2::qt::MusicScales::Value>("MusicScales::Value");

    qRegisterMetaType<QAbstractListModel*>("QAbstractListModel*");

    push2::QmlSceneLoader  qmlSceneLoader(fboQuickView);

    if(base.musicDeviceHolder.soundDevices.size() > 0)
    {
        soundDeviceAndVoiceSelection.setCurrentDeviceIndex(0);
    }
    if(base.musicDeviceHolder.controllerDevices.size() > 0)
    {
        controllerDeviceAndPresetSelection.setCurrentDeviceIndex(0);
    }

#ifndef __SIMULATION__MODE__
    QTimer displayRefreshTimer;
    QObject::connect(&displayRefreshTimer, &QTimer::timeout, [&push2Device](){
        push2Device.getRenderMedium(fp::Widget(displayId))->flushFrameBuffer();
    });
    displayRefreshTimer.start(900);
#endif
#ifdef __SIMULATION__MODE__
    QTimer timer;
    QObject::connect(&timer, &QTimer::timeout, [&push2Device](){
        push2Device.updateInputs();
    });
    timer.start(10);
#endif
    push2Pads.updateLeds();
    const auto appRet = app.exec();
    settingsFnCollection.invokeSavers();
    return appRet;
}