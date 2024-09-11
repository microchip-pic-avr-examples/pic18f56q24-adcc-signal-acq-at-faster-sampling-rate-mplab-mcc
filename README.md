<!-- Please do not change this logo with link -->

<a target="_blank" href="https://www.microchip.com/" id="top-of-page">
   <picture>
      <source media="(prefers-color-scheme: light)" srcset="images/mchp_logo_light.png" width="350">
      <source media="(prefers-color-scheme: dark)" srcset="images/mchp_logo_dark.png" width="350">
      <img alt="Microchip Technologies Inc." src="https://www.microchip.com/content/experience-fragments/mchp/en_us/site/header/master/_jcr_content/root/responsivegrid/header/logo.coreimg.100.300.png/1605828081463/microchip.png">
   </picture>
</a>

# Signal Acquisition at Higher Sampling Frequency Using the ADCC of the PIC18-Q24 Microcontroller Family

## Introduction

The PIC18-Q24 family of microcontrollers are equipped with a rich set of Core Independent Peripherals (CIP), such as Analog-to-Digital Converter with Computation (ADCC), Direct Memory Access (DMA), Serial Peripheral Interface (SPI), Universal Asynchronous Receiver and Transmitter (UART) and so on. The integrated ADCC peripheral offers a 10-bit resolution that can handle conversion processes at speeds reaching up to 300 ksps.

The main objective of this code example is to demonstrate the higher data sampling capabilities of the 10-bit ADCC peripheral within the PIC18-Q24 device family, thereby effectively reproducing the sampled input signal. Additionally, it highlights the CIP capabilities of 8-bit PIC microcontrollers (MCU) by effectively utilizing the ADCC, DMA, SPI and UART peripherals within the code example. Specifically, the DMA peripheral transfers the ADCC conversion results to the SPI peripheral data transmit buffer register without CPU intervention, whereas the SPI is used to output the received ADCC conversion results to the Data Visualizer tool in real time and at a faster rate.

This code example is developed using the PIC18F56Q24 MCU. However, it is relevant to all the 8-bit PIC® microcontrollers families that are supported by a 10-bit ADCC module capable of sampling input signals up to 300 ksps.

## Related Documentation

