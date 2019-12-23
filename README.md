# Pokémon Sword & Shield automation tools

Automatically farms Watts/Masterballs/Fossils at digging duo and Shiny fossil Pokemon (with repeat-a) in Pokémon Sword & Shield by emulating a controller on a Teensy++ 2.0

## Video (Watts Farm)
[![YouTube Video](https://i.ytimg.com/vi/_R4flAlUa9U/hqdefault.jpg)](https://youtu.be/_R4flAlUa9U)

## How to use
#### Watts Farm `wattsfarmer.hex`

1. walk to a den and throw an Wishing Piece in it
2. use the date spoofing exploit until the den glows red (don't go in yet!)
3. go into the **Change Grip/Order** window on your Switch Homescreen and press nothing
4. plug in the Teensy++ 2.0

#### Masterballs `masterballs.hex`

*This uses the new VS date spamming exploit, look it up on youtube.*

1. Enable the VS date spam exploit
2. got into a Pokécenter ritgh in front of the computer
3. make sure yo ucan play the lottery today (do not play yet)
4. set you switch clock settigns to manual mode
5. go into the **Change Grip/Order** window on your Switch Homescreen and press nothing
6. plug in the Teensy++ 2.0

#### Shiny breeding `wildareabreeding.hex`

* open wildareabreeding.c in src folder
* change value at line 62 `#define cycles`to correct amount and save

1. fly to the daycare in the wild area
2. your party needs to be full and in the first slot should be a Pokémon with the **Flame Body** ability
3. selection in menu needs to hover the map button
4. exit the menu
5. go into the **Change Grip/Order** window on your Switch Homescreen and press nothing
6. plug in the Teensy++ 2.0

#### Release a full box of Pokémon `releasebox.hex`

*be careful using this*

1. open the box you want to release
2. go into the **Change Grip/Order** window on your Switch Homescreen and press nothing
3. plug in the Teensy++ 2.0

#### repeat-a usage `repeat-a.hex`

*repeat-a just spams the A button. You can use it to farm Fossils at the Digging Duo and/or hunting Shiny Fossils at Route 6*

Before using it do this steps:
1. go into the **Change Grip/Order** window on your Switch Homescreen and press nothing
2. plug in the Teensy++ 2.0

-----

*In case you see issues with controller conflicts while in docked mode, try using a USB-C to USB-A adapter in handheld mode. In dock mode, changes in the HDMI connection will briefly make the Switch not respond to incoming USB commands, skipping parts of the sequence. These changes may include turning off the TV, or switching the HDMI input. (Switching to the internal tuner will be OK, if this doesn't trigger a change in the HDMI input.)*

**This repository has been tested using a Teensy++ 2.0.**

#### Compiling and Flashing onto the Teensy++ 2.0
Go to the Teensy website and download/install the [Teensy Loader application](https://www.pjrc.com/teensy/loader.html). For Linux, follow their instructions for installing the [GCC Compiler and Tools](https://www.pjrc.com/teensy/gcc.html). For Windows, you will need the [latest AVR toolchain](https://www.microchip.com/mplab/avr-support/avr-and-arm-toolchains-c-compilers). See [this issue](https://github.com/LightningStalker/Splatmeme-Printer/issues/10) and [this thread](http://gbatemp.net/threads/how-to-use-shinyquagsires-splatoon-2-post-printer.479497/) on GBAtemp for more information. (Note for Mac users - the AVR MacPack is now called AVR CrossPack. If that does not work, you can try installing `avr-gcc` with `brew`.)

LUFA has been included as a git submodule, so cloning the repo like this:

```
git clone --recursive git@github.com:jarlave/pkmn-swsh-automation-tools.git
```

will put LUFA in the right directory.

Now you should be ready to rock. Open a terminal window in the `pkmn-swsh-automation-tools` directory, type `make`, and hit enter to compile. If all goes well, the printout in the terminal will let you know it finished the build! Follow the directions on flashing `wattsfarmer.hex`/`masterballs.hex`/`repeat-a.hex`/`wildareabreeding.hex`/`releasebox.hex` onto your Teensy, which can be found page where you downloaded the Teensy Loader application.

#### Thanks

Thanks to Shiny Quagsire for his [Splatoon post printer](https://github.com/shinyquagsire23/Switch-Fightstick) and progmem for his [original discovery](https://github.com/progmem/Switch-Fightstick).
Also thanks to bertrandom for his [snowball thrower](https://github.com/bertrandom/snowball-thrower) and all the modifications.