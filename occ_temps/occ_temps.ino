:// Thanks to Pieter Heremans and George Laskowsky for their advises and lessons.
//
// read voltage passing trough a voltage divider and geting temperature


const float rinfinit = 93.79645;   // This you should calculate yours
const float voltage = 5.04;        // should change it to 5040 for 5.04V
const float B = 3380;
const float unodivr = 8.3832;  //93.79645;

const float resistor0 = 9830;
const float resistor1 = 9970;
const float resistor2 = 9850;
const float resistor3 = 9870;

int sensorPin0 = A0;               
int sensorPin1 = A1;               
int sensorPin2 = A2;               
int sensorPin3 = A3;               
int sensorValue0 = 0;               
int sensorValue1 = 0;               
int sensorValue2 = 0;               
int sensorValue3 = 0;               


void setup() {
Serial.begin(9600);                                    //  setup serial
   pinMode(11, OUTPUT);
   analogWrite(11, 255); 
}

void loop() {
  static unsigned char curspeed=20;  
  static unsigned char delaycnt=1;
  // Part 1:  Read the analog port and convert the value to volts.
  sensorValue0 = analogRead(sensorPin0);
  sensorValue1 = analogRead(sensorPin1);
  sensorValue2 = analogRead(sensorPin2);
  sensorValue3 = analogRead(sensorPin3);
  float v20 = (voltage*float(sensorValue0))/1024.0f;    // calculate volts in an awesome world
  float v21 = (voltage*float(sensorValue1))/1024.0f;    // calculate volts in an awesome world
  float v22 = (voltage*float(sensorValue2))/1024.0f;    // calculate volts in an awesome world
  float v23 = (voltage*float(sensorValue3))/1024.0f;    // calculate volts in an awesome world                                                             
  
 // Part 2: Calculate the resistance with the volt value trough the voltage divider ecuation
  //voltage = 5.04
  //R2 = 10000
  //R1 = Thermistor resistance
  //V2= v2
  //so V2=(R2*V)/(R1+R2)
  //and r1=((r2*v)/v2)-r2 <--final
  
  //ur2=ur1r2*r2/(r2+r1) 
  
  //(voltage*float(resistor0))/v20 - resistor0;
  float r10 = v20*resistor0/(voltage-v20);
  float r11 = v21*resistor1/(voltage-v21);
  float r12 = v22*resistor2/(voltage-v22);
  float r13 = v23*resistor3/(voltage-v23);
 
  //Part 3: Calculate the temperature based on the Steinhart-Hart equation based on the B value equation.  
  // back to first ecuation
  // T=B/ln(r1/rinfinit)
  // where
  // r infinit = 0.0106613843
  // To save calcs
  // 1/r infinit = unodivr= 93,796450053864018390182220520838 cut to 93,79645
  // B=4100 

  //r1=10000; <---uncomment to check if the next eq is working
  float T0 = B/log(r10*unodivr) -273.15;
  float T1 = B/log(r11*unodivr) -273.15;
  float T2 = B/log(r12*unodivr) -273.15;
  float T3 = B/log(r13*unodivr) -273.15;
  Serial.print("Temperatures T0: "); Serial.print(T0, 4);
  Serial.print(" T1: "); Serial.print(T1, 4);
  Serial.print(" T2: "); Serial.print(T2, 4);
  Serial.print(" T3: "); Serial.println(T3, 4);
  delay(1000);
  
  float Treg=(T1>T0) ? T1 : T0;
  
  if((Treg>80) )
  {
    curspeed=255;
    analogWrite(11, curspeed); 
  }
  if(Treg<30)
  {
     analogWrite(11, 0); 
  }
  delaycnt--;
  if(!delaycnt)
  {
    delaycnt=5;
    if((Treg>50) && (curspeed<255))
    { 
       curspeed++;
       analogWrite(11, curspeed); 
    }
    if(Treg>35)
    {
       if((Treg<45) && (curspeed>20))
       {
         delaycnt=10;
         curspeed--;
       }
       analogWrite(11, curspeed); 
    }
    Serial.print("Speed: "); Serial.println(curspeed, DEC);
  }  
}
