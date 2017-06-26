<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01//EN" "http://www.w3.org/TR/html4/strict.dtd">
<html>
<head>
  <meta content="text/html; charset=ISO-8859-1"
 http-equiv="content-type">
  <title>DSP Board</title>
</head>
<body>
DSP PCB<br>
<br>
Overview<br>
<br>
The DSP used for this project is Analog Device's ADAU1701. It has many
input and output pins, either analog and digital. In the project scope,
this board in 2.0 version is limited to the use of 2 input to 8 digital
+ 4 analog outputs.<br>
It has two 40 pins connectors. One to connect this board to the
raspberry pi, the other to connect the board to other stages (like
amplifier &amp; power supply). The DSP clock is supplied by a
24.576 Mhz Quartz and the programming of the DSP is done via external
microcontroller (Raspberry Pi) using I2C interface.<br>
<br>
Input channels<br>
The DSP can process analog or digital (I2S) input, but only analog
input can be used. Despite the presence of analog input circuit
components routed in the 40 pin connecor, the project do not rely on
it. <br>
To use the board with the Raspberry Pi, the 3 wires of I2S are
converted by a DAC (PCM5102a) and provided to DSP by the analog input. <br>
The benefit of using PCM5102 DAC is that it does not requires the I2S
system clock (SCK) to work. It's especially useful because the
raspberry pi does not provide the SCK out of the box and it avoids
useless painful struggle with the kernel. <br>
Moreover, this component is readily cheap and available on the market
(PCM5100 being pin to pin compatible). Extra functionality of this DAC
such as filter type or soft mute are not used and not left to end user.<br>
<br>
Output channels<br>
&nbsp;<br>
The DSP can output analog or digital (I2S) signals. Both of them are
available on this board but the amplifiers board is expecting I2S
signal before amplification stage. If you whish to use the I2S output
with other amplifier board, please mind that it does not output the SCK
as well. This choice has been made because the DAC do not need it to
operate normally and it avoid special care in the routing for this high
speed clock. <br>
<br>
I2S and clocks<br>
<br>
Input and output I2S signals are working at fixed LRCK of 48kHz. The
DSP is driven by a 24.576MHz clock. I2S LRCK and BCK at input and
output of the DSP are connected together as required in the datasheet
of the component.<br>
It's not possible to run the DSP at other sampling frequency or with
different outside clock without changing the routing of the board. <br>
<br>
Connectors <br>
<br>
The first 40 pin connector is fully compliant with the Raspberry Pi 2
and later connector layout. The other one, is used to provide input
&amp; output channels, power supply, I2C and amplifier mute to the
other stages.<br>
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
Power Supply<br>
<br>
The PCB receives its power supply from the amplifier PCB which has the
input DC jack socket and DC converters.<br>
The
5V supply at input&nbsp; is shared with the Raspberry Pi. From this
5V supply,
a 3.3V regulator on the board feed all components of this board.
</body>
</html>
