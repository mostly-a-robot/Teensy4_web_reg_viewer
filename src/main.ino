#include <Arduino.h>
#include <ArduinoWebsockets.h>
#include <QNEthernet.h>
#include <TeensyID.h>
#include <TeensyTimerTool.h>
#include "HTML.h"
#include <map>
#include "registerSetup.h"
#include <SD.h>

#define SD_CS_PIN BUILTIN_SDCARD  // Teensy 4.1 built-in SD card

//#define USING_DHCP            true
#define USING_DHCP            false
#define DEB //<=== uncomment for serial debugging

#if !USING_DHCP
  // Set the static IP address to use if the DHCP fails to assign
  IPAddress myIP(192, 168, 127, 254);   //DAS instrumentation subnet
//  IPAddress myIP(169, 254, 65, 111);
  IPAddress myNetmask(255, 255, 255, 0);
  IPAddress myGW(192, 168, 2, 1);
  //IPAddress mydnsServer(192, 168, 2, 1);
  IPAddress mydnsServer(8, 8, 8, 8);
#endif

using namespace websockets;

byte mac[6];

// Enter websockets server port.
const uint16_t websocketsPort = 8080;

// Define how many clients we accpet simultaneously.
const byte maxSocketClients = 4;

WebsocketsClient socketClients[maxSocketClients];
WebsocketsServer socketServer;
EthernetServer httpServer;

String reqStr;
EXTMEM String document;

// --- Streaming Register Support ---
struct StreamRequest {
    uint32_t address;
    uint32_t lastValue;
    uint32_t lastSendTime;
    uint8_t cardId;
    uint8_t bitWidth;  // Add bit width field
    bool active;
};

std::map<uint8_t, StreamRequest> streamRequests; // key: cardId

uint32_t readRegister(uint32_t address, uint8_t bitWidth = 32);

void doReboot() {
  #ifdef DEB
  Serial.println("rebbooting... ");
  #endif
  SCB_AIRCR = 0x05FA0004;
}

void setup() {
  teensyMAC(mac);

  // Start Serial for commands and debugging
  Serial.begin(115200);
  while (!Serial && millis() < 3000) {
    ; // Wait for serial port to connect, or timeout after 3 seconds
  }
  
  Serial.println("=== ADC Data Logger with Web Interface ===");
  Serial.println("Starting system...");

  // Start //Serial and wait until it is ready.
  #ifdef DEB
  Serial.println("Debug mode enabled");
  #endif

  while (Ethernet.linkStatus() != LinkON)
  {
    #if USING_DHCP
      // Start the Ethernet connection, using DHCP
      #ifdef DEB
      Serial.print("Initialize Ethernet using DHCP => ");
      #endif

      Ethernet.begin(mac);
    #else
      // Start the Ethernet connection, using static IP
      #ifdef DEB
      Serial.print("Initialize Ethernet using static IP => ");
      #endif

      Ethernet.begin(mac, myIP);
    #endif

    // Check for Ethernet hardware present
    if (Ethernet.hardwareStatus() == EthernetNoHardware) {

      #ifdef DEB
      Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
      #endif

      // Keyboard.println("Ethernet cable is not connected.");
      delay(10000);
    }
    else if (Ethernet.linkStatus() == LinkOFF) 
    {
      #ifdef DEB
      Serial.println("Ethernet cable is not connected.");
      #endif

      // Keyboard.println("Ethernet cable is not connected.");
      delay(5000);
    }
    else if (Ethernet.linkStatus() == LinkON) 
    {
      #ifdef DEB
      Serial.print(F("Connected! IP address:")); //Serial.println(Ethernet.localIP());
      #endif

      // Keyboard.print("Connected! IP address:"); Keyboard.println(Ethernet.localIP());
    }

    #if USING_DHCP
      delay(1000);
    #else
      delay(200);
    #endif

    regSetup(); // Setup registers and DMA
  }

  // Start websockets server.
  socketServer.listen(websocketsPort);
  if (!socketServer.available()) {

    #ifdef DEB
    Serial.println("Websockets Server not available!");
    #endif

  }

  // Start http server.
  httpServer.begin(80);

  #ifdef DEB
  Serial.print("Visit http://");
  Serial.print(Ethernet.localIP());
  Serial.println(" in the browser to connect.");
  #endif
}

