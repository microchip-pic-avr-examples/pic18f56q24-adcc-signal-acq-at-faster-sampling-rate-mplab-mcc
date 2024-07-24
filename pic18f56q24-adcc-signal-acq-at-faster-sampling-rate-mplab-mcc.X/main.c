/*
 * MAIN Generated Driver File
 * 
 * @file main.c
 * 
 * @defgroup main MAIN
 * 
 * @brief This is the generated driver implementation file for the MAIN driver.
 *
 * @version MAIN Driver Version 1.0.0
 */

/*
© [2024] Microchip Technology Inc. and its subsidiaries.

    Subject to your compliance with these terms, you may use Microchip 
    software and any derivatives exclusively with Microchip products. 
    You are responsible for complying with 3rd party license terms  
    applicable to your use of 3rd party software (including open source  
    software) that may accompany Microchip software. SOFTWARE IS ?AS IS.? 
    NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS 
    SOFTWARE, INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT,  
    MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT 
    WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY 
    KIND WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF 
    MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE 
    FORESEEABLE. TO THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP?S 
    TOTAL LIABILITY ON ALL CLAIMS RELATED TO THE SOFTWARE WILL NOT 
    EXCEED AMOUNT OF FEES, IF ANY, YOU PAID DIRECTLY TO MICROCHIP FOR 
    THIS SOFTWARE.
 */
#include "mcc_generated_files/system/system.h"
/**
 * **********************************************************
 *  Macros definitions
 * **********************************************************
 **/

/*
 * Below Macros are used to select the different ADC sampling frequency 
 * by loading the hex value to ADCLK register
 */
#define ADC_SAMPLING_RATE_50KSPS    (0X36)  
#define ADC_SAMPLING_RATE_300KSPS   (0X08)  


/*
 * Below Macros are used to generate a delay
 */
#define DELAY_100MS            (100)
#define DELAY_200MS            (200)
#define DELAY_50MS             (50)

/*
 * LookUpTable stored in RAM memory 
 * location to generate a  pulse signal
 * 
 */
uint8_t pulseSignalLUT[100] = {
127,124,125,123,127,123,126,122,127,127,
127,127,124,127,127,130,135,120,127,127,
127,127,127,127,127,195,254,197,127,127,
127,95,0,95,127,127,127,124,123,127,
125,127,127,127,120,125,127,127,127,127,
127,120,127,120,122,125,127,127,126,125,
123,127,125,124,123,127,128,130,132,127,
127,123,122,123,127,120,130,120,127,127,
127,127,127,127,127,127,127,197,127,130,
127,127,127,127,127,127,129,127,129,127


};
/**
 * **********************************************************
 *  File local variables
 * **********************************************************
 **/
const uint8_t applicationStartedText[] = "\r\nADCC Sampling Speed Demo\n";
const uint8_t pressSwitchText[] = "\r\nPress Switch to change ADCC Sampling Frequency\n\r";
const uint8_t ADCSamplingFrqSelectedText[] = "\r\nSampling Frequency is :";
const uint8_t samplingRate50kspsText[] = "50 ksps \n";
const uint8_t samplingRate300kspsText[] = "300 ksps \n";

//enum typedef for key press status
typedef enum {
    KEY_NO_ACTION,
    KEY_PRESSED,
    KEY_RELEASED,
} buttonState_t;

//enum typedef for adc sampling frequency selection
typedef enum {
    WAIT_FOR_SWITCH_PRESS,
    SAMPLING_RATE_50KSPS,
    SAMPLING_RATE_300KSPS,
} samplingFrequency_t;

static samplingFrequency_t adcSamplingFreqCurrState =  WAIT_FOR_SWITCH_PRESS;
static samplingFrequency_t adcSamplingFreqPrevState;
volatile buttonState_t buttonState = KEY_NO_ACTION;

/**
 * **********************************************************
 *  Function's declaration
 * **********************************************************
 **/
void SwitchPressInterruptHandler(void);
void ADCSamplingRateSelection(void);
void TimerInterruptHandler(void);
void TimerDelayUpdate(uint16_t delay);
void ADCSamplingFrqChange(const uint8_t samplingFrequency);
void SPITX_CountRegisterUpdate(void);
void ADCSamplingRateAndInstructionTextDisplay(void);


int main(void) 
{
    SYSTEM_Initialize();
    SPI1_Open(HOST_CONFIG);
	
	//SPI is configured in Transmit mode
    SPI1CON2bits.TXR = 1;
    SPI1CON2bits.RXR = 0;
    // If using interrupts in PIC18 High/Low Priority Mode you need to enable the Global High and Low Interrupts 
    // If using interrupts in PIC Mid-Range Compatibility Mode you need to enable the Global Interrupts 
    // Use the following macros to: 

    //Global Interrupt Enable API
    // INTERRUPT_GlobalInterruptEnable();

    //HIGH Global Interrupt Enable API
    INTERRUPT_GlobalInterruptHighEnable();

    //LOW Global Interrupt Enable API
    INTERRUPT_GlobalInterruptLowEnable();
    // Disable the Global Interrupts 
    //INTERRUPT_GlobalInterruptDisable(); 
   
    //DMA1 is set to low priority
    //DMA1 is used to transmit the pulse wave lut values to DAC input buffer
    DMA1_DMAPrioritySet(7);

    //DMA1 Trigger Start API
    DMA1_TransferWithTriggerStart();
    
    //DMA2 is set to high priority
    //DMA2 is used to transmit the ADC result to SPI data buffer
    DMA2_DMAPrioritySet(0);

    //DMA2 Trigger Start API
    DMA2_TransferWithTriggerStart();

    //Switch Interrupt CallBack function
    INT2_SetInterruptHandler(SwitchPressInterruptHandler);

    //Timer Interrupt CallBack function used to blink LED at different speed
    TMR0_OverflowCallbackRegister(TimerInterruptHandler);

    //ADCC Start Conversion API
    ADCC_StartConversion(channel_ANA2);
    
    //Displays ADC sampling frequency and switch press instruction on terminal window
    ADCSamplingRateAndInstructionTextDisplay();
    while (1) 
    {

        //main application API
       ADCSamplingRateSelection();

    }
}

