
# Real time condition monitoring system for industrial motors using current signature

The present code aim is to implement IoT for data acquisition of BLDC motors by recording different values of current and send to cloud for monitoring.The protocol used is mqtt


Why mqtt-> Message Queuing Telemetry Transport is used for data exchange between constrained devices and server applications. It keeps bandwidth requirements to an absolute minimum, handles unreliable networks, requires little implementation effort for developers, and is, therefore, ideal for machine-to-machine (M2M) communication.

*further read*->https://randomnerdtutorials.com/what-is-mqtt-and-how-it-works/


# --Prerequsites--

___install esp32 package___
  https://randomnerdtutorials.com/installing-the-esp32-board-in-arduino-ide-windows-instructions/
  
__install libraries__
 The following libraries can be found in arduino ide->Tools->Manage Libraries
   1. PubSubClient->for mqtt communication
   2. Emonlib ->for current reading using st 030 sensor from bldc
# --Content--   
The present git consists of three folders for individual uses:
 1. For checking of iot connection and sending some dummy data to check line chart (esp_send_dummy)
 2. For sole reading of Bldc current readings(current calc) ->this one is subjected to change according to the callinration readings please refer the site links i mentioned in        code
 3. Combined code for the above two (combined)
# --Theory--
<img width="394" alt="Capture" src="https://user-images.githubusercontent.com/28966259/118945734-5b5aff80-b973-11eb-8eaa-ff4f657d1c7a.PNG">

As it is pretty obvious that we can use Our primary current which could in range of 30A,50A,100A etc. (Ip in fig.) as it might fry up our microcontroller we would using Secondry current as our medium (Is in fig) to give us an idea. For that to happen we will require an Burden resistor as shown. So the main idea is to measure the voltage across the burden resistor through our microcontroller and in turn get the current readings (V->secondary voltage = I->secondary current x R->burden resistor)


For further reading->https://learn.openenergymonitor.org/electricity-monitoring/ctac/how-to-build-an-arduino-energy-monitor

The schematic :

<img width="568" alt="Capture" src="https://user-images.githubusercontent.com/28966259/118969576-d67be000-b98a-11eb-9fd7-be547bbc2ec0.PNG">


It is to be noted the microcontroller will not be able to read the negative cycle (indicated by yellow line in the diagram on right hand )side so for that reason we are shifting it up usig the above schematic. The input can be any of the ADC pin of arduino/ESP32

The burden resistor value will be  18 Ohms if supply voltage is 3.3V(stm32 etc.) or 33 Ohms if supply voltage is 5V (arduino and esp32)


youtube link ->https://www.youtube.com/watch?v=Z3YSHhS39Bc




 
 