int8_t getFreeSocketClientIndex() {
  // If a client in our list is not available, it's connection is closed and we
  // can use it for a new client.  
  for (byte i = 0; i < maxSocketClients; i++) {
    if (!socketClients[i].available()) return i;
  }
  return -1;
}

// Call this in your WebSocket message handler
void handleMessage(WebsocketsClient &client, WebsocketsMessage message) {
    auto data = message.data();

    #ifdef DEB
    Serial.print("Got Message: ");
    Serial.println(data);
    #endif

    // Handle ping messages - respond with pong
    if (data == "ping") {
        client.send("pong");
        #ifdef DEB
        Serial.println("Sent pong response");
        #endif
        return;
    }

    // Handle JSON streaming requests
    if (data.startsWith("{")) {
        // Parse JSON for {"action":"startStream","address":"0x40000000","cardId":0,"bitWidth":32}
        if (data.indexOf("\"action\":\"startStream\"") != -1) {
            int addrIdx = data.indexOf("\"address\":\"") + 11;
            int addrEnd = data.indexOf("\"", addrIdx);
            String addressStr = data.substring(addrIdx, addrEnd);
            
            int cardIdx = data.indexOf("\"cardId\":") + 9;
            int cardEnd = data.indexOf(",", cardIdx);
            if (cardEnd == -1) cardEnd = data.indexOf("}", cardIdx);
            String cardIdStr = data.substring(cardIdx, cardEnd);
            
            // Extract bit width if present
            uint8_t bitWidth = 32;
            int widthIdx = data.indexOf("\"bitWidth\":");
            if (widthIdx != -1) {
                widthIdx += 11;
                int widthEnd = data.indexOf(",", widthIdx);
                if (widthEnd == -1) widthEnd = data.indexOf("}", widthIdx);
                String widthStr = data.substring(widthIdx, widthEnd);
                bitWidth = atoi(widthStr.c_str());
            }
            
            uint32_t address = strtoul(addressStr.c_str(), nullptr, 16);
            uint8_t cardId = atoi(cardIdStr.c_str());
            streamRequests[cardId] = {address, 0, 0, cardId, bitWidth, true};
            
            #ifdef DEB
            Serial.printf("Start streaming cardId %d, address 0x%08lX, bitWidth %d\n", cardId, address, bitWidth);
            #endif
        } else if (data.indexOf("\"action\":\"stopStream\"") != -1) {
            int cardIdx = data.indexOf("\"cardId\":") + 9;
            int cardEnd = data.indexOf("}", cardIdx);
            String cardIdStr = data.substring(cardIdx, cardEnd);
            uint8_t cardId = atoi(cardIdStr.c_str());
            streamRequests.erase(cardId);
            #ifdef DEB
            Serial.printf("Stop streaming cardId %d\n", cardId);
            #endif
        }
        return;
    }

    // Echo message (for other messages)
    client.send("Echo: " + data);
}

// --- In your loop() or a timer, periodically send stream updates ---
void sendStreamUpdates() {
    static uint32_t lastTick = 0;
    uint32_t now = millis();
    if (now - lastTick < 50) return; // 5 Hz update rate
    lastTick = now;

    for (auto &kv : streamRequests) {
        auto &req = kv.second;
        if (!req.active) continue;
        uint32_t value = readRegister(req.address, req.bitWidth);  // Use bitWidth here
        // Only send if value changed or every time (remove this check if you want constant updates)
        if (value != req.lastValue || (now - req.lastSendTime > 1000)) {
            req.lastValue = value;
            req.lastSendTime = now;
            String msg = "streamValue:" + String(req.cardId) + ":" + String(value, HEX);
            for (byte i = 0; i < maxSocketClients; i++) {
                if (socketClients[i].available()) {
                    socketClients[i].send(msg);
                }
            }
        }
    }
}

void handleEvent(WebsocketsClient &client, WebsocketsEvent event, String data) {
  if (event == WebsocketsEvent::ConnectionClosed) {

    #ifdef DEB
    Serial.println("Connection closed");
    #endif

  }
}

