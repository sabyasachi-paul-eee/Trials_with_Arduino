


#include <inttypes.h>
#include <Wire.h>
#include <Arduino.h>
#include <SHT2x.h>
#include <Ethernet.h>
#include <SPI.h>
#include <librarytrial.h>




 void librarytrial::setup_system()
{
  IPAddress ip(192,168,1,129); 
  EthernetClient client; 
  EthernetServer server(80);

  byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED }; //Setting MAC Address

  Wire.begin();
  Serial.begin(9600);

  Serial.println ("Serial confirmed");
  
  if (Ethernet.begin(mac) == 0) 
  {
    Serial.println("Failed to configure Ethernet using DHCP");
    Serial.print("mac failed");}
  else 
  {
    Ethernet.begin(mac, ip);
    Serial.println("Connetced");
    Serial.print("device is at ");
    Serial.println(Ethernet.localIP());
    server.begin();
    Serial.print("server is at ");
    Serial.println(Ethernet.localIP());
  }
 // delay(1000);
}


void librarytrial::looping_system()
{
  float humidityData;
  float temperatureData;
  
  IPAddress ip(192,168,1,129); 
  EthernetServer server(80); 
  EthernetClient client = server.available();
  
  
Serial.println(client);
 
  humidityData=(SHT2x.GetHumidity());
  Serial.print("Humidity(%RH) from sensor: ");
  Serial.println (humidityData);
 
  temperatureData=(SHT2x.GetTemperature());
  Serial.print("Temperature(C) from sensor: ");
  Serial.println (temperatureData);



   // listen for incoming clients
  
  if (client) {
    Serial.println("new client");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");  // the connection will be closed after completion of the response
          client.println("Refresh: 5");  // refresh the page automatically every 5 sec
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
          // output the value of sensor data
          
            
            client.print("temperature value is: ");
            client.print(temperatureData);
            client.println("humidity value is: ");
            client.print(humidityData);
            client.println("<br />");
          
          client.println("</html>");
          break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        } else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    client.stop();
    Serial.println("client disconnected");
  }
}



