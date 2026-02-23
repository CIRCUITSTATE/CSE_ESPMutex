
![CSE_ESPMutex](https://socialify.git.ci/CIRCUITSTATE/CSE_ESPMutex/image?description=1&font=KoHo&forks=1&issues=1&logo=https%3A%2F%2Fwww.circuitstate.com%2Fwp-content%2Fuploads%2F2024%2F05%2FCIRCUITSTATE-R-Emblem-20052024-2.svg&name=1&pattern=Circuit%20Board&pulls=1&stargazers=1&theme=Auto)

# CSE_ESPMutex

**CSE_ESPMutex** is a Arduino library from [*CIRCUITSTATE Electronics*](https://www.circuitstate.com/) to create and manage Mutex Variables in the ESP32 platform. It allows you to create mutex variables for shared parallel tasks, gives an intuitive way to acquire and release them. You can use the mutexes to protect the access to any type of variables and prevent race conditions.

This library was developed with the help of AI. Thanks to humanity.

## Installation

This library is available from the official **Arduino Library Manager**. Open the Arduino IDE, search for "CSE_ESPMutex" and install the latest version of the library.

Additionally, you can download the latest release package from the GitHub repository and install it manually. To do so, open the Arduino IDE, go to `Sketch > Include Library > Add .ZIP Library…` and select the downloaded file.

Another method is to clone the GitHub repository directly into your `libraries` folder. The development branch will have the latest features, bug fixes and other changes. To do so, navigate to your `libraries` folder (usually located at `Documents/Arduino/libraries` on Windows and `~/Documents/Arduino/libraries` on macOS) and execute the following command:

```
git clone https://github.com/CIRCUITSTATE/CSE_ESPMutex.git
```

[**Git**](https://git-scm.com) should be installed in your computer.

The library can also be installed via [**PlatformIO**](https://platformio.org). All officially listed Arduino listed libraries are automatically fetched by PlatformIO. Use the [`lib_deps`](https://docs.platformio.org/en/latest/projectconf/sections/env/options/library/lib_deps.html) search option to install the library.

## Dependencies

This library does not have any external dependencies apart from the standard Arduino libraries.

## Examples

- [**Basic**](/examples/Basic/) - Shows how a counter variables is shared between two concurrent tasks.

## API Documentation

Please see the [API.md](/docs/API.md) file for the API reference.

## References

- [**How to Write Parallel Multitasking Applications for ESP32 using FreeRTOS & Arduino - CIRCUITSTATE**](https://www.circuitstate.com/tutorials/how-to-write-parallel-multitasking-applications-for-esp32-using-freertos-arduino/)
