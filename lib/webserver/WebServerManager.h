//
// Created by guillermo on 11/18/24.
//

#ifndef WEBSERVERMANAGER_H
#define WEBSERVERMANAGER_H

#include <WiFiServer.h>
#include <WiFiClient.h>

#define CLIENT_TIMEOUT 2000

class WebServerManager {
private:
    void checkEndpoints(WiFiClient *client, const String &message);

    static String prepareBaseHeaders();

    static String prepareHeaders();

    static String prepareHtmlPage();

    void checkForConnections();

    static void printDebugParam(const String &key, const String &value);

    static String urlDecode(String &src);

public:
    std::unique_ptr<WiFiServer> server;

    void setup();

    void run() const;

    void loop();
};


#endif //WEBSERVERMANAGER_H
