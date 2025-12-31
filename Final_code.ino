#include <DHT.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#define BLYNK_PRINT Serial
#define DHTPIN D3          // chan GPIO duoc ket noi
DHT dht(DHTPIN, DHT11);
char auth[] = "bV6v5cwR-kINmqKkbgP8uYQTXIgQoWBs";  //code gui toi email

const int moisturePin = A0;             // chan do do am
const int motorPin = D0;
unsigned long interval = 10000;
unsigned long previousMillis = 0;
unsigned long interval1 = 1000;
unsigned long previousMillis1 = 0;
float moisturePercentage;              //phantram do am
float h;                  // doc do am
float t;                  // doc nhiet do

void setup()
{
  Serial.begin(115200);
  delay(10);
  pinMode(motorPin, OUTPUT);
  digitalWrite(motorPin, LOW); // tat
  dht.begin();
  Blynk.begin(auth, "anhtudeptrai", "skibidtoilet1234");  //ten+pass wifi

  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop()
{
  Blynk.run();
  unsigned long currentMillis = millis(); // set

  h = dht.readHumidity();     // doc
  t = dht.readTemperature();     // doc

  if (isnan(h) || isnan(t))
  {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  moisturePercentage = ( 100.00 - ( (analogRead(moisturePin) / 1023.00) * 100.00 ) );

  if ((unsigned long)(currentMillis - previousMillis1) >= interval1) {
    Serial.print("Soil Moisture is  = ");
    Serial.print(moisturePercentage);
    Serial.println("%");
    previousMillis1 = millis();
  }

  if (moisturePercentage < 50) {
    digitalWrite(motorPin, HIGH);         // bat motor
  }
  if (moisturePercentage > 50 && moisturePercentage < 55) {
    digitalWrite(motorPin, HIGH);        // bat
  }
  if (moisturePercentage > 56) {
    digitalWrite(motorPin, LOW);          // tat
  }

  if ((unsigned long)(currentMillis - previousMillis) >= interval) {
    previousMillis = millis();
  }
    String url1="Moisture Percentage: ";
    url1+=moisturePercentage;
    url1+="%. Temperature: ";
    url1+=t;
    url1+=" C, Humidity: ";
    url1+=h;
  Blynk.notify(url1);

}
