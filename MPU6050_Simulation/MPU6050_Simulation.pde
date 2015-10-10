import processing.serial.*;

Serial  port;
int     lf = 10;       //ASCII linefeed
String  inString;      //String for testing serial communication

float   angle_x; 
float   angle_y;
float   angle_z;

 
void setup()  { 

  size(500, 500, P3D);
  noStroke();
 
  port = new Serial(this, "COM20", 115200);
  port.clear();
  port.bufferUntil(lf);
} 

void draw_rect_rainbow() {
  scale(90);
  beginShape(QUADS);

  fill(0, 1, 1); vertex(-1,  1.5,  0.25);
  fill(1, 1, 1); vertex( 1,  1.5,  0.25);
  fill(1, 0, 1); vertex( 1, -1.5,  0.25);
  fill(0, 0, 1); vertex(-1, -1.5,  0.25);

  fill(1, 1, 1); vertex( 1,  1.5,  0.25);
  fill(1, 1, 0); vertex( 1,  1.5, -0.25);
  fill(1, 0, 0); vertex( 1, -1.5, -0.25);
  fill(1, 0, 1); vertex( 1, -1.5,  0.25);

  fill(1, 1, 0); vertex( 1,  1.5, -0.25);
  fill(0, 1, 0); vertex(-1,  1.5, -0.25);
  fill(0, 0, 0); vertex(-1, -1.5, -0.25);
  fill(1, 0, 0); vertex( 1, -1.5, -0.25);

  fill(0, 1, 0); vertex(-1,  1.5, -0.25);
  fill(0, 1, 1); vertex(-1,  1.5,  0.25);
  fill(0, 0, 1); vertex(-1, -1.5,  0.25);
  fill(0, 0, 0); vertex(-1, -1.5, -0.25);

  fill(0, 1, 0); vertex(-1,  1.5, -0.25);
  fill(1, 1, 0); vertex( 1,  1.5, -0.25);
  fill(1, 1, 1); vertex( 1,  1.5,  0.25);
  fill(0, 1, 1); vertex(-1,  1.5,  0.25);

  fill(0, 0, 0); vertex(-1, -1.5, -0.25);
  fill(1, 0, 0); vertex( 1, -1.5, -0.25);
  fill(1, 0, 1); vertex( 1, -1.5,  0.25);
  fill(0, 0, 1); vertex(-1, -1.5,  0.25);

  endShape();
  
  
}

void draw_rect() {
  scale(90);
  beginShape(QUADS);
  
  fill(255, 0, 0);
  vertex(-1,  1.5,  0.25);
  vertex( 1,  1.5,  0.25);
  vertex( 1, -1.5,  0.25);
  vertex(-1, -1.5,  0.25);
  
  fill(0, 255, 0);
  vertex( 1,  1.5,  0.25);
  vertex( 1,  1.5, -0.25);
  vertex( 1, -1.5, -0.25);
  vertex( 1, -1.5,  0.25);

  fill(0, 0, 255);
  vertex( 1,  1.5, -0.25);
  vertex(-1,  1.5, -0.25);
  vertex(-1, -1.5, -0.25);
  vertex( 1, -1.5, -0.25);

  fill(255, 255, 0);
  vertex(-1,  1.5, -0.25);
  vertex(-1,  1.5,  0.25);
  vertex(-1, -1.5,  0.25);
  vertex(-1, -1.5, -0.25);

  fill(255, 0, 255);
  vertex(-1,  1.5, -0.25);
  vertex( 1,  1.5, -0.25);
  vertex( 1,  1.5,  0.25);
  vertex(-1,  1.5,  0.25);

  fill(0, 255, 255);
  vertex(-1, -1.5, -0.25);
  vertex( 1, -1.5, -0.25);
  vertex( 1, -1.5,  0.25);
  vertex(-1, -1.5,  0.25);

  endShape();
}

void draw()  { 
  
  background(0);
  lights();
    
  // Tweak the view of the rectangles
  int x_rotation = 90;
  pushMatrix();
  translate(width/2, height/2, -50);
  
  rotateX(radians(angle_x - x_rotation));
  rotateY(radians(angle_y));
  rotateZ(radians(angle_z));
  
  draw_rect();
  popMatrix();
  
  textSize(15);
  String angles = "Angle_X: " + (int) angle_x + " Angle_Y: " + (int) angle_y + " Angle_Z: " + (int) angle_z ;
  fill(255, 0, 0);
  text(angles, 20, 20); 
} 

void serialEvent(Serial p) {

  inString = (port.readString());
  
  try {
    // Parse the data
    String[] data = split(inString, ':');
    
    if (data[0].equals("#")) {
        angle_z = -float(data[1]);
        angle_x = float(data[2]);
        angle_y = float(data[3]);    
    }
    
  } catch (Exception e) {
      println("Caught Exception");
  }
}