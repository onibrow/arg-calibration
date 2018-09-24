int PUL = 7; // PULSE PIN
int DIR = 6; // DIRECTION PIN
int ENA = 5; // ENABLE PIN
int BASE_TICKS = 10;
int request;

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
        Serial.println("Biggest Left");
        drive(BASE_TICKS * 10, HIGH);
        break;
      case 115:
        Serial.println("Big Left");
        drive(BASE_TICKS * 5, HIGH);
        break;
      case 100:
        Serial.println("Left");
        drive(BASE_TICKS, HIGH);
        break;
      case 106:
        Serial.println("Right");
        drive(BASE_TICKS, LOW);
        break;
      case 107:
        Serial.println("Big Right");
        drive(BASE_TICKS * 5, LOW);
        break;
      case 108:
        Serial.println("Biggest Right");
        drive(BASE_TICKS * 10, LOW);
        break;
      default:
        Serial.println("Invalid Input");
    }
  }
}

void drive(int ticks, int forwards) {
  Serial.println("Driving");
  digitalWrite(DIR, forwards);
  for (int i = 0; i, i < ticks; i++) {
    digitalWrite(PUL, HIGH);
    delayMicroseconds(50);
    digitalWrite(PUL, LOW);
    delayMicroseconds(50);
  }
  Serial.println("Ready");
}

