/**********************************************************************************/
/*    Board: Olimexino-85                                                         */
/*    Manufacture: OLIMEX                                                         */
/*    Based on example code provided by Olimex for the Olimexino-85 board.        */
/**********************************************************************************/

/*
  Taken from Olimexino-85 documentation and examples
  
  Action:
  =======
  Switches over Olimexino-85's STAT led, i.e. pin #1 when the Button is pressed
  The Button must be connected to Olimexino-85's pin #2
  
  Simple Schematic:
  =================
  |---------------------------|        --------| R=10k |--------|vcc
  |                    |  #0  |        |       
  |                    |  #1  |        |       --------
  |                    |  #2  |--------o------| Button |--------|gnd
  |    OLIMEXINO-85    |  #3  |                --------
  |                    |  #4  |            
  |                    | #RST |
  |                    | GND  |---------------------------------|gnd
  |                    | VCC  |---------------------------------|vcc
  |---------------------------|     

  To upload to your Olimexino-85:
  1) Select the proper board from the Tools->Board Menu->Olimexino-85 ("Olimex AVR Boards" section)
  2) Unplug the device
  3) Press upload button
  4) When you see the message: "Plug in device now... (will timeout in 60 seconds)", Plug in the Olimexino-85
*/

/* Pin numbering and interrupts
 * On Olimexino-85 board, the following pins are indeed the same pin:
 * - Olimexino-85 board pin 2
 * - Attiny85 MCU pin 7
 * This pin is attached to interrupt n. 0.
 */
#define INTERRUPT_INDEX_FOR_PIN2 0
#define BUTTON_PIN               2
#define LED_PIN                  0

#define LOOP_DELAY_MS            500
#define FADE_DELAY_MS            5
#define BLINK_NORMAL_DELAY_MS    500
#define BLINK_FAST_DELAY_MS      250
#define BLINK_FAST_AND_SLOW_REP  5
#define BLINK_FAST_AND_SLOW_FAST_MS 100
#define BLINK_FAST_AND_SLOW_SLOW_MS 400

#define NUMBER_OF_STATES         6

//---------------------------------------------------------------------------------------------
// Globals
volatile byte g_nState = 0;

//---------------------------------------------------------------------------------------------
void setup()
{  
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  pinMode(BUTTON_PIN, INPUT);
  attachInterrupt(INTERRUPT_INDEX_FOR_PIN2, pin_interrupt, FALLING);
}

//---------------------------------------------------------------------------------------------
void loop()
{
  g_nState = g_nState % NUMBER_OF_STATES;
  switch(g_nState)
  {
    case 0:
    {
      state_fade_in_out();
      break;
    }
    case 1:
    {
      state_fade_and_stop();
      break;
    }
    case 2:
    {
      state_blink(BLINK_NORMAL_DELAY_MS);
      break;
    }
    case 3:
    {
      state_blink(BLINK_FAST_DELAY_MS);
      break;
    }
    case 4:
    {
      state_blink_fast_and_slow();
      break;
    }
    case 5:
    {
      state_off();
      break;
    }
  }
}

//---------------------------------------------------------------------------------------------
void pin_interrupt()
{
  ++g_nState;
}

//---------------------------------------------------------------------------------------------
void state_fade_in_out()
{
  for (int ix = 0; ix < 256; ix++)
  {
    analogWrite(LED_PIN, ix);
    delay(FADE_DELAY_MS);
  }
  for (int ix = 255; ix >= 0; ix--)
  {
    analogWrite(LED_PIN, ix);
    delay(FADE_DELAY_MS);
  }
  delay(LOOP_DELAY_MS);
}

//---------------------------------------------------------------------------------------------
void state_fade_and_stop()
{
  for (int ix = 0; ix < 256; ix++)
  {
    analogWrite(LED_PIN, ix);
    delay(FADE_DELAY_MS);
  }

  // now stop
  analogWrite(LED_PIN, 0);
  delay(LOOP_DELAY_MS);
}

//---------------------------------------------------------------------------------------------
void state_blink(unsigned int nDelayMs)
{
  digitalWrite(LED_PIN, HIGH);
  delay(nDelayMs);
  digitalWrite(LED_PIN, LOW);
  delay(nDelayMs);
}

//---------------------------------------------------------------------------------------------
void state_blink_fast_and_slow()
{
  for (int ix = 0; ix < BLINK_FAST_AND_SLOW_REP; ++ix)
  {
    digitalWrite(LED_PIN, HIGH);
    delay(BLINK_FAST_AND_SLOW_FAST_MS);
    digitalWrite(LED_PIN, LOW);
    delay(BLINK_FAST_AND_SLOW_FAST_MS);
  }
  for (int ix = 0; ix < BLINK_FAST_AND_SLOW_REP; ++ix)
  {
    digitalWrite(LED_PIN, HIGH);
    delay(BLINK_FAST_AND_SLOW_SLOW_MS);
    digitalWrite(LED_PIN, LOW);
    delay(BLINK_FAST_AND_SLOW_SLOW_MS);
  }
}

//---------------------------------------------------------------------------------------------
void state_off()
{
  digitalWrite(LED_PIN, LOW);
  delay(LOOP_DELAY_MS);
}
