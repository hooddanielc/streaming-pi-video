# Stream Video on Raspberry Pi

> Why do I do this to myself?

## Build and Run

First install dependencies

- [ib](https://github.com/JasonL9000/ib) - c++ build tool
- python - used by ib
- gcc - common compiler
- opencv - used to grab video from capture device

Once `ib` is added to your path

```
cd streaming-pi-video
ib spill-the-void/client
ib spill-the-void/server

# Run the client on the device with camera
../out/debug/spill-the-void/client 127.0.0.1 1337

# Run the server to _receive_ udp packets and display image
../out/debug/spill-the-void/server 1337
```

## Running Tests

```
cd streaming-pi-video
ib test/socket-test
../out/debug/test/socket-test
```

This software is known to work on raspberry pi and ubuntu desktops.