* [PIC18F56Q24 Product Page](https://www.microchip.com/en-us/product/PIC18F56q24)
* [PIC18-Q24 Product Family of Microcontrollers](https://www.microchip.com/en-us/products/microcontrollers-and-microprocessors/8-bit-mcus/pic-mcus/pic18-q24?utm_source=GitHub&utm_medium=TextLink&utm_campaign=MCU8_PIC18-Q24&utm_content=pic18f56q24-adcc-signal-acq-at-faster-sampling-rate-mplb-mcc-github&utm_bu=MCU08)
* [PIC18F56Q24 Code Examples on MPLAB® Discover](https://mplab-discover.microchip.com/v2?dsl=PIC18F56Q24)
* [PIC18-Q24 Family Code Examples on GitHub](https://github.com/orgs/microchip-pic-avr-examples/repositories?q=Q24&type=all&language=&sort=)

## Description

<p align="center">
  <img width=auto height=auto src="images/blockdiagram.png">
  <br> <b>Figure 1.</b> Block Diagram<br>
</p>

This section covers the implementation details of the code example. The PIC18F56Q24 Curiosity Nano and Power Debugger boards are used as a development platform. 

Input Signal Generation: A pulse signal is generated using 8-bit buffered Digital-to-Analog Converter (DAC), DMA, Timer2 (TMR2) and Fixed Voltage Reference (FVR) peripherals of the PIC18F56Q24 microcontroller. The signal is generated by continuously adjusting the output of a DAC to create a pulse signal made of a series of discrete steps. The DMA module has been used for seamless and direct data transfers between different memory regions without intervention from the CPU. The values of the DAC input data register are loaded from Look-up Table (LUT) stored in RAM using DMA. The frequency of the pulse signal is determined by the frequency of updating the DAC DATA register and the total number of points/samples in one cycle of the signal. In this example, the number of data samples per cycle of the pulse signal is set as 100 and is stored in an LUT in RAM memory. This data is transferred to the DAC data register using DMA which is hardware triggered by a Timer2 overflow interrupt occurring every 10 µs.

Therefore, frequency of signal, f = 1 / (Total no of samples per cycle * TMR2 period) = 1/ (100 * 10 µS) = ~1000 Hz.

**Note:** If the number of DAC samples in the pulse signal is kept constant, then the signal frequency can be changed by altering the TMR2 period. 

The DAC output signal is fed to the ADCC input channel through pin RA2.

In this code example, the firmware is developed to sample the input signal at two different sampling frequen ies: 50 ksps and 300 ksps. By default, the ADCC module samples the input signal at 300 ksps sampling frequency, and the user needs to press the on-board mechanical switch (SW0) to change the ADCC sampling frequency to 50 ksps and vice versa. The sampled ADCC conversion data are transmitted to the Data Visualizer in real time through the Data Gateway Interface (DGI) of the power debugger board and reproduce the input signal in the Time Plot window. Additionally, the on-board user LED (LED0) blinks at a specific rate depending on the sampling frequency selection. The instructions for the user to change the sampling frequency via a switch press event, along with the selected sampling frequency value, are transmitted to the Data Visualizer tool using the on-board debugger module and displayed on the terminal window.


## Software Used

This application uses MPLAB<sup>®</sup> X IDE compiler and graphical code generator to provide a seamless and efficient user experience.

The following tools are used for this demo application:

* [MPLAB® X IDE](https://www.microchip.com/en-us/tools-resources/develop/mplab-x-ide?utm_source=GitHub&utm_medium=TextLink&utm_campaign=MCU8_PIC18-Q24&utm_content=pic18f56q24-adcc-signal-acq-at-faster-sampling-rate-mplb-mcc-github&utm_bu=MCU08) 6.20 or newer
* [XC8 compiler](https://www.microchip.com/en-us/tools-resources/develop/mplab-xc-compilers?utm_source=GitHub&utm_medium=TextLink&utm_campaign=MCU8_PIC18-Q24&utm_content=pic18f56q24-adcc-signal-acq-at-faster-sampling-rate-mplb-mcc-github&utm_bu=MCU08) 2.50 or newer
* [MPLAB® Code Configurator(MCC)](https://www.microchip.com/en-us/tools-resources/configure/mplab-code-configurator?utm_source=GitHub&utm_medium=TextLink&utm_campaign=MCU8_PIC18-Q24&utm_content=pic18f56q24-adcc-signal-acq-at-faster-sampling-rate-mplb-mcc-github&utm_bu=MCU08) 5.7.1 or newer
* [Stand-alone MPLAB® Data Visualizer 1.3.1677](https://www.microchip.com/en-us/tools-resources/debug/mplab-data-visualizer?utm_source=GitHub&utm_medium=TextLink&utm_campaign=MCU8_PIC18-Q24&utm_content=pic18f56q24-adcc-signal-acq-at-faster-sampling-rate-mplb-mcc-github&utm_bu=MCU08) or newer
* [Microchip PIC18F-Q Series Device Support](https://packs.download.microchip.com) 1.25.433 or newer
newer
* ADCC MCC Melody Driver 1.0.6
* SPI MCC Melody Driver 1.2.0
* DMA Driver 1.1.2
* UART Driver 1.10.0
* DAC Driver 4.0.9
* TMR0 Driver 4.0.13
* TMR2 Driver 4.0.18


## Hardware Used

* [PIC18F56Q24 Curiosity Nano board](https://www.microchip.com/en-us/development-tool/EV01E86A?utm_source=GitHub&utm_medium=TextLink&utm_campaign=MCU8_PIC18-Q24&utm_content=pic18f56q24-adcc-signal-acq-at-faster-sampling-rate-mplb-mcc-github&utm_bu=MCU08)
* [Curiosity Nano base for Click boards™](https://www.microchip.com/developmenttools/ProductDetails/AC164162)
* [Power Debugger board](https://www.microchip.com/en-us/development-tool/ATPOWERDEBUGGER?utm_source=GitHub&utm_medium=TextLink&utm_campaign=MCU8_PIC18-Q24&utm_content=pic18f56q24-adcc-signal-acq-at-faster-sampling-rate-mplb-mcc-github&utm_bu=MCU08)

## Hardware Setup

<p align="center">
  <img width=auto height=auto src="images/hardwareSetup.png">
  <br>Figure 2. Hardware Setup<br>
</p>

The DGI SPI mode of the Power Debugger board is used for transmitting data from the PIC18F56Q24 microcontroller to the Data Visualizer tool. The table below provides the required connections between the PIC18F56Q24 Curiosity Nano and Power Debugger boards.  Also, both boards are powered up by a 5V supply.


| DGI Header of Power Debugger  | PIC18F56Q24 Curiosity Nano Board  | 
|-------------------------------|-----------------------------------|                                                                                        
| CLK                           | RC3 (SCK1)                        | 
| MISO(<--)                     | RB3 (MOSI)                        |   
| CS                            | RA5                               |   
| REF                           | VTG                               |  
| GND                           | GND                               |


## Firmware Implementation

* The application firmware is developed on the PIC18F56Q24 microcontroller, generated using MPLAB X IDE and MCC. The implemented firmware is performed on the PIC18F56Q24 Curiosity Nano development board. 

* The microcontroller peripherals, TIMERS, ADCC, DAC, FVR, DMA, UART and SPI, are used to implement the functionalities of the ADCC sampling speed demonstration 

* The TMR0 peripheral is used to blink the LED at a specific rate, depending on the sampling frequency selection. At 300 ksps sampling frequency, the LED blinks with a 50 ms delay and, at 50 ksps sampling frequency, the LED blinks with a 200 ms delay.

* The TMR2 peripheral is configured to generate an overflow interrupt for every 10 µs, used to trigger the DMA1 module 

* The TMR2 overflow interrupt acts as trigger source to the DMA1 peripheral instance. The DMA1 module is configured to transmit one byte of data from the LUT, stored in the RAM, to the DAC data register for each TMR2 overflow interrupt. 

* The DAC1 peripheral is configured with FVR as its positive reference and ground as its negative reference and is used to generate an output pulse signal. The output from the DAC1 peripheral is fed as an input to the ADCC module.

* To acquire the input signal, the ADCC module is configured in Basic mode with Fosc as the peripheral clock and FVR as the voltage reference. The ADCC result register (ADRES) is left justified, so the conversion data stored in ADRESH is used for the computation and the data in ADRESL register is ignored.

* The ADCC conversion completion interrupt is configured as a trigger source to the DMA2 peripheral instance. The DMA2 module is configured to transmit the ADCC conversion result to the SPI Transmit buffer register. 

* The SPI1 peripheral is configured as a host with an 8 MHz peripheral clock and used to transmit the ADCC conversion data to the Data Visualizer tool through the Data Gateway Interface (DGI) of the Power Debugger board

* The UART2 peripheral instance is used to display user instructions and the selected sampling rate for the ADCC peripheral in the terminal window of the Data Visualizer tool

* In the project settings, the XC8 compiler optimization is set to optimization 2, which performs optimizations of all generic code generators and minimal assembly optimizations

## Application Flow Diagram

Figure 3 shows the application firmware flow diagram of this code example.

<p align="center">
  <img width=auto height=auto src="images/applicationFlowDiagram.png">
  <br>Figure 3. Application Flow Diagram<br>
</p>

**Note:** The pulse signal generation and signal reproduction for the Data Visualizer are facilitated using CIPs, allowing the process to run independently of the CPU. Consequently, the application firmware does not require any additional coding to achieve these functions.


## MPLAB Data Visualizer Configuration

* Connect the hardware as explained in the ***Hardware Setup*** section.
* To open the Time Plot and Terminal window on stand-alone MPLAB Data Visualizer tool, follow these steps:
	1.	Steps to Open Time Plot Window in Data Visualizer
	2.	Steps to Open Terminal Window in Data Visualizer

**Steps to open Time Plot window on MPLAB Data Visualizer**

The ADCC conversion data is transmitted to the Data Visualizer in real time through the Power Debugger, and the sampled input signal is reproduced on the Time Plot window. Follow the steps below to open and configure the Time Plot window on the Data Visualizer.
1. Enable the DGI interface of the power debugger to display the supported DGI interface. 
2. Select the SPI interface and click the ![rightAlignedSymbol](images/rightAlignedSymbol.png) (right-aligned triangle) symbol.

<p align="center">
  <img width=auto height=auto src="images/spiInterfacePowerDebugger.png">
  <br>Figure 4. Select SPI DGI Interface on Power Debugger<br>
</p>

3. From the pop-up window that appears, select the “Plot Raw” option and close the pop-up window.

<p align="center">
  <img width=auto height=auto src="images/pop-upWindowTimeplot.png">
  <br>Figure 5. Pop-up Window for SPI Interface<br>
</p>

4. To start displaying the ADC conversion result, click the ![doubleTriangleSymbol](images/doubleTriangleSymbol.png) (double triangle) symbol with the text “Show live data”, which appears at the center of the time plot window.

**Steps to Open Terminal Window in MPLAB Data Visualizer**

The user is provided with an option to choose a sampling frequency setting to be used for the ADCC peripheral from the supported frequencies by pressing the on-board switch SW0. These instructions are displayed on the Terminal window of the Data Visualizer. Additionally, the selected frequency is displayed on the Terminal window after the user selects the sampling frequency option. Follow the steps below to open and configure the terminal window.
1. Open the stand-alone MPLAB Data Visualizer tool.
2. Click the **Connections tab**, which is available on the top left of the Data Visualizer.
3. Open the COM 15 settings window. By default, the Baud Rate is set to 9600. 

**Note:** The COM port number can vary depending on the availability of the port.

<p align="center">
  <img width=auto height=auto src="images/uartInterfaceCnanoBoard.png">
  <br>Figure 6. UART Interafce on PIC18F56Q24 Cnano Board<br>
</p>

4. Click the ![rightAlignedSymbol](images/rightAlignedSymbol.png) symbol on the COM 15 tab. From the pop-up window that appears, select the “Send to Terminal” option and close the pop-up window.

<p align="center">
  <img width=auto height=auto src="images/pop-upOnTerminalWindow.png">
  <br>Figure 7. Pop-up Window for UART Interface<br>
</p>

5. Text messages start printing on the Terminal window as soon as the firmware is loaded to the microcontroller.


## Demo Operation

* Ensure the hardware is connected as explained in the ***Hardware Setup*** section, and the Data Visualizer tool is setup as explained in the ***Data Visualizer Configuration*** section. 
* Download the firmware from [Microchip-Github](https://github.com/orgs/microchip-pic-avr-examples/repositories?q=Q24&type=all&language=&sort=) and open the project in MPLAB X IDE. Build the project to make sure there are no build errors. Refer to the [MPLAB User’s Guide](https://ww1.microchip.com/downloads/en/devicedoc/50002027d.pdf) for more details on how to build the project.
* Program the microcontroller by clicking the ![programDeviceIcon](images/programDeviceIcon.png) (Make and Program Device) icon on MPLAB X IDE. 
* The waveform below represents the pulse signal of 1 kHz frequency, generated using the DAC module.

<p align="center">
  <img width=auto height=auto src="images/pulseSignalGeneration.png">
  <br>Figure 8. Pulse Signal Generation Using DAC<br>
</p>

* Once the programming is completed, observe the user instructions on the Terminal window of the Data Visualizer tool. The user LED on the board will start blinking. 

<p align="center">
  <img width=auto height=auto src="images/terminalWindowAt300ksps.png">
  <br>Figure 9. Terminal Window<br>
</p>

* By default, the application firmware is configured to sample the input signal at a rate of 300 ksps using the ADCC module. The figure below shows the reproduced signal, when it is sampled at 300 ksps using the ADCC module.

<p align="center">
  <img width=auto height=auto src="images/reproducedSignalAt300ksps.png">
  <br>Figure 10. Reproduced Signal at 300 ksps Sampling Rate<br>
</p>

**Note:**: In the Time Plot window, the X-axis represents time and the Y-axis represents ADC count value. The ADC conversion result is left justified, and the resultant 8-bit data stored in the ADRESH register is plotted on the time-plot window.


* Now Press the switch (SW0) to change the ADCC sampling frequency to 50 ksps
* The configured sampling frequency and the instructions for the user to switch the sampling rate are displayed on the terminal window, as shown below 

<p align="center">
  <img width=auto height=auto src="images/terminalWindowAt50ksps.png">
   <br>Figure 11. Terminal Window<br>
</p>

* Additionally, the user LED on the board will also blink at a slower rate than with the 300 ksps sampling rate setting.
* The figure below shows the reproduced signal, sampled at a rate of 50 ksps using the ADCC module. Note that in the reproduced signal, some samples are missing from the positive and negative peaks of the signal.

<p align="center">
  <img width=auto height=auto src="images/reproducedSignal50ksps.png">
  <br>Figure 12. Reproduced Signal at 50 ksps Sampling Rate<br>
</p>

**Note:**: In the Time Plot window, the X-axis represents time and the Y-axis represents ADC count value. The ADC conversion result is left justified, and the resultant 8-bit data stored in the ADRESH register is plotted on the time-plot window.

## Conclusion

This code example showcases the ability of the ADCC module within the PIC18F-Q24 microcontroller to sample input signals of 300 ksps. This high-speed sampling allows for rapid and precise data acquisition and more accurate and responsive system behavior. The integration of the ADCC and other CIPs within the microcontroller simplifies design, reduces the need for external components and enhances overall system reliability.

