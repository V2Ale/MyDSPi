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


Config section

The input section is used to configure the input type of the DSP. It has one parameter :

**audioinput**
It can take the values :

- *digital*
to set DSP input I2S signal
- *analog* 
to set DSP input as analog
- *tone* 
This  force the input to be a twin sine tone signal (500 Hz and 5000 Hz)

**Input sections**
There is one input section per input channel with 2 parameters in each section.

- *mute*
            Mute when it's value is 1, must be 0 otherwise
- *volume*
            Control the input volume of the channel. It can take any value between -80 and 0

**Output sections**
There is one output section per output channel with 4 parameters in each section.

- *mute*
            Mute when it's value is 1, must be 0 otherwise
- *volume*
            Control the input volume of the channel. It can take any value between -80 and 0 and 0.0
- *delay*
           Control the delay in samples of this channel. It can take any value between 1 and 412
- *phase*
            Control the output channel polarity. It takes the value 1 to reverse polarity, 0 otherwise

**EQ sections**
There are 14 EQ filters avaialble which can be configured independently. 

 - *type*
           Defines the type of EQ. It can take the value
*	HSF for high shelf filter
*	LSF for low shelf filter
*	HPF for high pass filter (1st order Bessel)
*	LPF for Low pass filter (1st order Bessel)
*	peak for peak filter (parametric)
   
 - *F*
Set the corner frequency of the filter. It can range between 20 to 20000
 - *B*
Control boost factor of  peak, LSF and HSF filters. It can take any value between -80.0 to 20.0
 - *Q*
Control Q-factor of  peak type EQ. It can take any value between 0.1 to 10.0
 - *B*
Control the S parameter of LSF and HSF filters. It can take any value between 0.0 and 3.0

**Crossover section**
       There is one crossover for both output channels made of 2nd order butterworth low-pass and high-pass filter.   
       
- *FLow*
	Defines the Low pass filter corner frequency. It can range between 20 and 20000 and must be lower than the FHigh value

- *FHigh*
	Defines the High pass filter corner frequency. It can range between 20 and 20000 and must be greater than the FLow value

