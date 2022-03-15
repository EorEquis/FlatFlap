## What 
	ASCOM Cover Calibrator driver for use with Flat Flap.

## Who
	Created By:  eorequis@stuffupthere.com

## When
	Last modified:  2022-02-11

## Current State
	Conformance passed
	
## Tested With
	Software:
		Sequence Generator Pro 4.0.0.700 64 Bit
		N.I.N.A 2.0 BETA 019

## Usage Notes
	Arduino resets on serial connection.
		You can handle this 1 of 2 ways.  
		
		Either retain the 2000ms wait in the Connected(Set)
		of the ASCOM driver code (it is there by default and both setups include drivers
		compiled with this option)
		
		OR
		
		Add a 10uf capacitor to the arduino between GND and RST, remove the wait(2000) 
		line from the driver code, recompile and resintall the driver.
		
	SGP Switch Handling
		If you install the Switch Driver, and later the CC driver, using the same arduino, SGP
		will retain the previous switch driver's config, and auto-connect to that COM port, thus causing connection
		attempts to the new CC driver to fail, Access to COM Port Denied.	





