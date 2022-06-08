# Smart home prototype (v1)

## Start project

The project is developed with [platformIO](platformio.com), to see how can be setting up in your favorite IDE, check the official documentation.

**Run the project**

With platformIO installed you can use the next commands in the project:

```
// build and upload project (in ESP8266-ESP01S hardware)
pio run -e esp01 -t upload

// build and upload project (in Nodemcu hardware)
pio run -e nodemcuv2 -t upload

// see arduino logs
pio device monitor
```

> Note: `pio` is the short of `platformio`.

## Hardware

- ESP8266 ESP01S
- Nodemcu v2

> It can be use with any ESP8266 hardware, but it was only tested so far with the two above.

 ## Server side

[Smart home server]()

## Contributions

Any contribution is welcome

## Author

[wil92]()
