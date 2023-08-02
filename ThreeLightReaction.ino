const int blueLight = 13;
const int greenLight = 2;

const int blueButton = 12;
const int greenButton = 3;

// the setup function runs once when you press reset or power the board
void setup() {
  /* SETUP PINS*/
  pinMode(blueLight, OUTPUT);
  pinMode(greenLight, OUTPUT);

  pinMode(blueButton, INPUT);
  pinMode(greenButton, INPUT);
  // pinMode(LED_BUILTIN, OUTPUT); // for debugging
  /* SETUP SERIEL OUT */
  Serial.begin(9600);
  Serial.print("***,***"); // this will be picked up in Python script
 
  
}

/**
* Returns reaction time, acts as glue for more functions
*@param choice int, 1 - green, 2 - blue, 3 - red and blue
*@return int number of milliseconds it took for reaction
*/
int getReactionTime(int choice){
  if(!(choice == 1 || choice == 2 || choice == 3)){ // guard clause for input
    Serial.print("Error! Bad input on getReactionTime. Expected int [1-3], instead recieved: ");
    Serial.println(choice);
    return 0;
  }
  waitTillButtonClears(blueButton, greenButton); // loops until both buttons dont give signal (user takes hand off button)
  int reactionTime;
  switch(choice){
    case 1:
      reactionTime = getSingleLightTime(greenLight, greenButton, blueButton);
      break;
    case 2:
      reactionTime = getSingleLightTime(blueLight, blueButton, blueButton);
      break;
    case 3:
      reactionTime = getTwoLightsTime(greenLight, blueLight, greenButton, blueButton);
      break;
  }
  return reactionTime;
}


/**
* Loops until both buttons are returning a value of 0
* @param buttonA
* @param buttonB
*/
void waitTillButtonClears(int buttonA, int buttonB){
  bool buttonsCleared = false;
  int signalA, signalB; 
  while (!buttonsCleared){
    signalA = digitalRead(buttonA);
    signalB = digitalRead(buttonB);
    buttonsCleared = (signalA == 0 && signalB == 0);
  }
}

/**
* Helper function for getReactionTime
* @param light - the digital output for the light
* @param button - the digital input for the button
*/
int getSingleLightTime(int light, int correctButton, int wrongButton){
  digitalWrite(light, HIGH);
  const long int currentTime = millis();
  long int timeOfClick;
  bool buttonClicked = false; // we won't exit loop till button pressed
  while(!buttonClicked){
    int correctButtonState = digitalRead(correctButton);
    int wrongButtonState = digitalRead(wrongButton);
    bool buttonState = (correctButtonState == 1 && wrongButtonState == 0); // button press valid iff right button pressed, wrong button not

    if (buttonState){
      timeOfClick = millis();
      buttonClicked = true;
    }
  }
  digitalWrite(light, LOW);
  return timeOfClick - currentTime;
}

/**
* Helper function for getReactionTime, handles 
* 2 lights
* @param lightA - the digital output for lightA
* @param lightB - the digital output for lightB
* @param buttonA - the digital output for buttonA
* @param buttonB - the digital output for buttonB
* @returns int - number of elapsed milliseconds
*/
int getTwoLightsTime(int lightA, int lightB, int buttonA, int buttonB){
  digitalWrite(lightA, HIGH);
  digitalWrite(lightB, HIGH);
  const long int currentTime = millis();
  long int timeOfClick;
  bool buttonClicked = false; // we won't exit loop till button pressed
  while(!buttonClicked){
    int buttonAState = digitalRead(buttonA);
    int buttonBState = digitalRead(buttonB);
    if (buttonAState == 1 && buttonBState == 1){
      timeOfClick = millis();
      buttonClicked = true;
    }
  }
  digitalWrite(lightA, LOW);
  digitalWrite(lightB, LOW);
  return timeOfClick - currentTime;

}

// the loop function runs over and over again forever
void loop() {
  int choice = random(1,4);
  int reactionTime = getReactionTime(choice);
  int delayTime = random(2000);
  Serial.print(choice);
  Serial.print(",");
  Serial.print(delayTime);
  Serial.print(",");
  Serial.println(reactionTime);
  delay(delayTime);
} 
