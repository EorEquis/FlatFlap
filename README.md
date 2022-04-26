## What 
	A "Cover/Calibrator".  Features a motorized telescope cover, and optional calibrator 
	(light/led/EL panel) controller.

## Who
	Created By:  eorequis@stuffupthere.com


## When
	Last modified:  2022-02-11

## Current State
	V2 Arduino Firmware
		The basic syntax of the serial connection protocol is :
		c00n# : c = command ([P]ing, [C]over, [L]ight) and n is typically a value mapped to ASCOM enums
		for cover and calibrator.
		
		List of common commands:
		P000#
		Ping to check if board is connected. Returns P999* when successful.
		
		C001#
		Close the cover. Returns C020* if closing, and C011* if already closed.
		
		C003#
		Open the cover. Returns C020* if opening, and C031* if already opened.
		
		L001#
		Turn off light. Returns L010* if it switch off, and L011* if already off.
		S001* or S003* means the light is not configured.
		
		L003#
		Turn On light. Returns L030* if it switched on, and L031* if already on.
		S001* or S003* means the light is not configured.
		
		S000#
		Check staus. Returns S0[light status][cover status]*
		Light status can be 0 (not configured), 1 (off), or 3 (on).
		Cover status can be 1 (closed), 2 (moving), or 3 (open).
		Any other numbers indicate an error.
		
	ASCOM Driver Options. Use only one, you cannot connect to both at the same time. Only CoverCalibrator can
	control both the cover and light.
	
	ASCOM Switch Driver updated to V2 protocol 
		The ASCOM.ScopeCover.Switch can function as a switch driver to control only the cover.  Useful
		if you don't want/need the complexity of a calibration light, and also allows greater flexibility 
		with various clients/sequencing strategies.
		
		Download ScopeCover Setup.exe to install this driver.
	
	ASCOM CoverCalibrator Driver
		The ASCOM.FlatFlap.CoverCalibrator driver can be used with either a cover only (the light 
		functionality will still present in the client, but will do nothing) or with some sort of 
		switchable light source.  (My own device is a small EL sheet and 12V DC inverter, 
		controlled by a small relay). 
		
		Download FlatFlap Setup.exe to install this driver.
	
## Tested With
	Servos: 
		HiTec HS-635HB - https://servodatabase.com/servo/hitec/hs-635hb
		Aero Sport CS-28R - They're so old there is no URL.
		SG-90 Micro Servos.  There's dozens of various brands with the same model number.  Any should work.
			
	Arduino Boards:
		Gikfun Arduino Nano Clone - https://www.amazon.com/gp/product/B00SGMEH7G
		Arduino Uno - https://www.amazon.com/gp/product/B008GRTSV6/
		ELEGOO UNO R3 Board - https://www.amazon.com/ELEGOO-Board-ATmega328P-ATMEGA16U2-Compliant/dp/B01EWOE0UU
		ELEGOO MEGA R3 Clone - https://www.amazon.com/gp/product/B01H4ZLZLQ/
		Some Uno clone I can't even find a name of or link to.
	
	Relays:
		ARCELI KY-019 5V One Channel Relay - https://www.amazon.com/gp/product/B07BVXT1ZK/
		
	Calibrator Lights
		Adafruit EL Panel - https://www.adafruit.com/product/625 
		Adafruit 12V EL inverter - https://www.adafruit.com/product/448
		
	Software:
		Sequence Generator Pro 
			4.0.0.700 64 Bit
			4.1.0.741 64 Bit
		N.I.N.A 
			V2.0 BETA 019 
			V2.0 BETA 045
			V2.0 BETA 049
			V2.0 RC001

## Usage Notes
	V2 Arduino Firmware configuration
		Compile and upload the firmware with the Arduino IDE. Make sure to choose
		the correct board, processor, and port under the "Tools" menu. Everything
		you need to change is in "Config.h"
		
		DEBUG cannot be used with the ASCOM driver. It gives aditional responses
		in the serial monitor, that the driver cannot understand.
		
		SERVOREVERSE is needed if the cover closes when you try to open it and vice
		versa. You may have to switch the closedPos and openPos values, since this
		option only changes direction.
		
		RELAYLOW is needed if the light turns on when you try to turn it off and
		vice versa.
		
		CALIBRATOR needs to be defined if you are using a light panel. Comment it
		out if you only want to use the scope cover.
		
		closedPos may need to be adjusted to keep from trying to force the servo
		past the fully closed position. If the servo makes noise when closed,
		adjust until it is sealed but quiet.
		
		openPos can be adjusted if you want the cover to open a different amount.
		
		servoPin can be changed if you want to use a differnt pin for the servo.
		
		lightPin can be changed if you want to use a different pin for the relay.
		
		servodelay controls the speed the servo moves.
	
	Arduino resets on serial connection. (Ignore if using ScopeCover only)
		The ASCOM driver code by default is compiled with a 2000ms wait when
		connecting. This will give the Arduino time to reset before use.
		
		Optionally, you can stop it from resetting by adding a 10uf capacitor to
		the arduino between GND and RST. You can then remove the wait(2000) line
		in the Connected(Set) from the ASCOM driver code, recompile and resintall
		the driver.
		
	SGP Switch Handling
		Sequence Generator Pro (As of v4 anyway) handles switches...strangely.  
		After initial install, the first time you run SGP, it will claim to 
		find and connect to the switch, but probably won't actually connect.  
		(Unless your arduino is COM1...unlikely)
		
		To "solve" this either go to SGP's Switches tab in SGP Control Panel, and 
		change the switch's setting to reflect the correct COM Port, then stop and 
		restart SGP.

		OR
		
		Before launching SGP, open any other client program that doesn't try to 
		auto-connect every switch driver on your system, and configure the ScopeCover 
		switch to the correct COM port before connecting to it.  Disconnect and exit 
		gracefully, and the setting will be retained in the device profile.
		
		OR
		
		Before running SGP after switch installation, launch ASCOM Profile Explorer, 
		find "ASCOM.ScopeCover.Switch" in the "Switch Drivers" section on the left.  
		Click "ASCOM.ScopeCover.Switch", and then in the right pane, add a new Value
		called "COM Port" (Note the space) with Data of "COMn" (Note the lack of a 
		space) where n is your device's com port.

![image](https://user-images.githubusercontent.com/6656546/153234209-96820275-60fd-41fb-8752-63429af6ffd7.png)
		
		You should also note : If you install the Switch Driver, and later the CC 
		driver, using the same arduino, SGP will retain the previous switch driver's 
		config, and auto-connect to that COM port, thus causing connection attempts to 
		the new CC driver to fail, Access to COM Port Denied.



