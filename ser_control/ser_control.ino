int PUL = 7; // PULSE PIN
int DIR = 6; // DIRECTION PIN
int ENA = 5; // ENABLE PIN

int BASE_TICKS = 5000;
int DELAY = 25; //fast enough

unsigned long time_out = 500;
char ESTOP = 'E';
bool DEBUG = true;
String inString = "";

double displacement;
double velocity;
int cycles;

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
  if (DEBUG && built) parse_input();
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

bool parse_input() {
  int p = inString.indexOf('P');
  int v = inString.indexOf('V', p);
  int c = inString.indexOf('C', v);
  int m = inString.indexOf('M', m);
  if (p == -1 or v == -1 or c == -1 or m == -1) return false;
  displacement = (inString.substring(p+1,v)).toDouble();
  velocity     = (inString.substring(v+1,c)).toDouble();
  cycles       = (inString.substring(c+1,m)).toInt();
  if (DEBUG) {
    Serial.print("Displacement: ");
    Serial.println(displacement);
    Serial.print("Velocity ");
    Serial.println(velocity);
    Serial.print("Cycles: ");
    Serial.println(cycles);
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
