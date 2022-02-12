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
	SGP Switch Handling
		If you install the Switch Driver, and later the CC driver, using the same arduino, SGP
		will retain the previous switch driver's config, and auto-connect to that COM port, thus causing connection
		attempts to the new CC driver to fail, Access to COM Port Denied.	





