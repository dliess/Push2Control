# Push2Control
Midi Controller sw for Abletons Push2

## Dependencies:
  + qtbase5-dev
  + qtdeclarative5-dev
  + qml-module-qtquick
  + libasound2-dev
  + libgtest-dev
  + libusb-1.0-0-dev

## To make this work as non-root:
```
add in /etc/udev/rules.d/60-push2.rules:
SUBSYSTEM=="usb", ATTR{idProduct}=="1967", ATTR{idVendor}=="2982", MODE="0666"
```

## Build Example:
```
  cd <repo-dir>
  mkdir build
  cd build
  cmake ..
  cmake-gui . (set midi to ALSA)
  make
```
## run
```
  cd <repo-dir>
  build/Push2Control
```