int PUL = 7; // PULSE PIN
int DIR = 6; // DIRECTION PIN
int ENA = 5; // ENABLE PIN
int BASE_TICKS = 5000;
int DELAY = 25; //fast enough
int request;
char ESTOP = 'E';
bool DEBUG = true;;

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port
  }
  pinMode (PUL, OUTPUT);
  pinMode (DIR, OUTPUT);
  pinMode (ENA, OUTPUT);
}

void loop() {
  if (Serial.available() > 0) {
    request = Serial.read();
    switch (request) {
      case 97:
        if (DEBUG) Serial.println("Biggest Left");
        drive(BASE_TICKS * 10, HIGH);
        break;
      case 115:
        if (DEBUG) Serial.println("Big Left");
        drive(BASE_TICKS * 5, HIGH);
        break;
      case 100:
        if (DEBUG) Serial.println("Left");
        drive(BASE_TICKS, HIGH);
        break;
      case 106:
        if (DEBUG) Serial.println("Right");
        drive(BASE_TICKS, LOW);
        break;
      case 107:
        if (DEBUG) Serial.println("Big Right");
        drive(BASE_TICKS * 5, LOW);
        break;
      case 108:
        if (DEBUG) Serial.println("Biggest Right");
        drive(BASE_TICKS * 10, LOW);
        break;
      default:
        if (DEBUG) Serial.println("Invalid Input");
    }
  }
}

void drive(int ticks, int forwards) {
  digitalWrite(ENA, LOW);
  if (DEBUG) Serial.println("Driving");
  digitalWrite(DIR, forwards);
  for (int i = 0; i, i < ticks; i++) {
    // ESTOP
    if (Serial.available() > 0) {
      if (Serial.read() == ESTOP) return;
    }
    // Pulse
    digitalWrite(PUL, HIGH);
    delayMicroseconds(DELAY);
    digitalWrite(PUL, LOW);
    delayMicroseconds(DELAY);
  }
  // Flush Incoming Buffer
  while (Serial.available() > 0) {
    char t = Serial.read();
  }
  if (DEBUG) Serial.println("Ready");
  digitalWrite(ENA, HIGH);
}

