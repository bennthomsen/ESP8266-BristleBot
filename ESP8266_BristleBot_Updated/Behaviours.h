

void stopMotors(void) {
  analogWrite(RIGHT,0);
  analogWrite(LEFT,0);
 Sprintln("Stop Motors");
}

void setRightMotorValue(int value) {
  drive.right.value = value;
  analogWrite(RIGHT,value);
  Sprint( "Set Right Motor Power: ");
  Sprintln(value);
}

void setLeftMotorValue(int value) {
  left.value(value);
  Sprint( "Set Left Motor Power: ");
  Sprintln(value);
}


void forward(void) {
  drive.right.value = drive.power*drive.right.maximum>>8;
  drive.left.value = drive.power*drive.left.maximum>>8;     
  analogWrite(RIGHT,drive.right.value);
  analogWrite(LEFT,drive.left.value);
  start = 0xFFFFFFFF;
  //prox_sensor_run = true;
  Sprintln("Forward");
}

void hardLeft(void) {    
  analogWrite(RIGHT,drive.right.maximum);
  analogWrite(LEFT,0);
  start = millis()+duration;
  action = FORWARD;
  Sprint("Hard turn left");
}

void hardRight(void) {    
  analogWrite(LEFT,drive.left.maximum);
  analogWrite(RIGHT,0);
  start = millis()+duration;
  action = FORWARD;
  Sprint("Hard turn right");
}

void resumeOperation() {
  analogWrite(RIGHT,drive.right.value);
  analogWrite(LEFT,drive.left.value);
  Sprint("Resume");
}

void turn() {
  if (drive.steer < 0) {
    drive.left.value = ((256+drive.steer)*drive.power*drive.left.maximum) >> 16;
    analogWrite(LEFT,drive.left.value);
    drive.right.value = drive.power*drive.right.maximum>>8;
    analogWrite(RIGHT,drive.right.value);
  }
  else {
    drive.right.value = ((256-drive.steer)*drive.power*drive.right.maximum) >> 16;
    analogWrite(RIGHT,drive.right.value);
    drive.left.value= drive.power*drive.left.maximum>>8;
    analogWrite(LEFT,drive.left.value);
  }
}