void listenForSocketClients() {
  if (socketServer.poll()) {
    int8_t freeIndex = getFreeSocketClientIndex();
    if (freeIndex >= 0) {
      WebsocketsClient newClient = socketServer.accept();

      #ifdef DEB
      Serial.printf("Accepted new websockets client at index %d\n", freeIndex);
      #endif

      newClient.onMessage(handleMessage);
      newClient.onEvent(handleEvent);
      newClient.send("Hello from Teensy");
      socketClients[freeIndex] = newClient;
    } else {
      #ifdef DEB
      Serial.println("No free socket client available");
      #endif
    }

  }
}

void pollSocketClients() {
  for (byte i = 0; i < maxSocketClients; i++) {
    socketClients[i].poll();
  }
}

uint32_t readRegister(uint32_t address, uint8_t bitWidth) {
  switch (bitWidth) {
    case 8:
      return *(volatile uint8_t *)address;
    case 16:
      return *(volatile uint16_t *)address;
    case 32:
    default:
      return *(volatile uint32_t *)address;
  }
}

void writeRegister(uint32_t address, uint32_t value, uint8_t bitWidth = 32) {
  Serial.print("Writing to address: 0x"); Serial.print(address, HEX); 
  Serial.print(" value: 0x"); Serial.print(value, HEX);
  Serial.print(" width: "); Serial.print(bitWidth); Serial.println(" bits");
  
  switch (bitWidth) {
    case 8:
      *(volatile uint8_t *)address = (uint8_t)value;
      break;
    case 16:
      *(volatile uint16_t *)address = (uint16_t)value;
      break;
    case 32:
    default:
      *(volatile uint32_t *)address = value;
      break;
  }
}

