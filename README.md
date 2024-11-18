# rkaiq_test

This code demonstrates using Rockchip hardware ISP to manipulate a live video stream from a camera connected to the CSI port while 3A algorithms are running. It will start the 3A server, wait a few seconds for the 3A auto adjustment to finish, and then make some clearly visible changes to the video stream.

## Cloning this repo

Make sure to add --recurse-submodules option to git clone command in order to properly clone the submodule camera_engine_rkaiq.

## 3A Server

The generated executable includes a AIQ 3A server. If the default system 3A server is running, it must be stopped or disabled before running the rkaiq_test executable.

```sh
sudo systemctl stop rkaiq_3A.service
```

## Local testing

Clone this repo directly on a rk356x-based SBC, compile and run the code locally.

```sh
cd rkaiq_test/camera_engine_rkaiq
./build.sh
cd ..
mkdir build
cd build
cmake ..
make
sudo ./rkaiq_test
```

## Remote testing

Clone this repo on a separate development machine, then use rsync to send the code to a remote rk356x-based SBC on the same LAN, and the rk356x will compile and run the code.

1. Create file pw.txt containing the ssh password of your dev board
2. Edit variables in run-remote.sh to correct values
3. Run this script to test on remote dev board:

```sh
./run-remote.sh
```
