# Automated Protective System

## System Structure
[!system structure](./diagram.jpg)

Codes of each component are uploaded.

## Specification
1. Program’s Compiling Environment and compiling way
Linux is compiling environment to compile this program and compiling way is that make the Makefile and make. Next, we copy it on the embedded board using tftp connection and access at putty console window.


2. The way to access

	2.1. The bunker is threatened by flame(Fire), sound(Boom!!), Gas, Vibration.

	2.2. When the bunker’s sensor detect attack, it close. It sends information(strength, which attack is detected).

	2.3. Each embedded board’s component reveal it’s information.
	    - Text LCD : To show which attack is detected.
	    - OLED :  To show which attack is detected(By picture).
	    - 7-Segment : To show how much outer attack’s strength (by number).
	    - Bus LED : To show how much outer attack’s strength(by the  number of light on)
	    - Dot Matrix : To show outer attack is detected.
	    - Full Color LED : To show which attack is detected ( Gas/ Green, Flame/Red ….)
	    - Buzzer : To know which attack is detected ( Flame/Pick me , Gas/ Airplane ….)
	    - Wifi : Sending log to database.
	    - Dip SW : To use all components // Identifies user( Password )
	    - Keypad : To open the bunker ( Password )
	    - LCD : To bunker open or close, To watch outer situation(Door Bell)

	2.4. At the same time, Android user can watch it’s log

	2.5. People can open or close by android app or embedded board.
	    - By using application : Android App - (WIFI) - E.B - (Serial comm) - Arduino 
	    - By using embedded board : E.B - (Serial comm) - Arduino
	    
	2.6. About Android
	    - There are 4 Views
	        - Intro : Intro menu, tab the picture.
	        - Menu : you can choose on/off mode or watch log mode.
	        - On/Off  : You can open or close the bunker.
	        - See Log : You can see logs.
