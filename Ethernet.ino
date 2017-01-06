void webserver_setup() {
   // start the Ethernet connection and the server:
  Ethernet.begin(mac, ip);
  server.begin();
  
};

void webserver() {
  int s = millis();
  
  if (enableEthernet) {
      // listen for incoming clients
      EthernetClient client = server.available();
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
              client.println("Refresh: 1");  // refresh the page automatically every 5 sec
              client.println();
              client.println("<!DOCTYPE HTML>");
              client.println("<html>");
              // output the value of each analog input pin            
              client.print("Date: ");
              client.print(gpsDATE);
              client.println("<br />");
              
              client.print("Time: ");
              client.print(gpsTIME);
              client.println("<br />");
              
              client.print("Lat: ");
              client.print(gpsLAT,5);
              client.println("<br />");
              
              client.print("Lng: ");
              client.print(gpsLNG,5);
              client.println("<br />");
              
              client.print("Speed: ");
              client.print(gpsSOG,2);
              client.println("<br />");
              
              client.print("Course: ");
              client.print(gpsCOURSE,2);
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
        Ethernet.maintain();
      };
  };
  
  int sto = millis();
  cycEthernet=sto-s;
}
