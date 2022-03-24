# This Earthfile aims to build the firmware for the Harwell Dekatron clock
# http://www.sgitheach.org.uk/harwell.html
# Project source is distrubted via Dropbox at:
# https://www.dropbox.com/sh/vbxwpxwj4dzgiuu/AAAJOA2jXWSSztL8QWtX_jF5a/Firmware?dl=0&subfolder_nav_tracking=1
# Upstream source code is licended under: Creative Commons ShareAlike 4.0 International license.


# Build on Debian Linux
FROM debian:stable-slim

# Set the upstream version to build
# defaults to 1.2.1, as of 20220117 this is the latest release
# Can be overridden on the CLI
ARG UPSTREAM_VERSION = 1.2.1


# Update apt and install build time dependences
RUN apt update && \
    DEBIAN_FRONTEND=noninteractive apt install -y \
    unzip git g++ make avr-libc gcc-avr gcc-arm-none-eabi


dekatron:
    ## Build the harwell dekatron firmware

    # Copy in the shared files directory
    COPY --dir shared shared

    RUN mkdir dekatron
    COPY "Dekatron.$UPSTREAM_VERSION Final.zip" dekatron

    # Run echo 0 via || on failed exit code from unzip to ensure RUN command suceeds
    # Cause is malformed zip from Dropbox
    # REF:
    # https://unix.stackexchange.com/questions/634315/unzip-thinks-my-zip-file-is-a-zip-bomb
    RUN cd dekatron && unzip "Dekatron.$UPSTREAM_VERSION Final.zip" || echo 0
  
    # Copy in the project specfic Makefile
    COPY Makefile-dekatron dekatron/Makefile

    RUN cd dekatron && sed -i 's#d:/sgitheach/common/harwell/slave.h#../../shared/slave.h#' src/dekatron.h
    RUN cd dekatron && sed -i 's#d:/sgitheach/common/harwell/slave.h#../../shared/slave.h#' src/stuff/twi.h
    RUN cd dekatron && sed -i 's#d:/sgitheach/common/harwell/slave.h#../../shared/slave.h#' src/stuff/twi_procs.h
    RUN cd dekatron && sed -i 's#d:/sgitheach/common/harwell/slave.h#../../shared/slave.h#' src/main.c

    RUN echo "Building Dekatron firmware version: $UPSTREAM_VERSION"
    RUN cd dekatron && make all

    SAVE ARTIFACT "dekatron/Dekatron.$UPSTREAM_VERSION.cproj" AS LOCAL artifacts/dekatron/
    SAVE ARTIFACT "dekatron/Dekatron.$UPSTREAM_VERSION.eep" AS LOCAL artifacts/dekatron/
    SAVE ARTIFACT "dekatron/Dekatron.$UPSTREAM_VERSION.elf" AS LOCAL artifacts/dekatron/
    # Rename the hex file into Dos 8.3 comptabile filenames for flashing from the bootloader
    SAVE ARTIFACT "dekatron/Dekatron.$UPSTREAM_VERSION.hex" AS LOCAL artifacts/dekatron/dekatron.hex
    SAVE ARTIFACT "dekatron/Dekatron.$UPSTREAM_VERSION.lss" AS LOCAL artifacts/dekatron/
    SAVE ARTIFACT "dekatron/Dekatron.$UPSTREAM_VERSION.map" AS LOCAL artifacts/dekatron/
    SAVE ARTIFACT "dekatron/Dekatron.$UPSTREAM_VERSION.srec" AS LOCAL artifacts/dekatron/

