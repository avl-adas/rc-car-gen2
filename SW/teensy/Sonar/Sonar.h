/*


*/

#ifndef Sonar_h
#define Sonar_h


class Sonar {
public:

	//Type of sensor read
	typedef enum {
		AN,
		PW
	}
	SONAR_INPUT_t;
	
	//Type of filter
	typedef enum {
		NONE,
		MEDIAN,
		HIGHEST_MODE,
		LOWEST_MODE,
		BEST
	}
	SONAR_FILTER_t;
	
	// init
	Sonar(uint8_t pin, SONAR_INPUT_t input, SONAR_FILTER_t filter = NONE, uint8_t sample_size = 0);

	//simple api
	float getDistance();
	uint8_t getSampleSize(){
		return sample_size;
	};
	
	//advanced api
	void readSample();
	float* getSample(){
		return sample;
	};
  float* getSampleSorted(){
    return sample_sorted;
  };
	float getSampleMedian();
	float getSampleMode(bool highest = true);
	float getSampleBest();
			 
	// getters
	uint8_t getPin(){
		return pin;
	}
	SONAR_INPUT_t getInput(){
		return input;
	}
	SONAR_FILTER_t getFilter(){
		return filter;
	}

	//Printer
	void printArray(float* array, uint8_t array_size);
	
private:
	// config variables
	uint8_t pin;
	SONAR_INPUT_t input;
	SONAR_FILTER_t filter;
  uint8_t sample_size;
		
	// core
	void init();
  void trigger();
	float readSensor();
  void copySample();
	void sortSample();

  float sample[10];
  float sample_sorted[10];
		
};

void printArray();
void debug_front();
void debug_right();

#endif
