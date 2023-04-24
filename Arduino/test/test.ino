void setup() {
  Serial.begin(9600);
  Serial.setTimeout(1000);
  Serial.println("Starting...");
}

void loop() {
  if (Serial.available() > 0) {
    String msg = Serial.readString();
    Serial.print("MEssage got: ");
    Serial.println(msg);
    delay(100);
  }
}
