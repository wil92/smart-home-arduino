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
        bool resetFlag = false;

        while (client.connected() && millis() - connectionTime <= CLIENT_TIMEOUT) {
            if (client.available()) {
                String line = client.readStringUntil('\r');
                message += line;

                if (line.length() == 1 && line[0] == '\n') {
                    resetFlag = checkEndpoints(&client, message);

                    break;
                }
            }
        }

        client.stop();

        if (resetFlag) {
            rf();
        }
    }
}

bool WebServerManager::checkEndpoints(WiFiClient *client, const String &message) const {
    Serial.println(message);
    int index;
    if (const String postHeader = "POST /"; (index = message.indexOf(postHeader)) >= 0) {
        index += 1 + static_cast<int>(postHeader.length());

        Serial.print("index: ");
        Serial.println(index);

        String key = "", value = "";
        bool flag = false;
        while (index < (int) message.length() && message[index] != ' ') {
            if (!flag) {
                if (message[index] == '=') {
                    flag = true;
                } else {
                    key += message[index];
                }
            } else {
                if (message[index] == '&') {
                    flag = false;
                    setParameter(urlDecode(key), urlDecode(value));
                    key = "";
                    value = "";
                } else {
                    value += message[index];
                }
            }
            index++;
        }
        setParameter(urlDecode(key), urlDecode(value));
        flashManager->saveSetup();
        flashManager->loadSetup();
        client->println(prepareBaseHeaders() + "\r\n");
        return true;
    }
    if (message.indexOf("GET /") >= 0) {
        client->println(prepareHeaders());
        client->println(prepareHtmlPage());
    }
    return false;
}

String WebServerManager::urlDecode(String &src) {
    String ret;
    for (int i = 0, ii; i < (int) src.length(); i++) {
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

void WebServerManager::setParameter(const String &key, const String &value) const {
    flashManager->setVariable(key, value);

    // only on debug
    Serial.print("Parameter: (\"");
    Serial.print(key);
    Serial.print("\": \"");
    Serial.print(value);
    Serial.println("\")");
}

String WebServerManager::prepareHeaders() const {
    return prepareBaseHeaders() +
           "Content-Type: text/html\r\n" +
           "Connection: close\r\n" + // the connection will be closed after completion of the response
           // "Refresh: 30\r\n" + // refresh the page automatically every 5 sec
           "\r\n";
}

String WebServerManager::prepareBaseHeaders() const {
    return String("HTTP/1.1 200 OK") + "\r\n";
}

String WebServerManager::prepareHtmlPage() const {
    String result = String(base_html_page) + "\r\n";

    // Network
    result.replace("$$SSID$$", flashManager->ssidNetwork);
    result.replace("$$PASSWORD$$", flashManager->passwordNetwork);

    // Server
    result.replace("$$HOST$$", flashManager->host);
    result.replace("$$PORT$$", String(flashManager->port));
    result.replace("$$URL$$", flashManager->url);

    // Device configuration
    result.replace("$$DEVICE_ID$$", flashManager->ID);
    result.replace("$$DEVICE_NAME$$", flashManager->name);
    result.replace("$$DEVICE_TYPE$$", flashManager->type);

    return result;
}
