<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01//EN" "http://www.w3.org/TR/html4/strict.dtd">
<html>
<head>
  <meta content="text/html; charset=ISO-8859-1"
 http-equiv="content-type">
  <h1>MyDSPi</h1>
</head>
<body>

<br>
<div style="margin-left: 40px;">MyDSPi is an easy to
implement solution for audio processing and amplification with a
Raspberry Pi (2 and 3) audio output. <br>
It's intended to be a cost effective solution to make
active&nbsp;speakers.<br>
Electronic circuit is based on Analog Device ADAU1701 DSP and Texas
Instrument TPA3118 amplifier.<br>

</div>
<br style="font-weight: bold;">
<h3>Hardware description</h3>
<h4 style="margin-left: 40px; width: 1239px;">Overview : </h4>
<div style="margin-left: 40px;"><br>
<div style="margin-left: 40px;">MyDSPi is designed to be
compact and mounted over a Raspberry Pi 2 board. Hardware is split over
2 printed circuit boards stack one above the other.<br><br>
<img
 alt="Block diagram Amplifier"
 src="https://github.com/V2Ale/MyDSPi/blob/master/Ressources/ProjectStackOverviewDrawing.PNG"><br>
<br>
The first board is dedicated to DSP processing and Digital to Analog
signal convertion from Raspberry Pi audio output.&nbsp;Audio must
be output from Raspberry
Pi through I2S output pins.<br>
DSP is controlled by Raspberry Pi using I2C&nbsp; interface and
requires no
advanced DSP programming to have signal processed and can be easily
configured by editing a configuration file.<br>
The DSP work at fixed sampling frequency of 48000 Hz. <br>
Output channels are transmitted to the amplifier board through I2S
protocol.<br>
<br>
The second board host Digital to Analog converter that feed the
amplifier with analog signal for amplification. Power supply input
circuit is also located on this PCB, which can accept any supply
voltage from 12 to 24 volts to feed all system power supply (including
the Raspberry Pi).<br>
<br>
For more details about each board, please refer to :<br>
<span style="color: red;"><a
 href="https://github.com/V2Ale/MyDSPi/tree/master/Electronic/DSP%20Board/2.0">DSP Board page</a></span><br>
<span style="color: red;"><a
 href="https://github.com/V2Ale/MyDSPi/tree/master/Electronic/Amplification%20Board">Amplifier board page</a>)</span> 
amplifier pcb page<br>
</div>
</div>
<br>
<h3><big>Software description</big></h3>
<div style="margin-left: 40px;">Once install is done the
MyDSPi software run at start-up to load default DSP parameters and then
load the parameters read from the configuration file.<br>
<br>
</div>
<h4>DSP Capabilities description</h4>
<br>
<div style="margin-left: 80px;">The default parameters of
the DSP are right now a 2 input / 2
output configuration with a default crossover and no EQ values loaded.
Parameter customization is done throught the MyDSPi.conf file and can
configure : <br>
<br>
<div style="margin-left: 40px;">- Input channels volume
controls<br>
- Input channels mute controls<br>
- 2 channels crossover<br>
- output channels Delay (up to 412 samples) <br>
- Ouput channel mute control<br>
- Ouput channel phase control<br>
- Output channel volume control<br>
</div>
</div>
<br>
<h3>Installation</h3>
Installation of &nbsp;MyDSPi is tested and developped for Linux
Raspbian Jessie distribution.<br>
<br>
The following step describes only the installation major steps. <br>
- Setup I2C (<a
 href="https://learn.adafruit.com/adafruits-raspberry-pi-lesson-4-gpio-setup/configuring-i2c">https://learn.adafruit.com/adafruits-raspberry-pi-lesson-4-gpio-setup/configuring-i2c</a>)<br>
- Setup I2S soundcard (following &nbsp;<a
 href="https://www.hifiberry.com/build/documentation/hifiberry-software-configuration/">https://www.hifiberry.com/build/documentation/hifiberry-software-configuration/</a>)<br>
- Run installation script (see<span style="color: red;">
https://github.com/V2Ale/MyDSPi/tree/master/Software/Install</span>)<br>
<h3>Configuration</h3>
To edit the configuration file, use the command :<br>
<pre>&nbsp;sudo nano /etc/default/MyDSPi.conf</pre>
For more details about configuration file rules, please refer to <span
 style="color: red;">https://github.com/V2Ale/MyDSPi/tree/master/Software/Install/config</span><br>
<br>
<h3>Roadmap</h3>
Support 2 input / 2.1 output<br>
&nbsp;<br>
<h3>Disclaimer</h3>
I'm not a profesinnal developper, which might explain some ugly
workaround in my code. My programming skills and time are very limited.
Consequently, I can't afford to do a lot of support. I really whish
advanced and enthusiast developpers could join and improve the
project.&nbsp;
</body>
</html>
