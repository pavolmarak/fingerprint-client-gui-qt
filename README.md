# Qt GUI client
This repository houses source code for an experimental biometric client app communicating with authentication server over TCP, all written in Qt. The client app allows to choose IP and port of the server and connect to it. The app demonstrates the process of connecting, disconnecting and notifying about any state changes in TCP connection.

From biometric point of view, the app supports scanning fingerprints and transferring them to the authentication server which then performs fingerprint recognition based on the open-source library with codename DBOX, developed by László Kádek and Pavol Marák in 2018. 

The aforementioned DBOX library is composed of three main components (standalone shared libraries):
 * image preprocessing ([here](https://github.com/pavolmarak/Preprocessing))
 * feature extraction ([here](https://github.com/pavolmarak/Extraction))
 * feature matching ([here](https://github.com/pavolmarak/Matcher))
 
These repositories were forked from [László Kádek's profile](https://github.com/stupel) on GitHub and are planned to undergo further intensive development. 

Here is the preview of the app's GUI running under Manjaro Linux with KDE Plasma desktop environment.

![client-app-gui](client_app.png)

*What do you need to build this software?*

* Qt 5.11
* OpenCV 3.4.2 or higher
* Suprema BioMini fingerprint scanner and the corresponding BioMini SDK (only in case you want to use the scanner as image data source) 
