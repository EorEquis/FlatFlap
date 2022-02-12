## What 
	Motorized ScopeCover - To be used as an ASCOM Switch to open/close a scope cover
	V2 will support a relay to turn a light/el panel on/off for flats.

## Who
	Created By:  eorequis@stuffupthere.com


## When
	Last modified:  2022-02-10

## Current State
	V2 Arduino Firmware
		Protocol documented in arduino code, basic syntax is :
		c00n# : c = command ([P]ing, [C]over, [L]ight) and n is a value if necessary (0 for off or close, 1 for on or open)
	ASCOM Switch Driver updated to V2 protocol (0 = close, 1 = open), conformance passed
		Eventually an ASCOM CoverCalibrator driver is coming to allow use w/ EL Panel.  Device will continue to support either ASCOM driver,
			for those who wish only a cover.
	Very basic control app.  Open and Close buttons, and connect button.
	
## Test Cases
	Servos: 
		HiTec HS-635HB - https://hitecrcd.com/products/servos/discontinued-servos-servo-accessories/hs-635hb-karbonite-high-torque-servo/product
		Aero Sport CS-28R - They're so old there is no URL.
			
	Arduino Boards:
		Gikfun Arduino Nano Clone - https://www.amazon.com/gp/product/B00SGMEH7G
		Arduino Uno - https://www.amazon.com/gp/product/B008GRTSV6/

	Software:
		Sequence Generator Pro 4.0.0.700 64 Bit
		N.I.N.A 2.0 BETA 019

## Usage Notes
	SGP Switch Handling
		SGP handles switches...strangely.  After initial install, the first time you run SGP, it will claim to find
		and connect to the switch, but probably won't actually connect.  (Unless your arduino is COM1...unlikely)
		
		To "solve" this either go to SGP's Switches tab in SGP Control Panel, and change the switch's setting to
		reflect the correct COM Port, then stop and restart SGP.

		OR
		
		Before launching SGP, open any other client program that doesn't try to auto-connect every switch driver on your system, and configure
		the ScopeCover switch to the correct COM port before connecting to it.  Disconnect and exit gracefully,
		and the setting will be retained in the device profile.
		
		OR
		
		Before running SGP after switch installation, launch ASCOM Profile Explorer, find "ASCOM.ScopeCover.Switch" in the 
		"Switch Drivers" section on the left.  Click "ASCOM.ScopeCover.Switch", and then in the right pane, add a new Value
		called "COM Port" (Note the space) with Data of "COMn" (Note the lack of a space) where n is your device's com port.

![image](https://user-images.githubusercontent.com/6656546/153234209-96820275-60fd-41fb-8752-63429af6ffd7.png)



