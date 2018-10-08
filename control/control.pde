import g4p_controls.*;

import processing.serial.*;

Serial serial_port = null;        // the serial port

// serial port buttons
Button btn_serial_up;              // move up through the serial port list
Button btn_serial_dn;              // move down through the serial port list
Button btn_serial_connect;         // connect to the selected serial port
Button btn_serial_disconnect;      // disconnect from the serial port
Button btn_serial_list_refresh;    // refresh the serial port list
String serial_list;                // list of serial ports
int serial_list_index = 0;         // currently selected serial port 
int num_serial_ports = 0;          // number of serial ports in the list
int com_y = 30;

String delta_x;
String velocity_x;
String cycles;

boolean DEBUG = true;

boolean active;
boolean ready_x;
boolean ready_dx;
boolean ready_hz;
boolean valid;
boolean connected = false;

void setup() {
  size(315, 260);

  btn_serial_up = new Button("^", 140, com_y+10, 40, 20);
  btn_serial_dn = new Button("v", 140, com_y+50, 40, 20);
  btn_serial_connect = new Button("Connect", 190, com_y+10, 100, 25);
  btn_serial_disconnect = new Button("Disconnect", 190, com_y+45, 100, 25);
  btn_serial_list_refresh = new Button("Refresh", 190, com_y+80, 100, 25);

  serial_list = Serial.list()[serial_list_index];
  num_serial_ports = Serial.list().length;
  // GUI BUILDER
  createGUI();
}

void mousePressed() {
  // up button clicked
  if (btn_serial_up.MouseIsOver()) {
    if (serial_list_index > 0) {
      // move one position up in the list of serial ports
      serial_list_index--;
      serial_list = Serial.list()[serial_list_index];
    }
  }
  // down button clicked
  if (btn_serial_dn.MouseIsOver()) {
    if (serial_list_index < (num_serial_ports - 1)) {
      // move one position down in the list of serial ports
      serial_list_index++;
      serial_list = Serial.list()[serial_list_index];
    }
  }
  // Connect button clicked
  if (btn_serial_connect.MouseIsOver()) {
    if (serial_port == null) {
      // connect to the selected serial port
      label5.setText("Connected");
      serial_port = new Serial(this, Serial.list()[serial_list_index], 9600);
      connected = true;
    }
  }
  // Disconnect button clicked
  if (btn_serial_disconnect.MouseIsOver()) {
    if (serial_port != null) {
      // disconnect from the serial port
      serial_port.stop();
      serial_port = null;
      label5.setText("Disconnected");
      connected = false;
    }
  }
  // Refresh button clicked
  if (btn_serial_list_refresh.MouseIsOver()) {
    // get the serial port list and length of the list
    serial_list = Serial.list()[serial_list_index];
    num_serial_ports = Serial.list().length;
  }
}

void draw() {
  btn_serial_up.Draw();
  btn_serial_dn.Draw();
  btn_serial_connect.Draw();
  btn_serial_disconnect.Draw();
  btn_serial_list_refresh.Draw();
  DrawTextBox("Select Port", serial_list, 10, com_y+10, 120, 60);
}

void DrawTextBox(String title, String str, int x, int y, int w, int h)
{
  fill(255);
  rect(x, y, w, h);
  fill(0);
  textAlign(LEFT);
  textSize(14);
  text(title, x + 10, y + 10, w - 20, 20);
  textSize(12);  
  text(str, x + 10, y + 40, w - 20, h - 10);
}

class Button {
  String label;
  float x;  float y;  float w;  float h;
  Button(String labelB, float xpos, float ypos, float widthB, float heightB) {
    label = labelB;
    x = xpos;
    y = ypos;
    w = widthB;
    h = heightB;
  }
  void Draw() {
    fill(218);
    stroke(141);
    rect(x, y, w, h, 10);
    textAlign(CENTER, CENTER);
    fill(0);
    text(label, x + (w / 2), y + (h / 2));
  }
  boolean MouseIsOver() {
    if (mouseX > x && mouseX < (x + w) && mouseY > y && mouseY < (y + h)) {
      return true;
    }
    return false;
  }
}

boolean send_data() {
  serial_port.clear();
  while (serial_port.available() == 0);
  char c = serial_port.readChar();
  active = c == 'R';
  if (!connected || active || !ready_x || !ready_dx || !ready_hz) return false;
  serial_port.write("P" + delta_x + "V" + velocity_x + "C" + cycles);
  if (DEBUG) println("P" + delta_x + "V" + velocity_x + "C" + cycles);
  return true;
}

void estop() {
  serial_port.write("EEEEEEEE"); 
}