void listenForHttpClients() {
  const char* header = "";
  String buff;
  int buffSize=1024*2;
  int len = 0;
  bool send_chunked = true;

  // Create a client connection
  EthernetClient client = httpServer.available();
  if (client) {
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();

        //read char by char HTTP request
        if (reqStr.length() < 100) {

          //store characters to string
          reqStr += c;
        }

        //if HTTP request has ended
        if (c == '\n') {

          #ifdef DEB
          Serial.println("Client request: <====================");
          Serial.println(reqStr);
          #endif

          if(reqStr.indexOf("/style.css") >0)//checks for on
          {
            document = css;
            header = 
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/css\r\n"
            "Connection: close\r\n"
            "\r\n";
          } else

          // if(reqStr.indexOf("/script.js") >0)//checks for on
          // {
          //   document = script;
          //   header = 
          //   "HTTP/1.1 200 OK\r\n"
          //   "Content-Type: text/javascript\r\n"
          //   "Connection: close\r\n"
          //   "\r\n";
          // } else

          if (reqStr.indexOf("/writeRegister?address=") >= 0) {
              // Extract the register address and value from the URL
              int addrStart = reqStr.indexOf("address=") + 8;
              int addrEnd = reqStr.indexOf("&", addrStart);
              String addressStr = reqStr.substring(addrStart, addrEnd);
              int valueStart = reqStr.indexOf("value=", addrEnd) + 6;
              int valueEnd = reqStr.indexOf("&", valueStart);
              if (valueEnd == -1) valueEnd = reqStr.indexOf(" ", valueStart);
              String valueStr = reqStr.substring(valueStart, valueEnd);
              
              // Extract bit width if present
              uint8_t bitWidth = 32;
              int widthStart = reqStr.indexOf("bitWidth=");
              if (widthStart != -1) {
                  widthStart += 9;
                  int widthEnd = reqStr.indexOf("&", widthStart);
                  if (widthEnd == -1) widthEnd = reqStr.indexOf(" ", widthStart);
                  String widthStr = reqStr.substring(widthStart, widthEnd);
                  bitWidth = atoi(widthStr.c_str());
              }

              uint32_t address = strtoul(addressStr.c_str(), nullptr, 16);
              uint32_t value = strtoul(valueStr.c_str(), nullptr, 16);

              writeRegister(address, value, bitWidth);
              uint32_t result = readRegister(address, bitWidth);

              String response = "registerValue:" + String(address, HEX) + ":" + String(result, HEX);
              for (byte i = 0; i < maxSocketClients; i++) {
                  if (socketClients[i].available()) {
                      socketClients[i].send(response);
                  }
              }

              header = "HTTP/1.1 200 OK\r\n\r\n";
          } else

          if (reqStr.indexOf("/readRegister?address=") >= 0) {
              // Extract the register address from the URL
              int addrStart = reqStr.indexOf("address=") + 8;
              int addrEnd = reqStr.indexOf("&", addrStart);
              if (addrEnd == -1) addrEnd = reqStr.indexOf(" ", addrStart);
              String addressStr = reqStr.substring(addrStart, addrEnd);
              
              // Extract bit width if present
              uint8_t bitWidth = 32;
              int widthStart = reqStr.indexOf("bitWidth=");
              if (widthStart != -1) {
                  widthStart += 9;
                  int widthEnd = reqStr.indexOf("&", widthStart);
                  if (widthEnd == -1) widthEnd = reqStr.indexOf(" ", widthStart);
                  String widthStr = reqStr.substring(widthStart, widthEnd);
                  bitWidth = atoi(widthStr.c_str());
              }

              uint32_t address = strtoul(addressStr.c_str(), nullptr, 16);

              // Read back the value to confirm with specified bit width
              uint32_t result = readRegister(address, bitWidth);

              // Send the value via WebSocket to all connected clients
              String response = "registerValue:" + String(address, HEX) + ":" + String(result, HEX);
              for (byte i = 0; i < maxSocketClients; i++) {
                  if (socketClients[i].available()) {
                      socketClients[i].send(response);
                  }
              }

              header =
                  "HTTP/1.1 200 OK\r\n"
                  "\r\n";
          } else

          if (reqStr.indexOf("/files") >= 0) {
              // List all CSV files on SD card
              String fileList = "[";
              bool first = true;
              
              if (SD.begin(SD_CS_PIN)) {
                  File dataDir = SD.open("/data");
                  if (dataDir) {
                      while (true) {
                          File entry = dataDir.openNextFile();
                          if (!entry) break;
                          
                          if (!entry.isDirectory()) {
                              String fileName = String(entry.name());
                              if (fileName.endsWith(".csv")) {
                                  if (!first) fileList += ",";
                                  fileList += "{";
                                  fileList += "\"name\":\"" + fileName + "\",";
                                  fileList += "\"size\":" + String(entry.size());
                                  fileList += "}";
                                  first = false;
                              }
                          }
                          entry.close();
                      }
                      dataDir.close();
                  }
              }
              fileList += "]";
              
              header = 
                  "HTTP/1.1 200 OK\r\n"
                  "Content-Type: application/json\r\n"
                  "Connection: close\r\n"
                  "\r\n";
              
              document = fileList;
          } else

          if (reqStr.indexOf("/download?file=") >= 0) {
              // Download specific file
              int fileStart = reqStr.indexOf("file=") + 5;
              int fileEnd = reqStr.indexOf("&", fileStart);
              if (fileEnd == -1) fileEnd = reqStr.indexOf(" ", fileStart);
              String fileName = reqStr.substring(fileStart, fileEnd);
              
              String filePath = "/data/" + fileName;
              
              if (SD.begin(SD_CS_PIN) && SD.exists(filePath.c_str())) {
                  File downloadFile = SD.open(filePath.c_str(), FILE_READ);
                  if (downloadFile) {
                      // Send file download headers
                      String downloadHeader = 
                          "HTTP/1.1 200 OK\r\n"
                          "Content-Type: text/csv\r\n"
                          "Content-Disposition: attachment; filename=\"" + fileName + "\"\r\n"
                          "Content-Length: " + String(downloadFile.size()) + "\r\n"
                          "Connection: close\r\n"
                          "\r\n";
                      
                      client.write(downloadHeader.c_str());
                      client.flush();
                      
                      // Stream file contents with moderate buffer size
                      uint8_t buffer[1024];  // Moderate buffer size that works reliably
                      uint32_t totalSent = 0;
                      while (downloadFile.available()) {
                          int bytesRead = downloadFile.read(buffer, sizeof(buffer));
                          if (bytesRead > 0) {
                              client.write(buffer, bytesRead);
                              totalSent += bytesRead;
                              // Flush every 4KB for reliability
                              if (totalSent % 4096 == 0) {
                                  client.flush();
                              }
                          }
                      }
                      // Final flush to ensure all data is sent
                      client.flush();
                      downloadFile.close();
                      
                      #ifdef DEB
                      Serial.println("File download completed: " + fileName);
                      #endif
                      
                      client.stop();
                      reqStr = "";
                      return;
                  }
              }
              
              // File not found
              header = "HTTP/1.1 404 Not Found\r\n\r\n";
              document = "File not found: " + fileName;
          } else

          if (reqStr.indexOf("/delete?file=") >= 0) {
              // Delete specific file
              int fileStart = reqStr.indexOf("file=") + 5;
              int fileEnd = reqStr.indexOf("&", fileStart);
              if (fileEnd == -1) fileEnd = reqStr.indexOf(" ", fileStart);
              String fileName = reqStr.substring(fileStart, fileEnd);
              
              String filePath = "/data/" + fileName;
              
              if (SD.begin(SD_CS_PIN) && SD.exists(filePath.c_str())) {
                  bool deleteSuccess = SD.remove(filePath.c_str());
                  if (deleteSuccess) {
                      #ifdef DEB
                      Serial.println("File deleted successfully: " + fileName);
                      #endif
                      
                      header = 
                          "HTTP/1.1 200 OK\r\n"
                          "Content-Type: application/json\r\n"
                          "Connection: close\r\n"
                          "\r\n";
                      document = "{\"status\":\"success\",\"message\":\"File deleted: " + fileName + "\"}";
                  } else {
                      #ifdef DEB
                      Serial.println("Failed to delete file: " + fileName);
                      #endif
                      
                      header = 
                          "HTTP/1.1 500 Internal Server Error\r\n"
                          "Content-Type: application/json\r\n"
                          "Connection: close\r\n"
                          "\r\n";
                      document = "{\"status\":\"error\",\"message\":\"Failed to delete file: " + fileName + "\"}";
                  }
              } else {
                  // File not found
                  #ifdef DEB
                  Serial.println("File not found for deletion: " + fileName);
                  #endif
                  
                  header = 
                      "HTTP/1.1 404 Not Found\r\n"
                      "Content-Type: application/json\r\n"
                      "Connection: close\r\n"
                      "\r\n";
                  document = "{\"status\":\"error\",\"message\":\"File not found: " + fileName + "\"}";
              }
          } else

          if (reqStr.indexOf("/reset") >= 0) {
              #ifdef DEB
              Serial.println("Reset request received - rebooting in 1 second");
              #endif
              
              // Send response first
              header = "HTTP/1.1 200 OK\r\n\r\n";
              client.write(header);
              client.flush();
              client.stop();
              
              // Brief delay then reboot
              delay(1000);
              doReboot();
          } else

          {
            document = html_bitview;

            header = 
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/html\r\n"
            "Connection: close\r\n"
            "\r\n";

          }

          len = document.length();

          client.write(header);

          #ifdef DEB
          Serial.println("header sent");
          Serial.print("bytes to send: "); Serial.println(len);
          #endif

          #ifdef DEB
          Serial.print("Client connected: ");
          Serial.println(client.connected());
          #endif
          
          if (send_chunked){
            if (len){   //only send if there is a document to send.
              for(int i=0; i<=len; i+=buffSize){
                if ((i+buffSize) > len){
                  buff = document.substring(i,len);

                  if (client.connected()) {
                    client.write(buff.c_str(), buff.length());
                    client.flush();
                  } else {
                    #ifdef DEB
                    Serial.println("Socket not connected");
                    #endif
                  }
                  break;
                } else {
                buff = document.substring(i,i+buffSize);

                if (client.connected()) {
                  client.write(buff.c_str(),buffSize);
                  client.flush();
                } else {
                  #ifdef DEB
                  Serial.println("Socket not connected");
                  #endif
                }
                #ifdef DEB
                Serial.print("Buffer wrote: "); Serial.print(i+buffSize); Serial.println(" bytes");
                #endif
                }
              }
            }
          }

          send_chunked = true;

          delay(1);
         //stopping client
          client.stop();
          delay(10);
          reqStr ="";
        } // <-- This was the missing closing brace for the if (c == '\n') block
      } // <-- Closing brace for if (client.available())
    } // <-- Closing brace for while (client.connected())
  } // <-- Closing brace for if (client)
}

void loop() {
  listenForSocketClients();
  pollSocketClients();
  listenForHttpClients();
  sendStreamUpdates();
  regLoop();  // Handle ADC data and serial commands
  // socketClients[0].send("clic - source: start/stop button");
}