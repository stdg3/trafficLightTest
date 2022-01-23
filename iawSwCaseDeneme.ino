int state = 1;
volatile uint32_t lastRead = 0;
volatile uint32_t debounce;
int interval = 10;
int yellowInterval = 3;

bool doPause = false;
bool from5to3 = false;

int redLed = 4;
int yellowLed = 5;
int greenLed = 6;

void setup() {

  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  attachInterrupt(0, GreenInterrupt, CHANGE);
  attachInterrupt(1, RedInterrupt, CHANGE);

  pinMode(redLed, OUTPUT);
  pinMode(yellowLed, OUTPUT);
  pinMode(greenLed, OUTPUT);
  pinMode(13, OUTPUT);
}

void GreenInterrupt() {
  if (millis() - debounce >= 100 && digitalRead(2)) {
    debounce = millis();
    if (state == 1){
        state=2;
      }
    else if (state == 3){
      state = 4;
      lastRead = millis()/1000;
      doPause = true; 
      }    
  }
}


void RedInterrupt() {
    if (millis() - debounce >= 100 && digitalRead(3)) {
    debounce = millis();
    switch(state){
      case 2:
        state=3;
        lastRead = millis()/1000;
        break;
      case 3:
        doPause = true;
        lastRead = millis()/1000;
        state=1;
        break;
      case 4:
        from5to3 = true;
        state=5;
        break;
      case 5:
        state=3;
        lastRead = millis()/1000;
        break;
      }    
  }
}

void RedON(){
  if (millis()/1000 - lastRead <= interval){
    digitalWrite(redLed, HIGH);
    digitalWrite(yellowLed, LOW);
    digitalWrite(greenLed, LOW);    
    }
  else{
    lastRead += interval;
    state = 2;
    lastRead = millis()/1000; 
    }
  }

void YellowON(){
    if (millis()/1000 - lastRead <= yellowInterval){
      digitalWrite(redLed, LOW);
      digitalWrite(yellowLed, HIGH);
      digitalWrite(greenLed, LOW);   
    }
    else{
      state = 4;
      lastRead = millis()/1000;   
    }
  }

void GreenON(){
  if (millis()/1000 - lastRead <= interval){
    digitalWrite(redLed, LOW);
    digitalWrite(yellowLed, LOW);
    digitalWrite(greenLed, HIGH);    
    }
  else{
    lastRead += interval;
    state = 5;    
    }
  }

void RedBlink(){
  digitalWrite(redLed, LOW);      
    for (int i = 0; i <3; i++){
    digitalWrite(redLed, LOW);
    delay(1000);
    digitalWrite(redLed, HIGH);
    delay(1000);
    }
    state = 3;
    lastRead = millis()/1000;
}

void GreenBlink(){
  digitalWrite(greenLed, LOW);
  for (int i = 0; i <3; i++){
    digitalWrite(greenLed, LOW);
    delay(1000);
    digitalWrite(greenLed, HIGH);
    delay(1000);
    }
    if (from5to3){
      state = 3;
      from5to3 = false;
      }
    else{
      state = 1;
      }
    lastRead = millis()/1000;
  }

void loop() {  
  switch(state){
    case 1:
    if (doPause){
      doPause = false;
      delay(500);
      RedON();
      }
      RedON();
      break;
      
    case 2:
      RedBlink();
      break;
      
    case 3:
      YellowON();
      break;
      
    case 4:
      if (doPause){
        doPause = false;
        delay(500);
      GreenON();
      }
      GreenON();      
      break;
      
    case 5:
      GreenBlink();
      break;
    }
}
