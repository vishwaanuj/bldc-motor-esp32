
#include "EmonLib.h"     // Include Emon Library
#define VOLT_CAL 148.7   //will be particular to your project
#define CURRENT_CAL 62.6 //will be particular to your project

//// for total callibration theory->https://github.com/openenergymonitor/learn/blob/master/view/electricity-monitoring/ctac/emonlib-calibration-theory.md
///refer to this video -> https://www.youtube.com/watch?v=Ieuqvo5qxRc for calibration purpose of voltage

// CURRENT_CAL is calculated by the following formula :
//(CT_ratio)/(Burden Resistance)=(100A/.05A)/33 ohms = 62.6

//refer for what is ct ratio -> https://sciencing.com/determine-primary-secondary-transformer-6117755.html


EnergyMonitor emon1;             // Create an instance

void setup()
{  
  Serial.begin(9600);
  
  emon1.voltage(1, VOLT_CAL, 1.7);  // Voltage: input pin, calibration, phase_shift
  emon1.current(0, CURRENT_CAL);       // Current: input pin, calibration.
}

void loop()
{
  emon1.calcVI(20,2000);         // Calculate all. No.of half wavelengths (crossings), time-out

  float currentDraw            = emon1.Irms;             //extract Irms into Variable
  //float supplyVoltage   = emon1.Vrms;                    //extract Vrms into Variable
  Serial.print("Current: ");
  Serial.println(currentDraw);

}