/**
  @Param
  none
  @Returns
   none
  @Description
 ADCSamplingRateSelection API selects the different ADC Sampling Frequency depending
 on the switch press event.
 **/
void ADCSamplingRateSelection(void) 
{
    SPITX_CountRegisterUpdate();
    switch (adcSamplingFreqCurrState) 
    {
        case WAIT_FOR_SWITCH_PRESS:
            if (buttonState == KEY_RELEASED) 
            {
                adcSamplingFreqCurrState = adcSamplingFreqPrevState + 1;
                buttonState = KEY_NO_ACTION;
            }
            break;
        case SAMPLING_RATE_50KSPS:
            ADCSamplingFrqChange(ADC_SAMPLING_RATE_50KSPS);
            printf("%s %s", ADCSamplingFrqSelectedText, samplingRate50kspsText);
            TimerDelayUpdate(DELAY_200MS);
            printf("%s", pressSwitchText);
            adcSamplingFreqPrevState = adcSamplingFreqCurrState;
            adcSamplingFreqCurrState = WAIT_FOR_SWITCH_PRESS;
            break;
        case SAMPLING_RATE_300KSPS:
            ADCSamplingFrqChange(ADC_SAMPLING_RATE_300KSPS);
            printf("%s %s", ADCSamplingFrqSelectedText, samplingRate300kspsText);
            TimerDelayUpdate(DELAY_50MS);
            printf("%s", pressSwitchText);
            adcSamplingFreqPrevState = WAIT_FOR_SWITCH_PRESS;
            adcSamplingFreqCurrState = WAIT_FOR_SWITCH_PRESS;
            break;     
        default:
            break;
    }
}

/**
  @Param
  none
  @Returns
   none
  @Description
 ADCSamplingRateAndInstructionTextDisplay API displays ADC sampling frequency
 and switch press instruction to the user on terminal window. 
 **/
void ADCSamplingRateAndInstructionTextDisplay(void)
{
    DELAY_milliseconds(2000);//This delay is added to observe the text printing on terminal window
    printf("%s", applicationStartedText);
    printf("%s %s", ADCSamplingFrqSelectedText, samplingRate300kspsText);
    printf("%s", pressSwitchText);
}
/**
  @Param
   none
  @Returns
   none
  @Description
  SwitchPressInteruptHandler API is a call back function which is called at run time to detect switch press. 
 **/
void SwitchPressInterruptHandler(void) 
{
    //to detect the key pressed and released
    buttonState = KEY_RELEASED;
    __delay_ms(DELAY_100MS); // Delay is added to avoid switch debouncing
}

/**
  @Param
   none
  @Returns
   none
  @Description
  TimerInterruptHandler API is used to toggle an led at different delay period. 
 **/
void TimerInterruptHandler(void) 
{
    LED_Toggle();
}

/**
  @Param
  uint16_t delay parameter is used to pass the delay time in millisec unit
  @Returns
  none
  @Description
  TimerDelayUpdate API is used to update the timer period register on the selection
  of sampling frequency.
  Clock Frequency = 64MHz (HFINTOSC)
  Prescaler = 1:32768
  Postscaler = 1:16
  Timer frequency = (( Clock Frequency / Prescaler)/ Postscaler)
                  = ((64MHz / 32768) / 16)
                  = 122Hz
  1 Timer tick period = 1 / Timer frequency
 *                    = 1 / 122 => 8.192ms
 **/
void TimerDelayUpdate(uint16_t delay) 
{
    const float OneTimerTickInMilliSec = 8.192;
    delay = (uint16_t)((float)delay / OneTimerTickInMilliSec);
    TMR0_Stop();
    TMR0_Reload(delay);
    TMR0_Start();
}

/**
  @Param
  constant samplingFrequency is used to change the ADC clock frequency
  @Returns
  none
  @Description
  ADCSamplingFreqChange API is used to change the ADC Clock frequency on switch press event. 
  ADCLK = FOSC / (2*(n+1))
  1 TAD = 1 / ADCLK
  Tcy = FOSC/4
  ADC Conversion time = (14*TAD + 2*Tcy)
  ADC Sampling Frequency = 1 / ADC Conversion time
 **/
void ADCSamplingFrqChange(const uint8_t samplingFrequency) 
{
    ADCC_DisableContinuousConversion();
    ADCLK = samplingFrequency;
    ADCC_EnableContinuousConversion();
}

/**
  @Param
  none
  @Returns
  none
  @Description
  SPITX_CountRegisterUpdate API is used to update the SPI Count register
  after every 1 byte SPI data transmission
 **/
void SPITX_CountRegisterUpdate(void) 
{
    while (SPI1CON2bits.BUSY)
    ;
    {
        SPI1TCNT = 1;
    }
}