bootloader:
    ## Build the harwell bootloader

    # Copy in the shared files directory
    COPY --dir shared shared

    RUN mkdir bootloader
    COPY "Bootloader $UPSTREAM_VERSION Final.zip" bootloader

    # Run echo 0 via || on failed exit code from unzip to ensure RUN command suceeds
    # Cause is malformed zip from Dropbox
    # REF:
    # https://unix.stackexchange.com/questions/634315/unzip-thinks-my-zip-file-is-a-zip-bomb
    RUN cd bootloader && unzip "Bootloader $UPSTREAM_VERSION Final.zip" || echo 0

    # Copy in the project specfic Makefile
    COPY Makefile-bootloader bootloader/Makefile

    # Update paths for the shared header
    RUN cd bootloader && sed -i 's#d:/sgitheach/common/harwell/slave.h#../../shared/slave.h#' src/stuff/twi.h
    RUN cd bootloader && sed -i 's#d:/sgitheach/common/harwell/slave.h#../../shared/slave.h#' src/stuff/twi_procs.h
    RUN cd bootloader && sed -i 's#d:/sgitheach/common/harwell/slave.h#../../shared/slave.h#' src/main.c

    RUN echo "Building bootloader firmware version: $UPSTREAM_VERSION"
    RUN cd bootloader && make all

    SAVE ARTIFACT "bootloader/Bootloader $UPSTREAM_VERSION.cproj" AS LOCAL artifacts/bootloader/
    SAVE ARTIFACT "bootloader/Bootloader $UPSTREAM_VERSION.eep" AS LOCAL artifacts/bootloader/
    SAVE ARTIFACT "bootloader/Bootloader $UPSTREAM_VERSION.elf" AS LOCAL artifacts/bootloader/
    SAVE ARTIFACT "bootloader/Bootloader $UPSTREAM_VERSION.hex" AS LOCAL artifacts/bootloader/
    SAVE ARTIFACT "bootloader/Bootloader $UPSTREAM_VERSION.lss" AS LOCAL artifacts/bootloader/
    SAVE ARTIFACT "bootloader/Bootloader $UPSTREAM_VERSION.map" AS LOCAL artifacts/bootloader/
    SAVE ARTIFACT "bootloader/Bootloader $UPSTREAM_VERSION.srec" AS LOCAL artifacts/bootloader/

sam:
    ## Build just the SAM firmware

    # Copy in the shared files directory
    COPY --dir shared shared

    RUN mkdir sam
    COPY "Sgitheach.$UPSTREAM_VERSION Final.zip" sam

    # Run echo 0 via || on failed exit code from unzip to ensure RUN command suceeds
    # Cause is malformed zip from Dropbox
    # REF:
    # https://unix.stackexchange.com/questions/634315/unzip-thinks-my-zip-file-is-a-zip-bomb
    RUN cd sam && unzip "Sgitheach.$UPSTREAM_VERSION Final.zip" || echo 0

    # Copy in the project specfic Makefile
    COPY Makefile-sam sam/Makefile

    # Update paths for the shared header
    RUN cd sam && sed -i 's#d:\\sgitheach\\common\\harwell\\slave.h#../../shared/slave.h#' src/display/dekatron.h
    RUN cd sam && sed -i 's#d:\\sgitheach\\common\\harwell\\slave.h#../../shared/slave.h#' src/main.c

    # Ensure all paths have backslash's to confirm with correct Linux path standards
    RUN cd sam && sed -i 's#config\\conf_dekatron.h#config/conf_dekatron.h#' src/display/dekatron.h
    RUN cd sam && sed -i 's#stuff\\console.h#stuff/console.h#' src/sound/chimes.c

    RUN echo "Building SAM firmware version: $UPSTREAM_VERSION"
    RUN cd sam && make all

    SAVE ARTIFACT "sam/Sgitheach.$UPSTREAM_VERSION.bin" AS LOCAL artifacts/sam/
    SAVE ARTIFACT "sam/Sgitheach.$UPSTREAM_VERSION.cproj" AS LOCAL artifacts/sam/
    SAVE ARTIFACT "sam/Sgitheach.$UPSTREAM_VERSION.eep" AS LOCAL artifacts/sam/
    SAVE ARTIFACT "sam/Sgitheach.$UPSTREAM_VERSION.elf" AS LOCAL artifacts/sam/
    SAVE ARTIFACT "sam/Sgitheach.$UPSTREAM_VERSION.hex" AS LOCAL artifacts/sam/
    SAVE ARTIFACT "sam/Sgitheach.$UPSTREAM_VERSION.lss" AS LOCAL artifacts/sam/
    SAVE ARTIFACT "sam/Sgitheach.$UPSTREAM_VERSION.map" AS LOCAL artifacts/sam/
    SAVE ARTIFACT "sam/Sgitheach.$UPSTREAM_VERSION.srec" AS LOCAL artifacts/sam/

bossa:
    ## Build the BOSSA CLI applictions for Linux

    GIT CLONE https://github.com/shumatech/BOSSA.git bossa
    RUN cd bossa && make bossac

    SAVE ARTIFACT "bossa/bin/bossac" AS LOCAL artifacts/utils/

all-firmware:
    ## Build all the firmware for harwell

    FROM +bootloader
    FROM +dekatron
    FROM +sam
