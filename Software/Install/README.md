# Installation
First install Git on your Raspberry Pi using the command :

$ sudo apt-get install git

Then you need to clone the repository
$ sudo git clone https://github.com/V2Ale/MyDSPi.git

Go into the /software/Install folder and run the installation script

$ cd <yourdir>/Software/Install
$ ./MyPDSi_install.sh


This will install the relevant executables, configuration file and setup the service

## Executables softwares
### MyDSPi

MyDSPi is the software that manages the DSP and amplifier based on the configuration file


### PE_Configurator

PE_Configurator is the software that manages the port expander of the amplifier board. It can be launch with the '-init' command for normal operation and with '-mute' command to mute the amplifier :
$ PE_Configurator -init
$ PE_Configurator -mute
