int PUL = 7; // PULSE PIN
int DIR = 6; // DIRECTION PIN
int ENA = 5; // ENABLE PIN

unsigned long time_out = 500;
char ESTOP = 'E';
bool DEBUG = true;
double TICK_MM = 625;
int MAX_SPEED = 16;
String inString = "";

double displacement;
double velocity;
int cycles;
bool running = false;
bool one_way = false;

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
  if (build_input() && parse_input() && !running) drive();
  delay(10);
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
      //      if (DEBUG) Serial.println("TIME OUT");
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
  displacement = (inString.substring(p + 1, v)).toDouble() / 10;
  velocity     = min(MAX_SPEED, (inString.substring(v + 1, c)).toDouble());
  cycles       = (inString.substring(c + 1, m)).toInt();
  if (DEBUG) {
    Serial.print("Displacement: ");
    Serial.println(displacement);
    Serial.print("Velocity ");
    Serial.println(velocity);
    Serial.print("Cycles: ");
    Serial.println(cycles);
  }
  return true;
}

void drive() {
  digitalWrite(ENA, LOW);
  running = true;

  // One Way Adjustment
  if (cycles == 0) {one_way = true; cycles = 1;}
  else one_way = false;
  
  // Math
  bool dir = displacement > 0;

  double tick_interval = 1e6 / velocity / TICK_MM * 39/40;
  
  double ticks = abs(TICK_MM * displacement);

  if (DEBUG) {
    Serial.print("Ticks: ");
    Serial.println(ticks);
    Serial.print("Tick Interval: ");
    Serial.println(tick_interval);
    Serial.print("Cycles: ");
    Serial.println(cycles);
  }

  if (DEBUG) {Serial.print("Begin Driving @ "); Serial.println(millis());}

  for (int i = 0; i < cycles; i++) {
    // Destination Trip   
    digitalWrite(DIR, dir);
    for (double j = 0; j < ticks; j++) {
      // ESTOP
      if (Serial.available() > 0) {
        if (Serial.read() == ESTOP) {
          if (DEBUG) Serial.println("***ESTOP***");
          running = false;
          return;
        }
      }
      // Pulse
      digitalWrite(PUL, HIGH);
      delayMicroseconds(tick_interval);
      digitalWrite(PUL, LOW);
      delayMicroseconds(tick_interval);
    }
    if (!one_way) {
      // Return Trip
      digitalWrite(DIR, !dir);
      for (double j = 0; j < ticks; j++) {
        // ESTOP
        if (Serial.available() > 0) {
          if (Serial.read() == ESTOP) {
            if (DEBUG) Serial.println("***ESTOP***");
            running = false;
            return;
          }
        }
        // Pulse
        digitalWrite(PUL, HIGH);
        delayMicroseconds(tick_interval);
        digitalWrite(PUL, LOW);
        delayMicroseconds(tick_interval);
      }
    }
  }

  // Flush Incoming Buffer
  while (Serial.available() > 0) {
    char t = Serial.read();
  }

  if (DEBUG) {Serial.print("End Driving @ "); Serial.println(millis());}
  digitalWrite(ENA, HIGH);
  running = false;
}
