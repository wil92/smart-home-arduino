//
// Created by guillermo on 11/19/24.
//

#ifndef INDEX_H
#define INDEX_H

const char base_html_page[] PROGMEM = R"CERT(
<!DOCTYPE html>
<html>
  <head>
    <title>Device configuration page</title>
  </head>
  <body>
    <h1>Configuration Page</h1>
    <h2>Device configuration</h2>
    <div>
      <label for="deviceId">ID</label>
      <input type="text" id="deviceId" value="$$DEVICE_ID$$">
    </div>
    <div>
      <label for="deviceName">Name</label>
      <input type="text" id="deviceName" value="$$DEVICE_NAME$$">
    </div>
    <div>
      <label for="deviceType">Type</label>
      <input type="text" id="deviceType" value="$$DEVICE_TYPE$$">
    </div>
    <h2>Network</h2>
    <div>
      <label for="ssid">SSID</label>
      <input type="text" id="ssid" value="$$SSID$$">
    </div>
    <div>
      <label for="password">Password</label>
      <input type="text" id="password" value="$$PASSWORD$$">
    </div>
    <h2>Server</h2>
    <div>
      <label for="host">Host</label>
      <input type="text" id="host" value="$$HOST$$">
    </div>
    <div>
      <label for="port">Port</label>
      <input type="text" id="port" value="$$PORT$$">
    </div>
    <div>
      <label for="url">Url</label>
      <input type="text" id="url" value="$$URL$$">
    </div>
    <button id="summit" style="margin-top: 20px;">Configure device</button>
  </body>
  <script>
    var but = document.getElementById("summit");
    var deviceId = document.getElementById("deviceId");
    var deviceName = document.getElementById("deviceName");
    var deviceType = document.getElementById("deviceType");

    var ssid = document.getElementById("ssid");
    var password = document.getElementById("password");

    var host = document.getElementById("host");
    var port = document.getElementById("port");
    var url = document.getElementById("url");

    but.onclick = function() {
      var reqUrl = `/?deviceId=${encodeURI(deviceId.value)}&deviceName=${encodeURI(deviceName.value)}&deviceType=${encodeURI(deviceType.value)}&ssid=${encodeURI(ssid.value)}&password=${encodeURI(password.value)}&host=${encodeURI(host.value)}&port=${encodeURI(port.value)}&url=${encodeURI(url.value)}`;
      fetch(reqUrl, {method: 'post'})
        .then(() => console.log("Device configurated"))
        .catch(e => console.error(e));
    };

    function removeDefaultValue(ele) {
      if (ele.value.startsWith("$$") && ele.value.endsWith("$$")) {
        ele.value = "";
      }
    }

    (function () {
      removeDefaultValue(deviceId);
      removeDefaultValue(deviceName);
      removeDefaultValue(deviceType);
      removeDefaultValue(ssid);
      removeDefaultValue(password);
      removeDefaultValue(host);
      removeDefaultValue(port);
      removeDefaultValue(url);
    })();
  </script>
</html>
)CERT";

#endif //INDEX_H
