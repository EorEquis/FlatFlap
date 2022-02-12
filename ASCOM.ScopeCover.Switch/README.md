## What 
	ASCOM Switch driver for use with FlipFlat, as a cover only.
	
## Who
	Created By:  eorequis@stuffupthere.com

## When
	Last modified:  2022-02-11

## Current State
	Conformance passed

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

		You should also note : If you install the Switch Driver, and later the CC driver, using the same arduino, SGP
		will retain the previous switch driver's config, and auto-connect to that COM port, thus causing connection
		attempts to the new CC driver to fail, Access to COM Port Denied.

