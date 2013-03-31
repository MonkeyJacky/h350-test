# The H350-test Tool #

Use for H350 board testing. It can work on H350 board and ubuntu OS now (other OS haven't tested).


## To prepare? ##

* To ensure that your computer's equipped with the SDL library.
* Get mipsel compile tools, extract it under '/opt/' path.


## Building from source ##

1. Make under the root path:
    * Run on Ubuntu: make -f Makefile.ubuntu
    * Run on H350 board: make -f Makefile.h350
    * If you have the python, run configure to make more easier, after run 
      the configure, you can just click 'make' to build the source code.
      Or you can manually modify the file to achieve the same purpose.
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
12. Select Mode:
    * There's a list, you can choose the item what you want to test. Using UP and DOWN for moving, A for confirming, B for going on whole test.


## The follow-up function on Continuous Improvement ... ##
