#include <Arduino.h>
#include "Sonar.h"
#include "Sonar_Data.h"



//Constructor with member initialization
Sonar::Sonar(uint8_t pin, SONAR_INPUT_t input, SONAR_FILTER_t filter, uint8_t sample_size) : 
	pin(pin), input(input), filter(filter), sample_size(sample_size){
		pinMode(pin, INPUT);
    pinMode(SONAR_TRIGGER, OUTPUT); //should this go somewhere else?
		init();
}

//Initialization
void Sonar::init(){ //init sample size based on filter method

  //Init trigger output low
  digitalWrite(SONAR_TRIGGER, LOW);  
	
}

//Trigger reading
void Sonar::trigger(){
  digitalWrite(SONAR_TRIGGER, LOW);
  delayMicroseconds(2);
  digitalWrite(SONAR_TRIGGER, HIGH);
  delayMicroseconds(30);
  digitalWrite(SONAR_TRIGGER, LOW);
}

//Read sample and get filtered distance
float Sonar::getDistance(){
	float dist;
	
	readSample();
	
	switch(filter){
      
		case MEDIAN:
			dist = getSampleMedian();
			break;
			
		case HIGHEST_MODE:
			dist = getSampleMode(true);
			break;
			
		case LOWEST_MODE:
			dist = getSampleMode(false);
			break;
			
		case BEST:
			dist = getSampleBest();
      break;
   
    case NONE:
      dist = sample[sample_size-1];
      break;
    
		default:
			dist = sample[sample_size-1];
			break;
	}
	
	return dist;
}

//Read sensor pin [cm] based and use conversion factor
float Sonar::readSensor(){

	float result;

  trigger();
  
	switch (input) {
		
		//Analog voltage is read from the sensor in V
    //Sensor conversion: (Vcc/512)/inch
    //8bit Arduino => (1024/512)/inch = 2/inch
		case AN:
			result = (analogRead(pin) / 2.0) * 2.54;
			break;

    //Pulse-width is read from sensor in us
    //Sensor conversion: (147uS/inch)
		case PW:
			result = (pulseIn(pin, HIGH) / 147.0) * 2.54;
			break;
			
		default:
			break;
	}
	
	return result;
}

//Shift sample array elements left and reads sensor to the last element
void Sonar::readSample(){ 
	
	for (uint8_t i=0; i < sample_size-1; i++) {
	sample[i] = sample[i+1];
	}
 
	sample[sample_size-1] = readSensor();
  
	//sorts sample from least to greatest
	sortSample();
}	

//Copy sample array for sorting
void Sonar::copySample(){
  for (uint8_t i = 0; i < sample_size; i++){
    sample_sorted[i] = sample[i];
  }
}

//Sort sample for determining median/mode
void Sonar::sortSample(){ //sort the sample from least->greatest
  
  //copy the sample to be sorted
  copySample();
  
	for(uint8_t i = 1; i < sample_size; i++){
		float j = sample_sorted[i];
		uint8_t k;
		
		for(k = i - 1; (k >= 0) && (j < sample_sorted[k]); k--)
			sample_sorted[k + 1] = sample_sorted[k];
		
		sample_sorted[k + 1] = j;
	}
}

float Sonar::getSampleMedian(){
	return sample_sorted[sample_size / 2];
}

float Sonar::getSampleMode(bool highest){
    float mode = sample_sorted[0];
    uint8_t mode_count = 1;
    uint8_t count = 1;

    for (uint8_t i = 1; i < sample_size; i++) {
        if (sample_sorted[i] == sample_sorted[i - 1])
            count++;
        else
            count = 1;

        if (sample_sorted[i] == mode)
            mode_count++;
        else if (!highest && count > mode_count || highest && count == mode_count) {
            mode_count = count;
            mode = sample_sorted[i];
        }
    }

    return mode;
}
 
float Sonar::getSampleBest(){
    float dist;

    if ((dist = getSampleMode(true)) != getSampleMode(false))
        dist = getSampleMedian();

    return dist;
}	

//PrintArray for debug
void printArray(float* array, uint8_t array_size) {
  Serial.print("[");
  for (uint8_t i = 0; i < array_size; i++) {
    Serial.print(array[i]);
    if (i != array_size - 1) {
      Serial.print(", ");
    }
  }
  Serial.print("]");
}

//Print getDistance, unsorted/sorted array, high mode, low mode, and median
void debug_front() {
  unsigned long start;
  start = millis();
  Serial.print("BEST: ");
  Serial.print(frontSonarPW.getDistance());
  Serial.print("cm - ");
  Serial.print(millis() - start);
  Serial.print("ms");
  Serial.println();
  Serial.print("Unsorted: ");
  printArray(frontSonarPW.getSample(), frontSonarPW.getSampleSize());
  Serial.println();
  Serial.print("  Sorted: ");
  printArray(frontSonarPW.getSampleSorted(), frontSonarPW.getSampleSize());
  Serial.println();
  Serial.print("Highest Mode: ");
  Serial.print(frontSonarPW.getSampleMode(true));
  Serial.print("  /  Lowest Mode: ");
  Serial.print(frontSonarPW.getSampleMode(false));
  Serial.print("  /  Median: ");
  Serial.print(frontSonarPW.getSampleMedian());
  Serial.println();
  Serial.println();
}

void debug_right() {
  unsigned long start;
  start = millis();
  Serial.print("BEST: ");
  Serial.print(rightSonarPW.getDistance());
  Serial.print("cm - ");
  Serial.print(millis() - start);
  Serial.print("ms");
  Serial.println();
  Serial.print("Unsorted: ");
  printArray(rightSonarPW.getSample(), rightSonarPW.getSampleSize());
  Serial.println();
  Serial.print("  Sorted: ");
  printArray(rightSonarPW.getSampleSorted(), rightSonarPW.getSampleSize());
  Serial.println();
  Serial.print("Highest Mode: ");
  Serial.print(rightSonarPW.getSampleMode(true));
  Serial.print("  /  Lowest Mode: ");
  Serial.print(rightSonarPW.getSampleMode(false));
  Serial.print("  /  Median: ");
  Serial.print(rightSonarPW.getSampleMedian());
  Serial.println();
  Serial.println();
}
