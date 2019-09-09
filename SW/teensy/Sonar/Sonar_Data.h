/*____________________________________________
______________Things to consider______________
-                                            -
- ~ sequential triggering for no cross-talk  -
- ~ serial communication                     -
- ~ when the object is removed from path,    -
-   the distance should increase gradually   -
- ~ bundle the sensor reads into one         -
-   function in sonar.cpp and move the       -
-   class object delcarations (sonar_data.h) -
-   include from sonar.ino to sonar.h. This  -
-   is also where the logic for which sensor -
-   to choose while turning went             -
- ~ ACC_Func_Handler is called every 15ms    -
-   and it gets ultrasonic_distances which   -
-   cannot be called that fast               -
____________________________________________*/


//Define pins
const uint8_t SONAR_LEFT = 0;
const uint8_t SONAR_RIGHT = 8;
const uint8_t SONAR_FRONT = 7;
const uint8_t SONAR_TRIGGER = 11;

//Define sample size for filtering
//Must be an odd number
const uint8_t SONAR_BUFFER = 5;

//Ultrasonic Objects
//Sonar leftSonarPW(SONAR_LEFT, Sonar::PW, Sonar::BEST, SONAR_BUFFER);
Sonar rightSonarPW(SONAR_RIGHT, Sonar::PW, Sonar::BEST, SONAR_BUFFER);
Sonar frontSonarPW(SONAR_FRONT, Sonar::PW, Sonar::BEST, SONAR_BUFFER);
