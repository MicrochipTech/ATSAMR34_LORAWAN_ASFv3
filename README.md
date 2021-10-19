# ATSAMR34_LORAWAN_ASFv3
<img src="resources/media/IoT-Made-Easy-Logo.png" width=100>

> "Wireless Made Easy!" - Microchip LoRaWAN Stack Opened for SAM R34 IC and WLR089U0 Module running on Microchip Studio IDE

Devices: | **ATSAMR34 | WLR089U0**

<p align="left">
<a href="https://www.microchip.com" target="_blank">
<img src="resources/media/Microchip_logo.png" width=320></a>
<a href="https://www.microchip.com/lora" target="_blank">
<img src="resources/media/applications.png" width=240></a>
<img src="resources/media/microchip_studio.png" width=120>
</p>

## ⚠ Disclaimer

<p><span style="color:red"><b>
THE SOFTWARE ARE PROVIDED "AS IS" AND GIVE A PATH FOR SELF-SUPPORT AND SELF-MAINTENANCE.</br></br>
This repository contains unsupported example code intended to help accelerate client product development. It is not validated for production nor audited for security best-practices.</br></br>
Note that while this repository is unsupported, Microchip welcome community contributions, and all pull-requests will be considered for inclusion into the repository.
</span></p></b>

> Interact with peers in the community at [LoRa Forum](https://www.microchip.com/forums/f512.aspx).


## Resources

- [SAMR34/35 and WLR089U Landing page](https://www.microchip.com/design-centers/wireless-connectivity/low-power-wide-area-networks/lora-technology/sam-r34-r35)
- [SAMR34 Device page](https://www.microchip.com/wwwproducts/en/ATSAMR34J18)
- [SAMR34 Xplained Pro Evaluation Kit](https://www.microchip.com/DevelopmentTools/ProductDetails/dm320111)
- [WLR089U0 Module page](https://www.microchip.com/wwwproducts/en/WLR089U0)
- [WLR089U0 Module Xplained Pro Evaluation Kit](https://www.microchip.com/EV23M25A)
- [Microchip Studio 7.0 IDE](https://www.microchip.com/mplab/microchip-studio)

[Clone/Download](https://docs.github.com/en/free-pro-team@latest/github/creating-cloning-and-archiving-repositories/cloning-a-repository) the current repo to get the package.

## A la carte

1. [Introduction](#step1)
1. [Hardware Platforms](#step2)
1. [Software Requirements](#step3)
1. [LoRaWAN Certification Test Tool](#step4)
1. [Package Overview](#step5)
1. [Sample Applications](#step6)
   1. [LoRaWAN Mote Application](#step6a)
   2. [LoRaWAN Mote Application + ECC608](#step6b)
   3. [RN Command Parser](#step6c)
<!--   4. [RN Command Parser + ECC608](#step6d)-->

## Introduction<a name="step1"></a>

The Microchip LoRaWAN™ Stack (MLS) provides a solution for the LoRaWAN end-device that is used for Internet of Things (IoT) applications.

LoRa® is a wireless modulation technique designed to allow low-power end-devices to communicate over long range and at low data rates.

LoRaWAN is a wireless networking protocol which operates over LoRa communication layer and acts as Medium Access Control (MAC) layer.

LoRaWAN specification and its development is overseen by LoRa Alliance™. The specification is meant for secure communication of end-devices and ensures inter-operability within the LoRa network.

Microchip LoRaWAN Stack is Opened for SAM R34 IC and WLR089U0 Module where MAC and Radio layers are both provided as source code for self-support and self-maintenance.

MLS v1_0_P_6 release is pre-tested against LoRaWAN 1.0.4 Class A using official LoRa Alliance Test Tool (LCTT).

## Hardware Platforms<a name="step2"></a>

* Purchase the [SAM R34 Xplained Pro Evaluation Kit](https://www.microchip.com/en-us/development-tool/DM320111)</br></br>
<img src="resources/media/ATSAMR34Xpro.png" width=320>

* Purchase the [WLR089U0 Xplained Pro Evaluation Kit](https://www.microchip.com/en-us/development-tool/EV23M25A)</br></br>
<img src="resources/media/WLR089U0Xpro.png" width=320>

* Purchase a LoRa Gateway

* If using a Join Server with security solution for LoRaWAN, a pre-provisioned ECC608 secure element is required. Purchase the [ATECC608B-TNGLORA](https://www.microchip.com/en-us/product/ATECC608B-TNGLORA) and the [CryptoAuthentication Socket Kit](https://www.microchip.com/en-us/product/ATECC608B-TNGLORA)


## Software Requirements<a name="step3"></a>

* [Microchip Studio 7.0.2542 IDE](https://www.microchip.com/mplab/microchip-studio)
* [Advanced Software Framework ASFv3.50](https://www.microchip.com/en-us/development-tools-tools-and-software/libraries-code-examples-and-more/advanced-software-framework-for-sam-devices)

> Note: The following Atmel Studio tools needs to be uninstalled before installation of Microchip Studio 7.0.2542 version. Otherwise, you may face error while loading the projects.</br>- Atmel Driver Files</br>- Atmel LibUSB0 Driver(x64)</br>- Atmel Segger USB Drivers(501e)</br>- Atmel WinUSB

* A LoRa Network Server access


## LoRaWAN Certification Test Tool<a name="step4"></a>

The LoRa Alliance® has developed a LoRaWAN® Certification Test Tool (LCTT) for all LoRa Alliance® device manufacturers to pre-test their devices before sending them to the Authorised Test Houses (ATHs) for formal LoRaWAN® Certification testing. 

The LoRaWAN® certification tests for end node functionality, in other words it tests that node’s LoRaWAN protocol stack and application are compliant with the LoRaWAN specification.

Checkout the [LoRa Alliance](https://lora-alliance.org/lorawan-certification/) for more details.

<b><p>
The present software has been PRE-TESTED against the LoRaWAN® L2 Specification 1.0.4 (Class A only) for the following regions:
- EU863-870</br>
- US902-928</br>
</p></b>
And using the following setup:

- LCTT version: LCTT v3.1.0_D1 | GUI v1.10.0
- Gateway(s) used for EU campaign (OTAA, ABP): Picocell GW and Semtech GW (Picocell GW for all tests except SF12 steps)
- Gateway(s) used for NA campaign (OTAA, ABP): Picocell GW
- SAMR34 Xplained Pro Rev 4
- WLR089U0 Xplained Pro Rev A/01
- All testing have been done in Application mode using the **LoRaWAN Mote Application** since the LoRaWAN 1.0.4 test specification elaborated and standardised the command communication between the test Network Server and the test End Device.

> For reference, checkout the folder `MLS_SDK_1_0_P_6_Release/LCTT_TestReports` and get access to the test reports.


## Package Overview<a name="step5"></a>

The Microchip LoRaWAN Stack contains:
* A Microchip Studio 7.0 project, which provides a reference application
* The LoRaWAN stack components in a source code
* Drivers, software timer, PDS, PMM and radio drivers for the LoRaWAN stack
* The facility to support dynamic regional band switching within the supported bands

The package also contains the **Migration Guide** which provide all the information need to migrate the MLS LoRaWAN applications implemented on MLS_SDK_1_0_P_5 available in SAM platforms (SAMR34, WLR) to MLS_SDK_1_0_P_6. The improvements and bugfixes are listed as well as the MLS API changes.

MLS provides APIs for following software modules:
* LoRaWAN MAC Layer (MAC)
* LoRaWAN Radio Layer (TAL)
* Persistent Data Server (PDS)
* Power Manager Module (PMM)
* Hardware Abstraction Layer (HAL)

The APIs and the stack attributes are described in the [SAM R34-R35 Microchip LoRaWAN Stack Software API Reference Manual](https://www.microchip.com/en-us/product/ATSAMR34J18#document-table).

### LoRaWAN Stack Directory Structure

The following table provides the directory structure of the LoRaWAN stack code base (`src/ASF/thirdparty/wireless/lorawan`).

| Directory | Description |
| --------- | ----------- |
| `/hal` | Contains the implementation for the radio hardware interface, timers, etc | `/inc` | Contains commonly included file(s) |
| `/mac` | Contains the headers and sources of the LoRaWAN MAC layer specification independent of regional parameters |
| `/pmm` | Contains the Power Management Module (PMM) |
| `/regparams` | Contains the implementation of the MAC layer functionality specific to the regional bands |
| `/sal` | Contains the Security Abstraction Layer (SAL) |
| `/services` | Contains modules scuh as software timer, PDS and AES |
| `/sys` | Contains system modules such as task manager, power management and initialization |
| `/tal` | Contains transceiver related headers and sources, drivers for supported transceivers |


## Sample Applications<a name="step6"></a>

### LoRaWAN Mote Application<a name="step6a"></a>

| Application | Target | Location |
| ----------- | ------ | -------- |
| EndDevice_Demo | SAMR34 Xplained Pro | `MLS_SDK_1_0_P_6_Release/Enddevice_Demo/enddevice_demo_src_multiband_samr34_xpro` |
| EndDevice_Demo | WLR089U0 Xplained Pro | `MLS_SDK_1_0_P_6_Release/Enddevice_Demo/enddevice_demo_src_multiband_wlr089_xpro` |

The EndDevice_Demo application provides configuration for the application in `src/config/conf_app.h` where it is possible to configure the following parameters:
* Sub band number
* Activation methods (OTAA or ABP)
* Type of transmission (confirmed or unconfirmed message)
* Application port number (Fport 1-255)
* Device Class (A or C)
* Type of join Nonce (incremental or random)
* Join parameters (OTAA or ABP)
* Multicast parameters
* Sleep duration in milliseconds
* Periodic timer duration in milliseconds (used for periodic transmission option)

> Note #1: In SAM R34 project, the macro `EDBG_EUI_READ=1` is set by default in the symbols definition. This configuration can be use with SAM R34 Xplained Pro evaluation kit in order to use the EDBG EUI as DEVEUI key for Over-The-Air Join process. With a custom board which embed the SAM R34 IC, disable the macro `EDBG_EUI_READ=0` and fill your own DEVEUI key in the definition of `DEMO_DEVICE_EUI` located in the file `src/config/conf_app.h`.

> Note #2: In WLR089U0 project, the macro `MODULE_EUI_READ=1` is set by default in the symbols definition. This configuration can be use either with WLR089U0 Xplained Pro evaluation kit or with custom board.


Checkout the [SAM R34 MLS Getting Started Guide](https://www.microchip.com/en-us/product/ATSAMR34J18#document-table) to get details on the application configuration and how to flash and run the project.

### LoRaWAN Mote Application + ECC608<a name="step6b"></a>

| Application | Target | Location |
| ----------- | ------ | -------- |
| EndDevice_Demo_ecc608 | SAMR34 Xplained Pro | `MLS_SDK_1_0_P_6_Release/Enddevice_Demo/enddevice_demo_src_multiband_ecc608_samr34_xpro` |
| EndDevice_Demo_ecc608 | WLR089U0 Xplained Pro | `MLS_SDK_1_0_P_6_Release/Enddevice_Demo/enddevice_demo_src_multiband_ecc608_wlr089_xpro` |

Use the present software and refer to the following resources to get more details :

- [Develop with the SAM R34 LoRa SiP and WLR089U0 Module with Microchip LoRaWAN stack on TTI join server](https://github.com/MicrochipTech/atsamr34_ecc608a_tti)
- [Secure Authentication with SAMR34 & ATECC608 and The Things Industries’s Join Server](https://github.com/MicrochipTech/secure_lorawan_with_tti)


### RN Command Parser<a name="step6c"></a>

Checkout the instructions and get the software from the <a href="https://github.com/MicrochipTech/atsamr34_lorawan_rn_parser" target="_blank">RN Command Parser</a> repository.

<!--
### RN Command Parser + ECC608<a name="step6d"></a>

Checkout the instructions and get the software from the <a href="https://github.com/MicrochipTech/atsamr34_lorawan_rn_parser" target="_blank">RN Command Parser</a> repository.

-->