#include <Servo.h> // Include the Servo library for controlling the servo motor

Servo servo; // Create a servo object

// Pin definitions for motor and sensors
#define servopin 4
#define motorRplus 6 // Right motor forward
#define motorRminus 7 // Right motor backward
#define motorLplus 9 // Left motor forward
#define motorLminus 8 // Left motor backward
#define trigpin 10 // Ultrasonic sensor trigger pin
#define echopin 11 // Ultrasonic sensor echo pin
#define irleft 3 // Left IR sensor pin
#define irright 2 // Right IR sensor pin

// Global variables
int triglow = 2; // LOW pulse duration for ultrasonic sensor
int trighigh = 10; // HIGH pulse duration for ultrasonic sensor
int choice = 1; // Placeholder for future use
int wait = 1000; // Delay in milliseconds for various operations
float time; // Time taken for sound wave travel
float dleft; // Distance measured to the left
float dright; // Distance measured to the right
float obstacle; // Distance to the obstacle in front

void setup()
{
  // Set pin modes
  pinMode(servopin, OUTPUT);
  pinMode(motorRplus, OUTPUT);
  pinMode(motorRminus, OUTPUT);
  pinMode(motorLplus, OUTPUT);
  pinMode(motorLminus, OUTPUT);
  pinMode(trigpin, OUTPUT);
  pinMode(echopin, INPUT);
  pinMode(irleft, INPUT);
  pinMode(irright, INPUT);

  // Initialize servo
  servo.attach(servopin);
  servo.write(90); // Set servo to center position

  // Begin serial communication for debugging
  Serial.begin(9600);
}

void loop()
{
  // Measure distance to the obstacle in front
  obstacle = distance();
  Serial.print("Obstacle distance: ");
  Serial.println(obstacle);

  // If an obstacle is detected within 50 cm
  if (obstacle <= 50)
  {
    stop(); // Stop the robot
    delay(wait);
    backward(); // Move backward
    delay(wait);
  }

  // Scan the right side
  servo.write(25); // Rotate servo to the right
  dright = distance(); // Measure distance
  Serial.print("Right distance: ");
  Serial.println(dright);

  delay(wait);

  // Scan the left side
  servo.write(155); // Rotate servo to the left
  dleft = distance(); // Measure distance
  Serial.print("Left distance: ");
  Serial.println(dleft);

  delay(wait);

  // Reset servo to center
  servo.write(90);
  delay(wait);

  // Decide movement based on distances and sensor input
  if (dleft > dright)
  {
    Serial.println("Turning left");
    turnleft(); // Turn left if the left side is clearer
    delay(wait);
  }
  else if (dleft < dright)
  {
    Serial.println("Turning right");
    turnright(); // Turn right if the right side is clearer
    delay(wait);
  }
  else if (!(digitalRead(irleft)) && !(digitalRead(irright)))
  {
    Serial.println("Moving forward");
    forward(); // Move forward if no IR sensor detects an edge
    delay(wait);
  }
  else if ((digitalRead(irleft)) && !(digitalRead(irright)))
  {
    turnright(); // Turn right if only the left IR sensor detects an edge
    delay(wait);
  }
  else if (!(digitalRead(irleft)) && (digitalRead(irright)))
  {
    turnleft(); // Turn left if only the right IR sensor detects an edge
    delay(wait);
  }
}

// Function to move forward
void forward()
{
  digitalWrite(motorRplus, HIGH);
  digitalWrite(motorRminus, LOW);
  digitalWrite(motorLplus, HIGH);
  digitalWrite(motorLminus, LOW);
}

// Function to turn left
void turnleft()
{
  digitalWrite(motorRplus, HIGH);
  digitalWrite(motorRminus, LOW);
  digitalWrite(motorLplus, LOW);
  digitalWrite(motorLminus, LOW);  
}

// Function to turn right
void turnright()
{
  digitalWrite(motorRplus, LOW);
  digitalWrite(motorRminus, LOW);
  digitalWrite(motorLplus, HIGH);
  digitalWrite(motorLminus, LOW);  
}

// Function to move backward
void backward()
{
  digitalWrite(motorRplus, LOW);
  digitalWrite(motorRminus, HIGH);
  digitalWrite(motorLplus, LOW);
  digitalWrite(motorLminus, HIGH);  
}

// Function to stop the robot
void stop()
{
  digitalWrite(motorRplus, LOW);
  digitalWrite(motorRminus, LOW);
  digitalWrite(motorLplus, LOW);
  digitalWrite(motorLminus, LOW);  
}

// Function to measure the duration of the ultrasonic pulse
float duration()
{
  digitalWrite(trigpin, LOW);
  delay(triglow);
  digitalWrite(trigpin, HIGH);
  delay(trighigh);
  digitalWrite(trigpin, LOW);
  return pulseIn(echopin, HIGH) / 2.00; // Return half the pulse time
}

// Function to calculate distance from the duration
float distance()
{
  time = duration();
  return time * 0.034; // Convert time to distance in cm
}
