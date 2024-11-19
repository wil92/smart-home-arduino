//
// Created by guillermo on 11/18/24.
//

#include "WebServerManager.h"
#include "views/index.h"

void WebServerManager::setup() {
    server = std::make_unique<WiFiServer>(80);
}

void WebServerManager::run() const {
    server->begin();
    server->setNoDelay(true);
}

void WebServerManager::loop() {
    checkForConnections();
}

void WebServerManager::checkForConnections() {
    WiFiClient client = server->accept();
    if (client) {
        Serial.println("Client connected");

        const unsigned long connectionTime = millis();
        String message = "";

        while (client.connected() && millis() - connectionTime <= CLIENT_TIMEOUT) {
            if (client.available()) {
                String line = client.readStringUntil('\r');
                message += line;

                if (line.length() == 1 && line[0] == '\n') {
                    checkEndpoints(&client, message);

                    break;
                }
            }
        }

        client.stop();
    }
}

void WebServerManager::checkEndpoints(WiFiClient *client, const String &message) {
    Serial.println(message);
    int index;
    if (const String postHeader = "POST /"; (index = message.indexOf(postHeader)) >= 0) {
        index += 1 + static_cast<int>(postHeader.length());

        Serial.print("index: ");
        Serial.println(index);

        String key = "", value = "";
        bool flag = false;
        while (index < message.length() && message[index] != ' ') {
            if (!flag) {
                if (message[index] == '=') {
                    flag = true;
                } else {
                    key += message[index];
                }
            } else {
                if (message[index] == '&') {
                    flag = false;
                    printDebugParam(key, value);
                    key = "";
                    value = "";
                } else {
                    value += message[index];
                }
            }
            index++;
        }
        printDebugParam(urlDecode(key), urlDecode(value));
        client->println(prepareBaseHeaders() + "\r\n");
    } else if (message.indexOf("GET /") >= 0) {
        client->println(prepareHeaders());
        client->println(prepareHtmlPage());
    }
}

String WebServerManager::urlDecode(String &src) {
    String ret;
    for (int i = 0, ii; i < src.length(); i++) {
        if (src[i] == '%') {
            sscanf(src.substring(i + 1, i + 3).c_str(), "%x", &ii);
            const char ch = static_cast<char>(ii);
            ret += ch;
            i = i + 2;
        } else {
            ret += src[i];
        }
    }
    return (ret);
}

void WebServerManager::printDebugParam(const String &key, const String &value) {
    Serial.print("Parameters: (\"");
    Serial.print(key);
    Serial.print("\": \"");
    Serial.print(value);
    Serial.println("\")");
}

String WebServerManager::prepareHeaders() {
    return prepareBaseHeaders() +
           "Content-Type: text/html\r\n" +
           "Connection: close\r\n" + // the connection will be closed after completion of the response
           // "Refresh: 30\r\n" + // refresh the page automatically every 5 sec
           "\r\n";
}

String WebServerManager::prepareBaseHeaders() {
    return String("HTTP/1.1 200 OK\r\n");
}

String WebServerManager::prepareHtmlPage() {
    return String(base_html_page) + "\r\n";
}
