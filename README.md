# esp-homekit-retrofit

Collection of sketches for the ESP32 to retrofit existing devices with HomeKit support for home automation.

## Hardware Support

This should work with any ESP32 board that has a minimum of 2MB of flash. ESP8266 boards may work, but are not supported.

## Sketches

- [DimmableLamp](./sketches/DimmableLamp.ino) - Dimmable lamp using a PWM output to control the brightness.

## Prerequisites

It is recommended that you use the [Arduino IDE](https://www.arduino.cc/en/software) (2.0+) to compile and upload the sketches.

### Board Manager

You will need to [add the ESP32 board support](https://espressif-docs.readthedocs-hosted.com/projects/arduino-esp32/en/latest/installing.html) to your Arduino IDE.

### Library Manager

You will need to add the following libraries to your Arduino IDE:

- [HomeSpan](https://github.com/HomeSpan/HomeSpan) - HomeKit library for ESP32

## Installation

You can clone this repository and open the sketches individually, from the [`sketches`](./sketches) folder.

## Configuration

When you first upload a sketch, it will be missing WiFi credentials and a unique HomeKit code.
You can set these using the Serial Monitor in the Arduino IDE, issuing HomeSpan CLI commands to the ESP32.

### HomeKit Pairing Code

The HomeKit pairing code is an 8-digit number that is used to pair the ESP32 with your iOS device.
By default, the pairing code is set to `466-37-726` and can be changed using the `S xxxxxxxx` command.

**NOTE:** The pairing code cannot be all the same number, `12345678` or `87654321`, as these are considered too easy to guess.

### WiFi Credentials

The WiFi credentials are used to connect the ESP32 to your home network.
You can begin WiFi setup by issuing the `W` command, which will scan for available networks and list them in the Serial Monitor.

You can then select a network by sending the index number shown in the Serial Monitor, followed by the network password when prompted.
Alternatively, you can manually enter the SSID which can be useful if the network is hidden.

It will then attempt to connect to the network and save the credentials to flash. Upon reboot, the ESP32 will attempt to connect to the network automatically.

### Resetting

If you need to reset the ESP32, you can issue the `E` command to reset the WiFi credentials and pairing code back to the defaults.
This will unpair the ESP32 from your iOS device, and you will need to re-pair it.

## Troubleshooting

### WiFi Connection Issues

If you are having issues detecting or connecting to your WiFi network, you can try the following:

- Ensure that your WiFi network is available on 2.4GHz (not only 5GHz).
- Ensure that your WiFi network is not using a hidden SSID.
- Ensure that your WiFi network is not using a custom channel, band, bandwidth, or channel width.
- Ensure that your WiFi network is not using band steering (this can cause issues).
- Try moving the ESP32 closer to your WiFi router.
- Try using an external antenna for the ESP32. These are usually the "U" models and have IPEX connectors.
