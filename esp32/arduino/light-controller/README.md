Send these payloads to the appropriate MQTT topic (for example, `led/control`) to control the LEDs as desired. Pattern **"LEDNAME:COMMAND"** 

| **LED Command**                                | **Example Payload**                     | **Description**                                                            |
|------------------------------------------------|-----------------------------------------|----------------------------------------------------------------------------|
| **Turn LED On**                                | `RED:ON`                                | Turns the RED LED on.                                                      |
| **Turn  LED Off**                               | `BLUE:OFF`                              | Turns the BLUE LED off.                                                    |
| **Toggle LED**                                 | `GREEN:TOGGLE`                          | Toggles the current state of the GREEN LED.                                |
| **Blink LED Continuously**                     | `RED:BLINK,500,0`                       | Blinks the RED LED every 500ms with infinite cycles (0 = continuous).        |
| **Blink LED a Specific Number of Times**       | `BLUE:BLINK,250,5`                      | Blinks the BLUE LED every 250ms for 5 cycles.                              |
| **Fade LED (a Specific Number of Cycles)**       | `GREEN:FADE,0,255,2000,2`                | Fades the GREEN LED from brightness 0 to 255 over 2000ms, repeated 2 cycles. |
| **Fade LED Continuously**                      | `RED:FADE,0,255,1500,0`                  | Fades the RED LED continuously from 0 to 255 brightness (0 cycles means infinite). |