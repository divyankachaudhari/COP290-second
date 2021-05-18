#ifndef CREATE_MAZE_H
#define CREATE_MAZE_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <fstream>

using namespace std;

#define UP 0     //-y
#define DOWN 1   //+y
#define LEFT 2   //-x
#define RIGHT 3  //+x

#define nodeadend//generate a maze without any dead ends! (consequently, many solutions to maze)
// #define prim    //enable this to generate mazes using prim's algorithm.
#define backtrack//enable this to generate mazes using depth-first search. Don't enable both.

void initialize();
void generate();
void savetxt();

struct cell{
	bool in;  //Is this cell in the maze?
	bool up;  //Does the wall above this cell exist?
	bool left;//Does the wall to the left of this cell exist?
	int prevx, prevy; //The coordinates of the previous cell, used for backtracking.
};

void initialize();
void generate();
void savetxt();
void reflect();

#endif
