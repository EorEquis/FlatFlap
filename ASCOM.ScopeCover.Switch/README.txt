## What 
	Motorized ScopeCover - To be used as an ASCOM Switch to open/close a scope cover
	V2 will support a relay to turn a light/el panel on/off for flats.

## Who
	Created By:  eorequis@stuffupthere.com


## When
	Last modified:  2022-02-10

## Current State
	ASCOM Switch Driver updated to V2 protocol (0 = close, 1 = open), conformance passed
		Eventually an ASCOM CoverCalibrator driver is coming to allow use w/ EL Panel.  Device will continue to support either ASCOM driver,
			for those who wish only a cover.

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

## More Help
	
	See the github README at https://github.com/EorEquis/ScopeCover/tree/V2



