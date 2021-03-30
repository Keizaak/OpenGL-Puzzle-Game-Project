Puzzle Game - OpenGL
===================
- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
# Instructions
## Requirements
* Cmake 2.8
* OpenGL 3.2
* GLFW 3.0
## Build
The program can be compiled by running
```bash
cd path-to-project
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j 4
```
## Run
The program can be executed without any optional argument, as `./puzzle`.
- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
# Game overview
## Description
The aim is to solve a 2D puzzle with a shape to reproduce and a set of pieces.
## Gameplay
The player has a geometrical shape to reproduce by using a set of pieces that will be given and that they will have
to assemble correctly
## Controls
* ZQSD/WASD or Directional Arrows: move current piece
* A/Q, E or RIGHT_SHIFT, 1 (Numeric keypad): rotate current piece
* O, P: change current piece
## Demo
![Demo](etc/demo.gif "Demo of the puzzle game")
- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
# Team
* Raphaël ANCETTE <raphael.ancette@ecole.ensicaen.fr>
* Camille GUIGNOL <camille.guignol@ecole.ensicaen.fr>
* Nathan MICHEL <nathan.michel@ecole.ensicaen.fr>
* Arnaud RIO <arnaud.rio@ecole.ensicaen.fr>
- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
# License
The code is published under the MIT License (MIT)

Copyright (c) 2018 Loic Simon

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -