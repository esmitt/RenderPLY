# Loading PLY files using GLFW

This is a project uses [GLFW], [GLEW] and the math library [GLM] to load [PLY] files. The project was generated over Visual Studio 2017. The project contains the basic draw, keyboard, initialize and resize function over GLFW and the usage of vertex and fragment shader. 
Code contains the definition for the Visual Studio compiler

    #pragma comment(lib, "glfw3dll.lib")
    #pragma comment(lib, "opengl32.lib")
    #pragma comment(lib, "glew32.lib")
    #pragma comment(lib, "opengl32.lib")
at beginning of code.

Also, the structure is using a `namespace` called glfw which contains all the variables. A class `CArcBall` is included which controls the movement of objects using the mouse. The PLY files are loadad from `geometry` folder. A basic shader pass-away is used.

Functions and variables related to GLFW are encapsulated into a namespace called `glfwFunc` (nothing special about the name). The code include all the libraries (yes, I know that is not efficient) to be used directly without download any additional library (based on the mentioned platform/libraries).

[GLFW]:http://www.glfw.org/
[GLEW]:http://glew.sourceforge.net/
[GLM]:http://glm.g-truc.net/
[PLY]:https://en.wikipedia.org/wiki/PLY_(file_format)

> For Visual Studio 2017, you need to add the _CRT_SECURE_NO_WARNINGS in Project Configuration -> C/C++ -> Preprocessor -> Preprocessor Definitions

###### If you want to contribute to this project and make it better, your help is very welcome.