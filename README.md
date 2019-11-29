# Push2Control
Midi Controller sw for Abletons Push2

## Dependencies:
  + qtbase5-dev
  + qtdeclarative5-dev
  + qml-module-qtquick2
  + libasound2-dev
  + libusb-1.0-0-dev

## To make this work as non-root:
```
add in /etc/udev/rules.d/60-push2.rules:
SUBSYSTEM=="usb", ATTR{idProduct}=="1967", ATTR{idVendor}=="2982", MODE="0666"
```

## Build Example:
```
  cd <repo-dir>
  (at the very first time) git submodule update --init
  mkdir build
  cd build
  cmake ..
  make
```
## run
```
  cd <repo-dir>
  build/Push2Control
```
