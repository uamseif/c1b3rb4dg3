# C1B3RB4DG3 - Kahoot game

Kahoot style game for the c1b3rb4dg3. This game depends on the
[KahootClone API](#), developed by Bhavuk Sikka and Samuel de Lucas as
an assignment for the course _Proyecto de Sistemas Informáticos_.

> TODO: Add link to the API repository, and expand this README

## Important notes
To make the project work, you need to create a file called `wifi_config.h`
in the root folder of the project, with the following content:

```c++
/*
    wifi_creds.h

    File containing your WiFi credentials
    This file is ignored in the .gitignore file
*/

#ifndef wifi_creds_h
#define wifi_creds_h

const char* SSID = "<Your SSID here>";
const char* PASSWD = "<Your PASSWORD here>";

#endif
```