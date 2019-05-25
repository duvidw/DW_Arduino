# DW_Arduino
Place holder for Arduino projects

====================================================================================
V0.1.0
25 May, 2019
Add DWString class base on Arduino String calss
The new DWString class will hold spcial parsing functions

====================================================================================
V0.1
25 May, 2019
Board: ESP8266 D1 mini
Project: Wifi server that get command as string from client.
Commands:
"20" - Led Off
"21: - Led On
"22" - Led Blink
        Led bling uns timer clock not delay
        I tried to use interrupt with no success yet!
        
"1" - Send "Hello David!" to client
"2=n" - Extruct the n as integer number between 0-1023 and analogwrite to gpio pin0
