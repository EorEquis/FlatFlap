## What 
	A "Cover/Calibrator".  Features a motorized telescope cover, and optional calibrator 
	(light/led/EL panel) controller.

## Who
	Created By:  eorequis@stuffupthere.com


## When
	Last modified:  2022-02-11

## Current State
	V2 Arduino Firmware
		Protocol documented in arduino code, basic syntax is :
		c00n# : c = command ([P]ing, [C]over, [L]ight) and n is typically a value mapped to ASCOM enums
		for cover and calibrator.
		
	ASCOM Switch Driver updated to V2 protocol 
		The ASCOM.ScopeCover.Switch can function as a switch driver to control only the cover.  Useful
		if you don't want/need the complexity of a calibration light, and also allows greater flexibility 
		with various clients/sequencing strategies.
		
		Download ScopeCover Setup.exe if all you want is this driver.
	
	ASCOM CoverCalibrator Driver
		The ASCOM.FlatFlap.CoverCalibrator driver can be used with either a cover only (the light 
		functionality will still present in the client, but will do nothing) or with some sort of 
		switchable light source.  (My own device is a small EL sheet and 12V DC inverter, 
		controlled by a small relay).
		
		Download FlatFlap Setup.exe if all you want is this driver.
	
## Tested With
	Servos: 
		HiTec HS-635HB - https://servodatabase.com/servo/hitec/hs-635hb
		Aero Sport CS-28R - They're so old there is no URL.
			
	Arduino Boards:
		Gikfun Arduino Nano Clone - https://www.amazon.com/gp/product/B00SGMEH7G
		Arduino Uno - https://www.amazon.com/gp/product/B008GRTSV6/

	Calibrator Lights
		Adafruit EL Panel - https://www.adafruit.com/product/625 
		Adafruit 12V EL inverter - https://www.adafruit.com/product/448
		
	Software:
		Sequence Generator Pro 4.0.0.700 64 Bit
		N.I.N.A V2.0 BETA 019 and V2.0 BETA 045

## Usage Notes
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



