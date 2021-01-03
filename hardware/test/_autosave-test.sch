EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
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
L 4xxx:4049 U?
U 1 1 5F69F88B
P 4400 4150
F 0 "U?" H 4400 4467 50  0000 C CNN
F 1 "4049" H 4400 4376 50  0000 C CNN
F 2 "" H 4400 4150 50  0001 C CNN
F 3 "http://www.intersil.com/content/dam/intersil/documents/cd40/cd4049ubms.pdf" H 4400 4150 50  0001 C CNN
	1    4400 4150
	1    0    0    -1  
$EndComp
$Comp
L Device:R R?
U 1 1 5F6A1E63
P 3900 4000
F 0 "R?" H 3970 4046 50  0000 L CNN
F 1 "R" H 3970 3955 50  0000 L CNN
F 2 "" V 3830 4000 50  0001 C CNN
F 3 "~" H 3900 4000 50  0001 C CNN
	1    3900 4000
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR?
U 1 1 5F6A2E1C
P 3900 3850
F 0 "#PWR?" H 3900 3700 50  0001 C CNN
F 1 "+5V" H 3915 4023 50  0000 C CNN
F 2 "" H 3900 3850 50  0001 C CNN
F 3 "" H 3900 3850 50  0001 C CNN
	1    3900 3850
	1    0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x01_Male J?
U 1 1 5F6A3E2C
P 3500 4150
F 0 "J?" H 3608 4239 50  0000 C CNN
F 1 "Conn_01x01_Male" H 3608 4240 50  0001 C CNN
F 2 "" H 3500 4150 50  0001 C CNN
F 3 "~" H 3500 4150 50  0001 C CNN
	1    3500 4150
	1    0    0    -1  
$EndComp
Wire Wire Line
	4100 4150 3900 4150
$Comp
L Connector:Conn_01x01_Male J?
U 1 1 5F6A4860
P 5650 4150
F 0 "J?" H 5622 4082 50  0000 R CNN
F 1 "Conn_01x01_Male" H 5622 4173 50  0000 R CNN
F 2 "" H 5650 4150 50  0001 C CNN
F 3 "~" H 5650 4150 50  0001 C CNN
	1    5650 4150
	-1   0    0    1   
$EndComp
Text Notes 5400 4300 0    50   ~ 0
arduino
Text Notes 3250 4250 0    50   ~ 0
splitflap sensor
Connection ~ 3900 4150
Wire Wire Line
	3900 4150 3700 4150
Wire Wire Line
	4700 4150 5450 4150
$EndSCHEMATC
