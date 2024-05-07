#include <DHT.h>
#include <WiFi.h>
#include "ThingSpeak.h"

#define DHTPIN 22
DHT dht(DHTPIN, DHT11);

#define LDRPIN 32

#define WIFI_SSID "INFINITUM6923"
#define WIFI_PASS "1oDxvThMDx"

#define TS_CHANNEL_ID 2538211
#define TS_API_KEY "WE6ZM8HRSLE8382J"

WiFiClient client;

void setup() {
  Serial.begin(115200);
  dht.begin();

  ThingSpeak.begin(client);

  WiFi.mode(WIFI_STA);
}

void loop() {
  float t = dht.readTemperature();

  if (WiFi.status() != WL_CONNECTED) {
    Serial.print("Intentando conectar a Internet...");

    while (WiFi.status() != WL_CONNECTED) {
      WiFi.begin(WIFI_SSID, WIFI_PASS);
      delay(5000);
    }

    Serial.println("\nConectado!");
  }

  if (isnan(t)) {
    Serial.println("No se pudo leer la temperatura.");
  }

  int l = analogRead(LDRPIN);
  int ldr = map(l, 0, 4095, 0, 100);


  /* Luminosidad */
  {
    int responseCode = ThingSpeak.writeField(TS_CHANNEL_ID, 3, ldr, TS_API_KEY);
    if (responseCode != 200) {
      Serial.print("Error al enviar la luminosidad: ");
      Serial.println(responseCode);
    } else {
      Serial.print("Luminosidad enviada: ");
      Serial.println(ldr);
    }
  }

  delay(15*1000);

  /* Temperatura */
  {
    int responseCode = ThingSpeak.writeField(TS_CHANNEL_ID, 2, t, TS_API_KEY);
    if (responseCode != 200) {
      Serial.print("Error al enviar la temperatura: ");
      Serial.println(responseCode);
    } else {
      Serial.print("Temperatura enviada: ");
      Serial.println(t);
    }
  }

  delay(15*1000);
}
