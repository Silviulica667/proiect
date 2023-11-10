
//Arduino Code - 1602LCD I2C
 
const int triggerPin = 9;
const int echoPin = 10;
float pulse_width, distance;

void setup() {
  // put your setup code here, to run once:
  pinMode(triggerPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Serial.begin(9600);
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
  pulse_width = pulseIn(echoPin, HIGH);
  distance = (pulse_width*.0343)/2;
  Serial.print("Distance = ");
  Serial.println(distance);
  Serial.println(" cm");
  delay(500);
}