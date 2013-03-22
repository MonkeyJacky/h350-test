# The H350-test Tool #

Use for H350 board testing. This can work only on h350 board, I'll change it work on PC later.


## To prepare? ##

* To ensure that your computer's equipped with the SDL library.
* Get mipsel compile tools, extract it under '/opt/' path.


## Building from source ##

1. Make under the root path:
    * On Ubuntu: make
    * Haven't ran on other systems.


## How's the function ##

1. Look at the test.conf file, the keyword 'test\_order', the test item have: joystick, keypad, headphone, speader, lcd, tfcard, avout, hdmi, wifi, internal card, bat-voltage. You can change the order of the test or change what're the project you need by changing these keywords.If you change 'test\_order' keyword, then 'test\_total\_num' should change.
2. Joystick test:
    * Follow the text prompts to trigger key.
    * Trigger joystick in all directions, turns black all the grid can go through the test.
3. Keypad test:
    * Trigger all keys on the board, turns black all the grid to go through.
4. Headphone test:
    * Follow the text prompts to plug in headphone, then will play some audio.
    * The play will be prompted to pull out, a dialog box appears to give the test results.
5. Speaker test:
    * Will play some audio, then give the test results. 
6. Lcd test:
    * The fullscreen will display three colors red, green and blue, and finally a backlight adjustment. Give the test results at last.
7. Tfcard test:
    * External card test. Prompted to insert the specified external card which has a file named 'test\_mode.txt'.
    * Then prompted to plug out the card to pass the test.
8. Avout test:
    * The avout can't work now, press A to continue scan or press B to pass the test directly.
9. Hdmi test:
    * Promted to plug in the HDMI cable, then press a key to start scan, because it is not hot plug.
    * Then will show three colors on the TV, after this give the test result.
10. Wifi test:
    * The wifi test will connected automatically, you will write some router information in the file named 'wpa\_supplicant.conf' before this test. There's an example pattern in the file.Put the file in the internal card at root path.
    * Attempts to connect three times, each call to the ping command to determine whether the connection was successful.
11. Bat-voltage test:
    * The middle of the screen will display the power value read. 
