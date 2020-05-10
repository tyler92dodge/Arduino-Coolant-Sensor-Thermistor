/* This example code shows how to display temperature
 *  from a standard 1 or 2 wire automotive coolant temperature
 *  sensor. These types of sensors are a simple thermistor in
 *  an industrial casing. Thermistor's change resistance based
 *  on temperature. A voltage divider circuit must be used to
 *  read the voltage to a microcontroller. Contact tylerovens@me.com
 *  if you have any questions.
 */

#define coolantsensorDivider 2970   //defines the resistor value that is in series in the voltage divider
#define coolantsensorPin A0         //defines the analog pin of the input voltage from the voltage divider
#define NUMSAMPLES 5                //defines the number of samples to be taken for a smooth average


const float steinconstA = 0.00132774106461327;        //steinhart equation constant A, determined from wikipedia equations
const float steinconstB = 0.000254470874104285;       //steinhart equation constant B, determined from wikipedia equations
const float steinconstC = 0.000000101216538378909;    //steinhart equation constant C, determined from wikipedia equations

int samples[NUMSAMPLES];                              //variable to store number of samples to be taken

void setup() {
  Serial.begin(9600);                                 //start serial monitor
}

void loop() {
  uint8_t i;                                          //integer for loop
  float average;                                      //decimal for average
  
  for (i=0; i<NUMSAMPLES; i++) {                      
    samples[i] = analogRead(coolantsensorPin);        //takes samples at number defined with a short delay between samples
    delay(10);
  }

  average = 0;
  for (i=0; i< NUMSAMPLES; i++) {
    average += samples[i];                            //adds all number of samples together
  }
  average /= NUMSAMPLES;                              //divides by number of samples to output the average

  Serial.print("Average Analog Coolant Reading = ");
  Serial.println(average);                                        //analog value at analog pin into arduino
  average = (coolantsensorDivider*average)/(1023-average);        //conversion equation to read resistance from voltage divider
  Serial.print("Coolant Sensor Resistance = ");
  Serial.println(average);

  float steinhart;                              //steinhart equation to estimate temperature value at any resistance from curve of thermistor sensor
  steinhart = log(average);                     //lnR
  steinhart = pow(steinhart,3);                 //(lnR)^3
  steinhart *= steinconstC;                     //C*((lnR)^3)
  steinhart += (steinconstB*(log(average)));    //B*(lnR) + C*((lnR)^3)
  steinhart += steinconstA;                     //Complete equation, 1/T=A+BlnR+C(lnR)^3
  steinhart = 1.0/steinhart;                    //Inverse to isolate for T
  steinhart -= 273.15;                          //Conversion from kelvin to celcius

  Serial.print("Temperature = ");
  Serial.print(steinhart);                      //prints final temp in celcius
  Serial.println(" *C");
  
  delay(1000);                                  //delay between readings
}
