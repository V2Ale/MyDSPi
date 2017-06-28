<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01//EN" "http://www.w3.org/TR/html4/strict.dtd">
<html>
<head>
  <meta content="text/html; charset=ISO-8859-1"
 http-equiv="content-type">
</head>
<body>
<h1>Amplifier and Power Supply Board</h1>
<h3>Overview</h3>
<br>
Amplifier and Power Supply board is the second PCB of this project that
can be stack with the DSP board. It's designed to host 2 set of
amplifiers and DAC, which can handle up to for 4 channels of
amplification. In addition, the board is used for power supply the
entire stack. <br>
<br style="color: red;">
<span style="color: red;"></span><img
 alt="Block diagram Amplifier"
 src="https://github.com/V2Ale/MyDSPi/blob/master/Ressources/BlockDiagramMyAmpi.PNG"><br>
<br>
<h3>DAC</h3>
<br>
Digital to Analog conveters used for this board are PCM5102a (and
similar such as PCM5100). The
benefit of using PCM5102 DAC is that it does not requires the I2S
high frequency system clock (SCK) to work making routing between boards
much easier. <br>
Also, this component provides additionnal controls like demphasis, FIR
or IIR filter of output, format (I2S or Left justified) and soft mute.
Those controls are driven by a port expander with I2C interface that
allows end user to use this amplifier board for other designs. More
details on those controls can be found in the component datasheet. <br>
Last, this component
is readily cheap and available on the market (PCM5100 being pin to pin
compatible).<br>
<br>
<br>
<h3>Amplifier</h3>
&nbsp;<br>
The Texas Instrument TPA3118 amplifiers are stereo class D amplifiers
that can drive 2x30W into 8 Ohms load (with a 24V supply). It can
operate with a wide single operating voltage from 4.5V to 26V keeping
the power supply design simple. <br>
As audio input, the amplifier expect single ended audio which are
provided by DAC. All external controls (like gain) of the amplifier are
set constant except the amplifier mute which can be controlled thanks
to the port expander.<br>
Last, the amplifier integrates bad operating conditions (like
overtemperature, output short circuit) which are sent back to the
Faultz pins. Those pins are connected to the port expander and can be
monitored as well.<br>
<br>
<h3>Port Expander</h3>
<br>
As explain in previous parts, the port expander is in charge of
monitoring amplifier state and set DAC and amplifiers controls. The
part used for this task is very common MCP23008 from Microchip.<br>
<br>
<h3>Power Supply </h3>
<br>
The board do not have any power supply transformer and switch mode
power supply block design is outside the scope of this project. As a
result, a maximum of 26V voltage is expected at the DC jack terminal
connector. This voltage is directly used to feed the amplifiers and is
also converted down to a 5V supply voltage for all other boards
(including Raspberry Pi supply). &nbsp; <br>
<br>
<h3>Connectors </h3>
<br>
The&nbsp; 40 pin connector is used to connected to the DSP board
and get DSP output channels,&nbsp; I2C and amplifier mute command.
It also provides the supply rail voltage for the entire project.<br>
<br>
<span style="color: red;">[IMAGE DU SCHEMA DU CONNETEUR]</span><br>
<br>
<table style="text-align: left; width: 378px; height: 1216px;"
 border="1" cellpadding="2" cellspacing="2">
  <tbody>
    <tr>
      <td>Pin</td>
      <td>Name &nbsp; &nbsp; </td>
      <td>Description &nbsp; &nbsp; &nbsp; &nbsp;
