#include <Wire.h>
#include  <MQUnifiedsensor.h>
#define         Board                   ("Arduino NANO")
#define         Pin6                     (A0)  //Analog input 0 of your arduino
#define         Pin135                   (A1)  //Analog input 1 of your arduino
#define         Pin9                     (A2)  //Analog input 2 of your arduino



#define         RatioMQ6CleanAir          (4.4)  //RS / R0 = 4.4ppm 
#define         RatioMQ135CleanAir        (3.6) //RS / R0  = 3.6 ppm   
#define         RatioMQ9CleanAir          (9.6) //RS / R0 = 9.6 ppm 
#define         ADC_Bit_Resolution        (10) // 10 bit ADC 
#define         Voltage_Resolution        (5) //  Volt resolution to calc the voltage
#define         Type                      ("Arduino  NANO") //Board used

//Declare Sensor
MQUnifiedsensor MQ6(Board, Voltage_Resolution,  ADC_Bit_Resolution, Pin6, Type);
MQUnifiedsensor MQ9(Board, Voltage_Resolution,  ADC_Bit_Resolution, Pin9, Type);
MQUnifiedsensor MQ135(Board, Voltage_Resolution,  ADC_Bit_Resolution, Pin135, Type);



void setup() {
  Serial.begin(9600);
                         
//calibration to calculate R0.
  
  Serial.print("Calibrating  please wait.");
  float  MQ6calcR0 = 0,
         MQ135calcR0  = 0,
         MQ9calcR0  = 0;
  for (int i = 1; i <= 10; i ++)
  {
    //Update the voltage lectures
    MQ6.update();
    MQ135.update();
    MQ9.update();

    MQ6calcR0 += MQ6.calibrate(RatioMQ6CleanAir);
    MQ135calcR0 += MQ135.calibrate(RatioMQ135CleanAir);
    MQ9calcR0 += MQ9.calibrate(RatioMQ9CleanAir);

    Serial.print(".");
  }
  MQ6.setR0(MQ6calcR0 / 10);
  MQ135.setR0(MQ135calcR0  / 10);
  MQ9.setR0(MQ9calcR0  / 10);
  Serial.println("  done!.");

  Serial.print("(MQ6 - MQ9):");
  Serial.print(MQ6calcR0 / 10); Serial.print(" | ");
  Serial.print(MQ135calcR0 / 10); Serial.print("  | ");
  Serial.print(MQ9calcR0 / 10); Serial.println("  |");

  
}


void loop() {
  //Update the voltage lectures
  MQ6.update();
  MQ135.update();  
  MQ9.update();

 
  MQ135.setA(110.47); MQ135.setB(-2.862);  //CO2 
float CO2 = MQ135.readSensor(); 
  
  MQ135.setA(44.947); MQ135.setB(-3.445);  // Toluene
float Toluene = MQ135.readSensor(); 
  
  MQ135.setA(102.2 );  MQ135.setB(-2.473); //NH4 
float NH4 = MQ135.readSensor(); 
  
  MQ135.setA(34.668);  MQ135.setB(-3.369); //Acetone
float Acetone = MQ135.readSensor(); 
 
  MQ9.setA(99.042);  MQ9.setB(-1.518); //CO
float CO = MQ9.readSensor();


  Serial.print("CO2:      "); Serial.println(CO2);
  Serial.print("Toluene:  "); Serial.println(Toluene);
  Serial.print("NH4:      "); Serial.println(NH4);
  Serial.print("Acetone:  "); Serial.println(Acetone);
  Serial.print("CO:       "); Serial.println(CO);
  Serial.println("--------------------------------------------------------");

  delay(2000);
}
