# Web Server with ESP8266 using AT Commands

This repository provides a solution for microcontrollers interfaced with the ESP8266 WiFi module, allowing them to function as a simple web server using AT commands.

## Features:

1. **Web Server Functionality**: Ability to serve an HTML page from a microcontroller to any client connecting to the ESP8266's IP.
2. **Serial Communication**: Debug information is continuously printed to the serial monitor, aiding in troubleshooting and monitoring.
3. **Dynamic Connection Management**: Efficiently manages multiple client connections using a dynamic stack data structure.

## Detailed Functions:

- `getReply3(int wait)`: Awaits a specific duration while reading and echoing all available data from `Serial3`.

- `loadPacket(char *packet, int len, int clientId)`: Transmits data packets to a specific client.

- `loadHTML(int clientId)`: Serves an HTML content to a client.

- `breakConnection(int clientId)`: Terminates the connection with a specified client.

- `setup()`: Initializes the ESP8266, setting it up as a web server.

- `loop()`: The core loop that waits for connections and serves HTML content upon requests.

## Classes:

- `dynamicStack`: A utility class that implements stack operations, aiding in managing multiple client connections.

## Prerequisites:

- **Hardware**: Any microcontroller (e.g., Arduino) with an interfaced ESP8266 WiFi module.
- **Software**: Arduino IDE or a similar platform.
- **Dependencies**: Default Arduino libraries.

## Getting Started:

1. Populate the `htmlCode` variable with the HTML content you desire to serve.
2. Upload the code to your microcontroller.
3. Connect your device to the ESP8266's WiFi hotspot or ensure the ESP8266 is connected to the same network as your device.
4. Send a request to the ESP8266's IP address through a web browser.
5. You will be presented with the web page stored in the `htmlCode` variable.

## Additional Notes:

- Ensure your `htmlCode` content size doesn't exceed the limit set by the `SIZE` variable.
- The default baud rate for serial communication is set at 115200 for both the microcontroller and the ESP8266 (Serial3). Adjust this value if necessary for your specific setup.

---

**Contributions**: Please feel free to fork, improve upon this project, and create pull requests. Kindly report any bugs or feature requests in the issues section.
