EESchema Schematic File Version 4
LIBS:Démo Shiel Uno-cache
EELAYER 29 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 2 2
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L Switch:SW_DPST SW?
U 1 1 6012A3D1
P 5150 2750
F 0 "SW?" H 5150 3075 50  0000 C CNN
F 1 "SW_DPST" H 5150 2984 50  0000 C CNN
F 2 "" H 5150 2750 50  0001 C CNN
F 3 "~" H 5150 2750 50  0001 C CNN
	1    5150 2750
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 6012A8F7
P 4700 2950
F 0 "#PWR?" H 4700 2700 50  0001 C CNN
F 1 "GND" H 4705 2777 50  0000 C CNN
F 2 "" H 4700 2950 50  0001 C CNN
F 3 "" H 4700 2950 50  0001 C CNN
	1    4700 2950
	1    0    0    -1  
$EndComp
Wire Wire Line
	4950 2850 4700 2850
Wire Wire Line
	4700 2850 4700 2950
Wire Wire Line
	4950 2650 4700 2650
Wire Wire Line
	4700 2650 4700 2850
Connection ~ 4700 2850
Wire Wire Line
	5350 2650 5550 2650
Wire Wire Line
	5350 2850 5550 2850
Wire Wire Line
	5550 2850 5550 2650
Connection ~ 5550 2650
Wire Wire Line
	5550 2650 6000 2650
Text HLabel 6000 2650 2    50   Input ~ 0
IN1
$EndSCHEMATC
