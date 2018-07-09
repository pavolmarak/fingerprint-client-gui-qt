# Qt GUI client
This repository houses source code for an experimental biometric client app communicating with authentication server over TCP, all written in Qt. The client app allows to choose IP and port of the server and connect to it. The app demonstrates the process of connecting, disconnecting and notifying about any state changes in TCP connection.

From biometric point of view, the app supports scanning fingerprints and transferring them to the authentication server which then performs fingerprint recognition based on the open-source library with codename DBOX, developed by László Kádek and Pavol Marák in 2018. This library is composed of three main components (standalone shared libraries):
 * preprocessing (https://github.com/stupel/Preprocessing)

Here is the screenshot of the app's GUI running under Manjaro Linux with KDE Plasma desktop environment.

![client-app-gui](client_app.png)
