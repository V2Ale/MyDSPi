<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01//EN" "http://www.w3.org/TR/html4/strict.dtd">
<html>
<head>
  <meta content="text/html; charset=ISO-8859-1"
 http-equiv="content-type">
  <title></title>
</head>
<body>
<h3>Configuration</h3>
<br>
<div style="margin-left: 40px;">MyDSPi configuration files
needs to respect a few rules to allow to correct parameter loading.<br>
All parametes are grouped into sections. Each sections can define a set
of parameters which have their respective values. Each set of
&nbsp;parameter and its value must be written on the same line.<br>
</div>
<br>
<h4>Rules</h4>
<div style="margin-left: 40px;">Writting a configuration
needs to respect the following rules<br>
<ul>
  <li>Comments are ignored from the configuration lines. A
comment is everything that is after&nbsp;<span
 style="font-weight: bold;">;</span> character</li>
  <li>A section is written in between brackets as
&nbsp;&nbsp; [SECTION]</li>
  <li>To be valid a parameter must be followed by a value as
parameter = value</li>
  <li>Section, parameters and values must respect letter case</li>
</ul>
The available sections are :<br>
<ul>
  <li>[config]</li>
  <li>[Input1]</li>
  <li>[Input2]</li>
  <li>[Output1 </li>
  <li>[Output2]</li>
  <li>[EQ1]</li>
  <li>[crossover]</li>
</ul>
</div>
<h4 style="margin-left: 40px;">config section</h4>
<div style="margin-left: 80px;">The input section is used
to configure the input type of the DSP. It has one parameter :<br>
<pre><pre>audioinput<br></pre></pre>
The values can be <br>
</div>
<div style="margin-left: 40px;">
<div style="margin-left: 40px;">
<pre><span style="font-weight: bold;">digital</span><br></pre>
To set DSP input I2S signal
</div>
<pre style="margin-left: 40px;"><span
 style="font-weight: bold;">analog</span><br></pre>
<div style="margin-left: 40px;">To set DSP input as analog
</div>
</div>
<div style="margin-left: 40px;">
<div style="margin-left: 40px;">
<pre><span style="font-weight: bold;">tone</span><br></pre>
This &nbsp;force the input to be a twin sine tone signal (500 Hz
and 5000 Hz)<br>
</div>
<div style="margin-left: 80px;"></div>
</div>
<div style="margin-left: 80px;"><br>
</div>
<h4 style="margin-left: 40px;">
Input sections</h4>
<h4 style="margin-left: 80px;"><span
 style="font-weight: normal;">There is one input section per
input channel with 2 parameters in each section.</span></h4>
<div style="margin-left: 40px;">
<pre>&nbsp;&nbsp;&nbsp; &nbsp;&nbsp;mute</pre>
&nbsp;&nbsp;&nbsp; &nbsp;&nbsp;&nbsp;
&nbsp;&nbsp;&nbsp; Mute when it's value is 1, must be 0
otherwise <br>
&nbsp;&nbsp;&nbsp; <br>
<pre>&nbsp;&nbsp;&nbsp; &nbsp;&nbsp;volume</pre>
&nbsp;&nbsp;&nbsp; &nbsp;&nbsp;&nbsp;
&nbsp;&nbsp;&nbsp; Control the input volume of the channel.
It can take any value between -80 and 0</div>
<br>
<h4 style="margin-left: 40px;">Output sections</h4>
<h4 style="margin-left: 80px;"><span
 style="font-weight: normal;">There is one output section per
output channel with 4 parameters in each section.</span></h4>
<div style="margin-left: 40px;">
<pre>&nbsp;&nbsp;&nbsp; &nbsp;&nbsp;mute</pre>
&nbsp;&nbsp;&nbsp; &nbsp;&nbsp;&nbsp;
&nbsp;&nbsp;&nbsp; Mute when it's value is 1, must be 0
otherwise <br>
&nbsp;&nbsp;&nbsp; <br>
<pre>&nbsp;&nbsp;&nbsp; &nbsp;&nbsp;volume</pre>
&nbsp;&nbsp;&nbsp; &nbsp;&nbsp;&nbsp;
&nbsp;&nbsp;&nbsp; Control the input volume of the channel.
It can take any value between -80.0 and 0.0<br>
<pre>&nbsp;&nbsp;&nbsp; &nbsp;&nbsp;delay</pre>
&nbsp;&nbsp;&nbsp; &nbsp;&nbsp;&nbsp;
&nbsp;&nbsp; Control the delay in samples of this channel. It
can take any value between 1 and 412<br>
&nbsp;&nbsp;&nbsp; <br>
<pre>&nbsp;&nbsp;&nbsp; &nbsp;&nbsp;phase</pre>
&nbsp;&nbsp;&nbsp; &nbsp;&nbsp;&nbsp;
&nbsp;&nbsp;&nbsp; Control the output channel polarity. It
takes the value 1 to reverse polarity, 0 otherwise</div>
<h4 style="margin-left: 40px;"></h4>
<h4 style="margin-left: 40px;"><br>
EQ sections</h4>
<h4 style="margin-left: 80px;"><span
 style="font-weight: normal;">There are 14 EQ filters
avaialble which can be configured independently.&nbsp;</span></h4>
<div style="margin-left: 40px;">
<pre>&nbsp;&nbsp;&nbsp; &nbsp;&nbsp;type</pre>
&nbsp;&nbsp;&nbsp; &nbsp;&nbsp;&nbsp;
&nbsp;&nbsp;&nbsp; Defines the type of EQ. It can take the
value<br>
<div style="margin-left: 80px;">&nbsp;<span
 style="font-style: italic;">HSF</span> for high shelf
filter <br>
<span style="font-style: italic;">LSF</span> for low
shelf filter <br>
<span style="font-style: italic;">HPF</span> for
high pass filter (1st order Bessel)<br>
<span style="font-style: italic;">LPF</span> for Low
pass filter (1st order Bessel)<br>
<span style="font-style: italic;">peak</span> for
peak filter (parametric)<br>
</div>
&nbsp;&nbsp;&nbsp; <br>
<pre>&nbsp;&nbsp;&nbsp; &nbsp;&nbsp;F</pre>
&nbsp;&nbsp;&nbsp; &nbsp;&nbsp;&nbsp;
&nbsp;&nbsp;&nbsp; Set the corner frequency of the filter.
It can range between 20 to 20000<br>
<pre>&nbsp;     B</pre>
&nbsp;&nbsp;&nbsp; &nbsp;&nbsp;&nbsp;
&nbsp;&nbsp; Control boost factor of &nbsp;peak, LSF and
HSF filters. It can take any value between -80.0 to 20.0 <br>
<pre>&nbsp;&nbsp;&nbsp; &nbsp;&nbsp;Q</pre>
&nbsp;&nbsp;&nbsp; &nbsp;&nbsp;&nbsp;
&nbsp;&nbsp; Control Q-factor of &nbsp;peak type EQ. It can
take any value between 0.1 to 10.0 <br>
&nbsp;&nbsp;&nbsp; <br>
<pre>&nbsp;&nbsp;&nbsp; &nbsp;&nbsp;S</pre>
&nbsp;&nbsp;&nbsp; &nbsp;&nbsp;&nbsp;
&nbsp;&nbsp;&nbsp; Control the S parameter of LSF and HSF
filters. It can take any value between 0.0 and 3.0<br>
<pre>      </pre>
</div>
<h4 style="margin-left: 40px;"><br>
Crossover</h4>
&nbsp;&nbsp;&nbsp; &nbsp;&nbsp;&nbsp;
&nbsp;&nbsp;&nbsp; There is one crossover for both output
channels made of 2nd order butterworth low-pass and high-pass filter. <br>
&nbsp;&nbsp;&nbsp; &nbsp;&nbsp;&nbsp;
&nbsp;&nbsp;&nbsp; <br>
<pre style="margin-left: 40px;">FLow</pre>
<div style="margin-left: 40px;">Defines the Low pass
filter corner frequency. It can range between 20 and 20000 and must be
lower than the FHigh value<br>
</div>
<pre style="margin-left: 40px;">FHigh</pre>
<div style="margin-left: 40px;">Defines the High pass
filter corner frequency. It can range between 20 and 20000 and must be
greater than the FLow value<br>
</div>
<br>
<h4>Block diagram<br>
</h4>
<br>
</body>
</html>
