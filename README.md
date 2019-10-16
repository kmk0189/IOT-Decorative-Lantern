# Smart IOT Decorative Lantern
A smart lantern that can be controlled from a phone or google home. Its included light effects that are able to be put on half or all the LEDs. This allows for custom lighting scenes that are constructed within the lantern itself.

This repository is the code that runs on the lantern itself. To view the hardware, please see this repo<TBD>.

Effects
-----------
The device is designed to allow for a single effect for the entire led puck or half for two effects to be combined. e.g. An orange color on even numbered leds and a lightning effect on odd numbered leds to illuminate what is located inside the lantern.

| Effect Name | Description |
|-------------|-------------|
| Blink       | LEDs blink on and off at an interval. |
| Bounce      | Each LED lights up before the next one. (e.g. Knight Rider). |
| Lightning   | Recreates a strobe effect to mimic lightning. |
| Rainbow     | The LEDs cycle through the primary colors of the rainbow. |
| Raceway     | Each LED takes a turn at full brightness before the next one lights up, the previous then dims a little bit forming a tail.|
| Solid       | A solid color at a specified brightness. |
| Stars       | A random LED fades in and out at a random interval. |

*Note: Each effect may be all or half the leds (even or odd). *

Communication
-----------
The code uses a MQTT client to talk with the server, the overview of the system is below.

[[https://github.com/kmk0189/IOT-Decorative-Lantern-Code/blob/develop/docs/Overview.png|alt=overview]]
