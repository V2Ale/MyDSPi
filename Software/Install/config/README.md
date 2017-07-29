Configuration
----------



MyDSPi configuration files needs to respect a few rules to allow to correct parameter loading.
All parametes are grouped into sections. Each sections can define a set of parameters which have their respective values. 

**Block diagram**

The audio block are linked together as follow to build the audio chain :

	audioinput -> input mute -> input volume -> EQ -> crossover -> delay -> phase -> mute -> volume

**Rules**
Writting a configuration needs to respect the following rules

 1.  Comments are ignored from the configuration lines.
 2.  A comment is everything that is after ';'character
 3.  A section is written in between brackets as    [SECTION]
 4.  To be valid a parameter must be followed by a value as parameter = value
 5.  Section, parameters and values must respect letter case
 6.  Each set of  parameter and its value must be written on the same line

**Sections**
The available sections are :

    [config]
    [Input1]
    [Input2]
    [Output1
    [Output2]
    [EQ1]
    [crossover]


----------


**Config section**

The input section is used to configure the input type of the DSP. It has one parameter : *audioinput*

It can take the following values

| Parameter | Value | Description  |
| ------- | ---- | --- |
| audioinput | *digital* |  set DSP input to I2S signal|
| audioinput | *analog* | set DSP input to analog|
| audioinput | *tone* | This  force the input to be a twin sine tone signal (500 Hz and 5000 Hz)|

 

**Input sections**
There is one input section per input channel with 2 parameters in each section.

| Parameter     | Value | Description  |
| ------- | ---- | --- |
| *mute* | 0 or 1|  muted when it's value is 1, must be 0 otherwise|
| *volume* | -80.0 to 0.0|  control the input volume of the channel|

**Output sections**
There is one output section per output channel with 4 parameters in each section.
      

| Parameter     | Value | Description  |
| ------- | ---- | --- |
| *mute* | 0 or 1|  muted when it's value is 1, must be 0 otherwise|
| *volume* | -80.0 to 0.0|  control the input volume of the channel|
| *delay* | 1 to 412|  control the delay in samples of this channel |
| *phase*| 0 or 1|  control the output channel polarity. 1 to reverse polarity, 0 otherwise|

**EQ sections**
There are 14 EQ filters avaialble which can be configured independently. 

HSF for high shelf filter
LSF for low shelf filter
HPF for high pass filter (1st order Bessel)
LPF for Low pass filter (1st order Bessel)
peak for peak filter (parametric)

| Parameter     | Value | Description  |
| ------- | ---- | --- |
| *type* | HSF or LSF or HPF or LPF or peak| Defines the type of EQ |
| *F* | 20 to 20000| Set the corner frequency of the filter|
| *B* | -80.0 to 20.0|  Control boost factor of  peak, LSF and HSF filters.  |
| *Q* | 0.1 to 10.0|  Control Q-factor of  peak type EQ  |
| *B*| 0.0 to 3.0|  Control the S parameter of LSF and HSF filters.

**Crossover section**
       There is one crossover for both output channels made of 2nd order butterworth low-pass and high-pass filter.   
       
| Parameter     | Value | Description  |
| ------- | ---- | --- |
| *FLow* | between 20 and 20000 |  Defines the Low pass filter corner frequency. It must be lower than the FHigh value    |
| *FHigh*    | between 20 and 20000   |  Defines the High pass filter corner frequency. It must be greater than the FLow value


----------
**Config file Example**

[config]
audioinput=digital									

[Input1]										
mute=1												
volume=-25										

[Input2]										
mute=0												
volume=-25											

[Output2]											
phase=-1
mute=1												
delay=1												
volume=0											

[Output1]											
phase=1
mute=1												
delay=1												
volume=-6											

[EQ1]												
F=125												
B=-2.8												
Q=1.41												
Type=peak											

[EQ2]
F=5000												
B=-2												
Q=4													
Type=peak	

[EQ3]
F=6000
B=-2
Q=1.41
Type=peak	

[EQ4]
F=2200
B=-2
S=2
Type=HSF

[crossover]											
FLow = 2800										
FHigh = 3000
