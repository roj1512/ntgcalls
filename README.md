# NativeTgCalls: <br/>  An experimental implementation of Telegram Group Calls
Welcome to NativeTgCalls, an innovative open-source project. NativeTgCalls represents the next evolution in Telegram calling, building on the foundation laid by PyTgCalls.

|                                                                                         Powerful                                                                                          |                                                                                 Simple                                                                                 |                                                                                                   Light                                                                                                    |
|:-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------:|:----------------------------------------------------------------------------------------------------------------------------------------------------------------------:|:----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------:|
| <img src="https://raw.githubusercontent.com/pytgcalls/ntgcalls/master/.github/images/fast.gif" width=150 alt="Fast Logo"/><br>Built from scratch in C++ using Boost, libwebrtc and FFmpeg | <img src="https://raw.githubusercontent.com/pytgcalls/ntgcalls/master/.github/images/simple.gif" width=150 alt="Simple Logo"/><br>Simple Python, GO and C Bindings<br> | <img src="https://raw.githubusercontent.com/pytgcalls/ntgcalls/master/.github/images/light.gif" width=150 alt="Light logo"/><br>We removed anything that could burden the library, including <b>NodeJS</b> |

...and more, **without even rewriting your code that uses PyTgCalls!**

## Overview
NativeTgCalls, often referred to as NTgCalls, is an exciting open-source venture. This project redefines the 
Telegram calling experience and introduces innovative features while preserving the legacy of PyTgCalls.

## Build Status
| Architecture |                                Windows                                 |                              Linux                              |                                 MacOS                                  |
|:------------:|:----------------------------------------------------------------------:|:---------------------------------------------------------------:|:----------------------------------------------------------------------:|
|    x86_64    |    ![BUILD](https://img.shields.io/badge/build-passing-dark_green)     | ![BUILD](https://img.shields.io/badge/build-passing-dark_green) | ![BUILD](https://img.shields.io/badge/build-not%20supported-lightgrey) |
|    ARM64     | ![BUILD](https://img.shields.io/badge/build-not%20supported-lightgrey) |    ![BUILD](https://img.shields.io/badge/build-failing-red)     |    ![BUILD](https://img.shields.io/badge/build-passing-dark_green)     |

## Key Features

### Experimentation and Customization
NativeTgCalls is a playground for experimentation. We understand the importance of customization for developers, 
empowering you to tailor your Telegram calling solutions to your precise requirements.

### Advancing with PyTgCalls
Building upon the success of PyTgCalls, NativeTgCalls provides a natural progression for developers familiar with its predecessor. 
This evolution streamlines the development process and extends the capabilities of Telegram calling.

<i>Importantly, PyTgCalls will seamlessly integrate the new core (NTgCalls) to ensure backward compatibility.</i>

## Compiling

### Python Bindings
NativeTgCalls offers Py Bindings, enabling seamless integration with Python. Follow these steps to compile NativeTgCalls with Python Bindings:
1. Ensure you are in the root directory of the NativeTgCalls project.
2. Run the following command to install the Py Bindings:

   ```shell
   python3 setup.py install
   ```
### C Bindings
For developers looking to use NativeTgCalls with C and C++, we provide C Bindings. Follow these steps to compile NativeTgCalls with C Bindings:
1. Ensure you are in the root directory of the NativeTgCalls project.
2. Run the following command to generate the shared libs:
   ```shell
   python3 setup.py build_shared
   ```
3. Upon successful execution, a shared library will be generated in the "shared-output" directory. 
   You can now use this library to develop applications with NativeTgCalls.
4. To include the necessary headers in your C/C++ projects, you will find the "include" folder in the root of the project. 
   Utilize this folder for including the required header files.

## Key Contributors
* <b><a href="https://github.com/Laky-64">@Laky-64</a> (DevOps Engineer, Software Architect, Porting Engineer):</b>
    * Played a crucial role in developing NTgCalls.
    * Created the Python Bindings that made the library accessible to Python developers.
    * Developed the C Bindings, enabling the library's use in various environments.
* <b><a href="https://github.com/dadadani">@dadadani</a> (Senior C++ Developer, Performance engineer):</b>
    * Contributed to setting up CMakeLists and integrating with pybind11,
      greatly simplifying the library's usage for C++ developers.
* <b><a href="https://github.com/kuogi">@kuogi</a> (Senior UI/UX designer, Documenter):</b>
    * As a Senior UI/UX Designer, Kuogi has significantly improved the user interface of our documentation, 
      making it more visually appealing and user-friendly.
    * Has also played a key role in writing and structuring our documentation, ensuring that it is clear,
      informative, and accessible to all users.
* <b><a href="https://github.com/vrumger">@vrumger</a> (Mid-level NodeJS Developer):</b>
    * Avrumy has made important fixes and enhancements to the WebRTC component of the library, 
      improving its stability and performance.

## Junior Developers
* <b><a href="https://github.com/TuriOG">@TuriOG</a> (Junior Python Developer):</b>
    * Currently working on integrating NTgCalls into <a href="//github.com/pytgcalls/pytgcalls">PyTgCalls</a>, an important step
      in expanding the functionality and usability of the library.
* <b><a href="https://github.com/null-nick">@null-nick</a> (Junior Go Developer, Tester):</b>
    * Performs testing of NTgCalls on macOS to ensure its reliability and compatibility.
    * Is in the process of integrating NTgCalls into a Go wrapper, further enhancing the library's
      versatility and accessibility.
* <b><a href="https://github.com/tappo03">@tappo03</a> (Junior Go Developer, Tester):</b>
    * Performs testing of NTgCalls on Windows to ensure its reliability and compatibility.
    * Is in the process of integrating NTgCalls into a Go wrapper, further enhancing the library's
      versatility and accessibility.

## Special Thanks
* <b><a href="https://github.com/shiguredo">@shiguredo</a>:</b>
  We extend our special thanks to 時雨堂 (shiguredo) for their invaluable assistance in integrating the WebRTC component. Their contributions,
  utilizing the Sora C++ SDK, have been instrumental in enhancing the functionality of our library.
* <b><a href="https://github.com/evgeny-nadymov">@evgeny-nadymov</a>:</b>
  A heartfelt thank you to Evgeny Nadymov for graciously allowing us to use their code from telegram-react.
  His contribution has been pivotal to the success of this project.

* <b><a href="https://github.com/morethanwords">@morethanwords</a>:</b>
  We extend our special thanks to morethanwords for their invaluable help in integrating the connection to WebRTC with Telegram Web K. 
  Their assistance has been instrumental in enhancing the functionality of our library.
