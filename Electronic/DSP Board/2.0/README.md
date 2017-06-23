DSP PCB

Power Supply

The PCB receives its power supply from the amplifier PCB which has the input DC jack socket and DC converters.

The 5V supply at input  is shared with the Raspberry Pi. From this supply, a 3.3V regulator on the board feed all components of this board.

Input channels
The DSP can process analog or digital (I2S) input, but only analog circuit is made available. Despite the presence of analog input circuit components routed in the 40 pin connecor, the project do not rely on it.
To use the board with the Raspberry Pi, the 3 wires of I2S are converted by a DAC (PCM5102a) and provided to DSP by the analog input.
The benefit of using PCM5102 DAC is that it does not requires the I2S system clock (SCK) to work. It's especially useful because the raspberry pi does not provide the SCK out of the box and it avoids useless painful struggle with the kernel.
Moreover, this component is readily cheap and available on the market (PCM5100 being pin to pin compatible). Extra functionality of this DAC such as filter type or soft mute are not used and not left to end user.

Output channels
 
The DSP can output analog or digital (I2S) signals. Both of them are available on this board but the amplifiers board is expecting I2S signal before amplification stage. If you whish to use the I2S output with other amplifier board, please mind that it does not output the SCK as well. This choice has been made because the DAC do not need it to operate normally and it avoid special care in the routing for this high speed clock.

I2S and clocks

Input and output I2S signals are working at fixed LRCK of 48kHz. The DSP is driven by a 24.576MHz clock. I2S LRCK and BCK at input and output of the DSP are connected together as required in the datasheet of the component.
It's not possible to run the DSP at other sampling frequency or with different outside clock without changing the routing of the board.

Connectors 
