int PUL = 7; // PULSE PIN
int DIR = 6; // DIRECTION PIN
int ENA = 5; // ENABLE PIN

int BASE_TICKS = 5000;
int DELAY = 25; //fast enough

unsigned long time_out = 500;
char ESTOP = 'E';
bool DEBUG = true;
String inString = "";

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
  bool built = build_input();
  if (DEBUG && built) Serial.println(inString);
  delay(500);
}

bool build_input() {
  bool end = false;
  unsigned long t = millis();
  inString = "";
  while (true) {
    if (Serial.available() > 0) {
      int inChar = Serial.read();
      inString += (char)inChar;
      if (inChar == 'M') {
        end = true;
        return true;
      }
    }
    if (millis() > t + time_out) {
      if (DEBUG) Serial.println("TIME OUT");
      return false;
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

