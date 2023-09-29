# Tetris
C++ and SFML, Tetris. Figuring out how animations work was fun. Procedural was forced.

# Running it on Linux
Extract the repo into any directory, and run the following commands in a terminal:

g++ -c main.cpp

g++ main.o -o game -lsfml-graphics -lsfml-audio -lsfml-window -lsfml-system

./game
