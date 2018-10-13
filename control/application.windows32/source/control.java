import processing.core.*; 
import processing.data.*; 
import processing.event.*; 
import processing.opengl.*; 

import g4p_controls.*; 
import processing.serial.*; 

import java.util.HashMap; 
import java.util.ArrayList; 
import java.io.File; 
import java.io.BufferedReader; 
import java.io.PrintWriter; 
import java.io.InputStream; 
import java.io.OutputStream; 
import java.io.IOException; 

public class control extends PApplet {





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

public void setup() {
  

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

public void mousePressed() {
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

public void draw() {
  btn_serial_up.Draw();
  btn_serial_dn.Draw();
  btn_serial_connect.Draw();
  btn_serial_disconnect.Draw();
  btn_serial_list_refresh.Draw();
  DrawTextBox("Select Port", serial_list, 10, com_y+10, 120, 60);
}

public void DrawTextBox(String title, String str, int x, int y, int w, int h)
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
  public void Draw() {
    fill(218);
    stroke(141);
    rect(x, y, w, h, 10);
    textAlign(CENTER, CENTER);
    fill(0);
    text(label, x + (w / 2), y + (h / 2));
  }
  public boolean MouseIsOver() {
    if (mouseX > x && mouseX < (x + w) && mouseY > y && mouseY < (y + h)) {
      return true;
    }
    return false;
  }
}

public boolean send_data() {
  if (!connected) return false;
  serial_port.clear();
  //while (serial_port.available() == 0);
  //char c = serial_port.readChar();
  //active = c == 'R';
  active = false;
  println(connected + " " + active + " " + ready_x + " " + ready_dx + " " + ready_hz);
  if (!connected || active || !ready_x || !ready_dx || !ready_hz) return false;
  serial_port.write("P" + delta_x + "V" + velocity_x + "C" + cycles + "M");
  if (DEBUG) {
    println("Writing: P" + delta_x + "V" + velocity_x + "C" + cycles + "M");
    println("Echoed: " + serial_port.readString());
  }
  return true;
}

public void estop() {
  serial_port.write("EEEEEEEE"); 
}
/* =========================================================
 * ====                   WARNING                        ===
 * =========================================================
 * The code in this tab has been generated from the GUI form
 * designer and care should be taken when editing this file.
 * Only add/edit code inside the event handlers i.e. only
 * use lines between the matching comment tags. e.g.

 void myBtnEvents(GButton button) { //_CODE_:button1:12356:
     // It is safe to enter your event code here  
 } //_CODE_:button1:12356:
 
 * Do not rename this tab!
 * =========================================================
 */

public void start_test(GButton source, GEvent event) { //_CODE_:start_button:264212:
  // START BUTTON
  boolean ack = send_data();
  if (DEBUG) println("Sent data is   " + ack);
  active = true;
} //_CODE_:start_button:264212:

public void stop_test(GButton source, GEvent event) { //_CODE_:stop_button:289303:
  // STOP BUTTON
  if (DEBUG) println("*** ESTOP ***");
  estop();
  active = false;
} //_CODE_:stop_button:289303:

public void position(GTextArea source, GEvent event) { //_CODE_:x_input:412721:
  try {
    delta_x = x_input.getText(0).trim();
    Double.valueOf(delta_x);
    ready_x = true;
  } 
  catch (NumberFormatException nfe) {
    if (DEBUG) println("x_input format invalid");
    ready_x = false;
  }
} //_CODE_:x_input:412721:

public void velocity_x(GTextArea source, GEvent event) { //_CODE_:vel_input:368130:
  try {
    velocity_x = vel_input.getText(0).trim();
    Double.valueOf(velocity_x);
    ready_dx = true;
  }
  catch (NumberFormatException nfe) {
    if (DEBUG) println("velocity_x format invalid");
    ready_dx = false;
  }
} //_CODE_:vel_input:368130:

public void cycle_x(GTextArea source, GEvent event) { //_CODE_:cycle_input:939489:
  try {
    cycles = cycle_input.getText(0).trim();
    Double.valueOf(cycles);
    ready_hz = true;
  }
  catch (NumberFormatException nfe) {
    if (DEBUG) println("velocity_x format invalid");
    ready_hz = false;
  }
} //_CODE_:cycle_input:939489:



// Create all the GUI controls. 
// autogenerated do not edit
public void createGUI(){
  G4P.messagesEnabled(false);
  G4P.setGlobalColorScheme(GCScheme.BLUE_SCHEME);
  G4P.setCursor(ARROW);
  surface.setTitle("Sketch Window");
  start_button = new GButton(this, 10, 210, 140, 40);
  start_button.setText("Start");
  start_button.setTextBold();
  start_button.setLocalColorScheme(GCScheme.GREEN_SCHEME);
  start_button.addEventHandler(this, "start_test");
  stop_button = new GButton(this, 160, 210, 140, 40);
  stop_button.setText("Stop");
  stop_button.setTextBold();
  stop_button.setLocalColorScheme(GCScheme.RED_SCHEME);
  stop_button.addEventHandler(this, "stop_test");
  x_input = new GTextArea(this, 10, 160, 90, 40, G4P.SCROLLBARS_NONE);
  x_input.setOpaque(true);
  x_input.addEventHandler(this, "position");
  vel_input = new GTextArea(this, 110, 160, 90, 40, G4P.SCROLLBARS_NONE);
  vel_input.setOpaque(true);
  vel_input.addEventHandler(this, "velocity_x");
  cycle_input = new GTextArea(this, 210, 160, 90, 40, G4P.SCROLLBARS_NONE);
  cycle_input.setOpaque(true);
  cycle_input.addEventHandler(this, "cycle_x");
  label1 = new GLabel(this, 10, 140, 90, 20);
  label1.setTextAlign(GAlign.CENTER, GAlign.MIDDLE);
  label1.setText("Δx[mm]");
  label1.setOpaque(false);
  label2 = new GLabel(this, 110, 140, 90, 20);
  label2.setTextAlign(GAlign.CENTER, GAlign.MIDDLE);
  label2.setText("Speed[mm/s]");
  label2.setOpaque(false);
  label3 = new GLabel(this, 210, 140, 90, 20);
  label3.setTextAlign(GAlign.CENTER, GAlign.MIDDLE);
  label3.setText("Num Cycles");
  label3.setOpaque(false);
  label4 = new GLabel(this, 30, 10, 250, 20);
  label4.setTextAlign(GAlign.CENTER, GAlign.MIDDLE);
  label4.setText("Arias Research Group - Force Calibration");
  label4.setLocalColorScheme(GCScheme.YELLOW_SCHEME);
  label4.setOpaque(true);
  label5 = new GLabel(this, 10, 110, 90, 20);
  label5.setTextAlign(GAlign.CENTER, GAlign.MIDDLE);
  label5.setText("Disconnected");
  label5.setOpaque(true);
}

// Variable declarations 
// autogenerated do not edit
GButton start_button; 
GButton stop_button; 
GTextArea x_input; 
GTextArea vel_input; 
GTextArea cycle_input; 
GLabel label1; 
GLabel label2; 
GLabel label3; 
GLabel label4; 
GLabel label5; 

  public void settings() {  size(315, 260); }
  static public void main(String[] passedArgs) {
    String[] appletArgs = new String[] { "control" };
    if (passedArgs != null) {
      PApplet.main(concat(appletArgs, passedArgs));
    } else {
      PApplet.main(appletArgs);
    }
  }
}