&nbsp;</td>
    </tr>
    <tr>
      <td>1</td>
      <td>DC In</td>
      <td>Input Power supply is duplicated on this pin</td>
    </tr>
    <tr>
      <td>2</td>
      <td>DC In</td>
      <td>Input Power supply is duplicated on this pin</td>
    </tr>
    <tr>
      <td>3</td>
      <td>-</td>
      <td>Not used</td>
    </tr>
    <tr>
      <td>4</td>
      <td>SCL</td>
      <td>I2C Clock</td>
    </tr>
    <tr>
      <td>5</td>
      <td>-</td>
      <td>Not used</td>
    </tr>
    <tr>
      <td>6</td>
      <td>SDA</td>
      <td>I2C Data</td>
    </tr>
    <tr>
      <td>7</td>
      <td>Amp_mute</td>
      <td>Pin connected to Raspberry pi and amplifier mute pin.</td>
    </tr>
    <tr>
      <td>8</td>
      <td>-</td>
      <td>Not used</td>
    </tr>
    <tr>
      <td>9</td>
      <td>-</td>
      <td>Not used</td>
    </tr>
    <tr>
      <td>10</td>
      <td>-</td>
      <td>Not used</td>
    </tr>
    <tr>
      <td>11</td>
      <td>GND</td>
      <td>Ground</td>
    </tr>
    <tr>
      <td>12</td>
      <td>-</td>
      <td>Not used</td>
    </tr>
    <tr>
      <td>13</td>
      <td>-</td>
      <td>Not used</td>
    </tr>
    <tr>
      <td>14</td>
      <td>-</td>
      <td>Not used</td>
    </tr>
    <tr>
      <td>15</td>
      <td>-</td>
      <td>Not used</td>
    </tr>
    <tr>
      <td>16</td>
      <td>-</td>
      <td>Not used</td>
    </tr>
    <tr>
      <td>17</td>
      <td>GPO3</td>
      <td>DSP MPIO3 Output pin</td>
    </tr>
    <tr>
      <td>18</td>
      <td>GPO2</td>
      <td>DSP&nbsp;MPIO2 Output pin</td>
    </tr>
    <tr>
      <td>19</td>
      <td>GPO1</td>
      <td>DSP&nbsp;MPIO1 Output pin</td>
    </tr>
    <tr>
      <td>20</td>
      <td>DSPIN_SADATA0</td>
      <td>DSP I2S input pin 1</td>
    </tr>
    <tr>
      <td>21</td>
      <td>DSP_SDATA0</td>
      <td>DSP I2S output pin 1</td>
    </tr>
    <tr>
      <td>22</td>
      <td>DSP_BCK</td>
      <td>DSP I2S Bitclock output</td>
    </tr>
    <tr>
      <td>23</td>
      <td>DSP_SDATA1</td>
      <td>DSP I2S output pin 2</td>
    </tr>
    <tr>
      <td>24</td>
      <td>DSP_LRCK</td>
      <td>DSP I2S Bitclock output</td>
    </tr>
    <tr>
      <td>25</td>
      <td>ANALOG_IN_R</td>
      <td>DSP Analog input pin 1</td>
    </tr>
    <tr>
      <td>26</td>
      <td>ANALOG_IN_L</td>
      <td>DSP Analog input pin 2</td>
    </tr>
    <tr>
      <td>27</td>
      <td>AUDIO_OUT0</td>
      <td>DSP Analog output 1</td>
    </tr>
    <tr>
      <td>28</td>
      <td>GPO9</td>
      <td>DSP MPIO 9</td>
    </tr>
    <tr>
      <td>29</td>
      <td>AUDIO_OUT1</td>
      <td>DSP Analog output 2</td>
    </tr>
    <tr>
      <td>30</td>
      <td>GPO8</td>
      <td>DSP MPIO 8</td>
    </tr>
    <tr>
      <td>31</td>
      <td>AUDIO_OUT2</td>
      <td>DSP Analog output 3</td>
    </tr>
    <tr>
      <td>32</td>
      <td>AUDIO_OUT3</td>
      <td>DSP Analog output 4</td>
    </tr>
    <tr>
      <td>33</td>
      <td>-</td>
      <td>Not used</td>
    </tr>
    <tr>
      <td>34</td>
      <td>-</td>
      <td>Not used</td>
    </tr>
    <tr>
      <td>35</td>
      <td>GND</td>
      <td>Ground</td>
    </tr>
    <tr>
      <td>36</td>
      <td>GND</td>
      <td>Ground</td>
    </tr>
    <tr>
      <td>37</td>
      <td>5V</td>
      <td>5V power supply</td>
    </tr>
    <tr>
      <td>38</td>
      <td>GND</td>
      <td>Ground</td>
    </tr>
    <tr>
      <td>39</td>
      <td>5V</td>
      <td>5V power supply</td>
    </tr>
    <tr>
      <td>40</td>
      <td>5V</td>
      <td>5V power supply</td>
    </tr>
  </tbody>
</table>
<br>
<br>
</body>
</html>
