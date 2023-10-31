void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
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

void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();

  // Feel free to add more if statements to control more GPIOs with MQTT

// Kawalan RELAY 1 ------------------------------------------------

  if (String(topic) == "psp/pamAir/relay1") {
    Serial.print("Changing output to ");
    if(messageTemp == "on"){
      Serial.println("on");
      digitalWrite(relay1, HIGH);
    }
    else if(messageTemp == "off"){
      Serial.println("off");
      digitalWrite(relay1, LOW);
    }
  }

  
//--------------------------- T A M A T ------------------ 

}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
//--------------------------------------------------------------------------    
    // Attempt to connect
    if (client.connect("PspProjek2023")) {
//--------------------------------------------------------------------------
      Serial.println("connected");
//--------------------------------------------------------------------------
      // Subscribe
      client.subscribe("psp/pamAir/relay1");     
//--------------------------------------------------------------------------
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
