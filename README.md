# Earthly based build environment for the Sgitheach Harwell dekatron clock

## Overview
The [Sgitheach Harwell dekatron](http://www.sgitheach.org.uk/harwell.html) clock is an open source dekatron clock.   
The source code is distributed via [Dropbox](https://www.dropbox.com/sh/vbxwpxwj4dzgiuu/AAAJOA2jXWSSztL8QWtX_jF5a/Firmware?dl=0&subfolder_nav_tracking=1) under a Creative Commons ShareAlike 4.0 International license.   

### Build system
This build frame work makes use of [Earthly](https://earthly.dev/get-earthly) to build all firmware in a portable and reproducible manor.
Earthly depends on [Docker](https://docs.docker.com/get-docker).   

By using this build framework, it enables us to build the firmware on:   
- Linux   
- MacOS   
- Windows   

Once you have Docker and Eathly installed for your Platform you can move on with building the firmware.   

### Building firmware
**NOTE**
Ensure Earthy and Docker are working before proceeding.   

#### Obtain upstream source
Download the zip files from [Upstream](https://www.dropbox.com/sh/vbxwpxwj4dzgiuu/AAAJOA2jXWSSztL8QWtX_jF5a/Firmware?dl=0&subfolder_nav_tracking=1) and place them into the directory where this README.md is located.  

#### Building the Firmware
**NOTE**
You will need to adjust the name of the Earthly binary to match your Operating system.  
In the case of Windows this would be `earthly.exe`.   

##### What version to build?   
At the time of writing this guide(20220117) the latest release from Upstream is `1.2.1`.   
This version is set as a default inside the [Earthfile](https://github.com/brendanhoran/sgitheach-harwell-earthly-build/blob/ceffbd7469a058b4031b0eccf344f0558db9efd0/Earthfile#L14).   
This can be overridden at build time by inserting the following option into the command line examples below.   
Example to set version `1.3.0` and build al firmware:   
```
earthy --build-arg UPSTREAM_VERSION=1.3.0 +all-firmware
```



##### To build all the required firmware.    
Issue the following command:   
```
earthly +all-firmware
```

##### To build a specific firmware component.   
Components currently supported are:   
- bootloader   
- dekatron   
- sam   

Example to build just the SAM firmware:   
``` 
earthly +sam
```

Once any of of the builds have finished you can find the firmware/build artifacts under the `artifacts/` directory that will be automatically created for you.   



### Flashing the firmware

For MacOS and Windows uses, please refer back to the respective Sgitheach PDF manuals and use the GUI's as instructed.   
For Linux users, This build framework will be extended later to support CLI tooling to flash the firmware files.   