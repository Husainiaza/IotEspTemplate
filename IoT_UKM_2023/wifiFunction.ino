void setup_wifi() {
  delay(10);
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

  display.setTextColor(WHITE,BLACK);
  display.setCursor(0,10);
  display.print("WiFi connected");
  display.setCursor(0,20);
  display.print(WiFi.localIP());
   display.display();
  delay(5000);
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

  // add more if statements to control more GPIOs with MQTT

  // If a message is received on the topic esp32/output, you check if the message is either "on" or "off". 
  if (String(topic) == "kksj/relay1") {
    Serial.print("Changing output to ");
    if(messageTemp == "on"){
      Serial.println("on");
      digitalWrite(Relay01,HIGH);
      client.publish("kksj/relay1/status", "RELAY 1 ON");
      display.setCursor(0,40);
      display.print(" SOCKET 1 ON ");
      display.display();
      
    }
    else if(messageTemp == "off"){
      Serial.println("off");
      digitalWrite(Relay01,LOW);
      client.publish("kebun/socket1/status", "RELAY 1 OFF");
      display.setCursor(0,40);
      display.print(" SOCKET 1 OFF");
      display.display();
    }
  }

 if (String(topic) == "kksj/relay2") {
    Serial.print("Changing output to ");
    if(messageTemp == "on"){
      Serial.println("on");
      digitalWrite(Relay02,HIGH);
      client.publish("kebun/socket2/status", "RELAY 2 ON");
    }
    else if(messageTemp == "off"){
      Serial.println("off");
      digitalWrite(Relay02,LOW);
       client.publish("kksj/relay2/status", "RELAY 2 OFF");
    }
  }

   if (String(topic) == "kksj/buzzer") {
    Serial.print("Changing output to ");
    if(messageTemp == "on"){
      Serial.println("on");
      digitalWrite(buzzer,HIGH);
      client.publish("kebun/socket2/status", "RELAY 2 ON");
    }
    else if(messageTemp == "off"){
      Serial.println("off");
      digitalWrite(buzzer,LOW);
       client.publish("kksj/relay2/status", "RELAY 2 OFF");
    }
  }
  
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("IoTkelasdenganUKM2003")) {
      Serial.println("connected");
      // Subscribe
      client.subscribe("kksj/relay1");
      client.subscribe("kksj/relay2");
      client.subscribe("kksj/buzzer");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
