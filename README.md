# QuickFace 
A real-time 3D human facial reconstruction application (software) with a Windows-side software and a Linux-side service.

v 1.0.0 - 2017.1.6


## Introduction

QuickFace is a software capable of 
* capturing the user’s facial expressions with a normal 2D camera,
* reconstructing the corresponding 3D models 
* and generating live 3D facial animations in real time.

![Missing image](https://raw.githubusercontent.com/WMBao/QuickFace/master/image/Composition.jpg)

The Server-side service takes on model reconstruction, which requires strong computing power.

## File Structure

|Path|Discription|
|:---|:---|
|./QuickFace-Client/   | |
|./QuickFace-Client/QuickFace/   |*source files for the Windows-side software*|
|./QuickFace-Client/QuickFace Installer.exe   |*The installer for the Windows-side software*|
|./QuickFace-Server/   |*source files for the Server-side service*|
|./QuickFace Installer.exe|*Windows-side software installer*|

## Installation
### Windows-side
Double click ``./QuickFace Installer.exe``.

### Server-side
For detailed information for configuration, please see [README.md](https://github.com/WMBao/QuickFace/blob/master/QuickFace-Server/README.md) within the ``./QuickFace-Server/`` folder.


## How to Use?

* Start the server-end by typing these instructions in the terminal:
```Shell
cd {ROOT_DIR}/QuickFace-Server/install/bin
./QuickFace
```
* Double click the QuickFace.exe in the root of the installation path.
* Click the “*Start Camera*” button to open the camera and establish the connection with the server.
* Click the “*Change Mode*” button to change the background of 3D model.
* Click the “*Save Object*” button to save the 3D model to your device.
* Click the “*Quit*” button to quit.

Snapshots for illustration:

![Missing image](https://raw.githubusercontent.com/WMBao/QuickFace/master/image/snapshot1.jpg)

![Missing image](https://raw.githubusercontent.com/WMBao/QuickFace/master/image/snapshot2.jpg)

An example reconstructed 3D facial model is shown as follows (in a third-party 3D model viewer):

![Missing image](https://raw.githubusercontent.com/WMBao/QuickFace/master/image/3D.jpg)
