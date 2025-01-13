# UI

Standalone UI Library for C.

This library is written using the [stb-style single header only style<sup>1</sup>](https://github.com/nothings/stb?tab=readme-ov-file#how-do-i-use-these-libraries), meaning the whole library is just one header file.

This library doesn't provide any way of drawing to a window, let alone create a window; Just the logics of an [Immediate-mode UI<sup>2</sup>](https://en.wikipedia.org/wiki/Immediate_mode_(computer_graphics)#Immediate\_mode\_GUI).
You must initialize the window and renderer yourself, then just provide the callbacks to draw basic stuff to this library.


## References
1 - https://github.com/nothings/stb?tab=readme-ov-file#how-do-i-use-these-libraries
2 - https://en.wikipedia.org/wiki/Immediate_mode_(computer_graphics)#Immediate\_mode\_GUI
