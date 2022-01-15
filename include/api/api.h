/**
 * @file api.h
 * @author Zeid Kootbally (zeidk@umd.edu)
 * @brief This file is copied from the example downloaded from github
 *
 *This file consists of all the methods to interact with the simulator
 * @version 0.1
 * @date 2021-10-24
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#pragma once

#include <string>

class API {

public:

    static int mazeWidth();
    static int mazeHeight();

    static bool wallFront();
    static bool wallRight();
    static bool wallLeft();

    static void moveForward(int distance = 1);
    static void turnRight();
    static void turnLeft();

    static void setWall(int x, int y, char direction);
    static void clearWall(int x, int y, char direction);

    static void setColor(int x, int y, char color);
    static void clearColor(int x, int y);
    static void clearAllColor();

    static void setText(int x, int y, const std::string& text);
    static void clearText(int x, int y);
    static void clearAllText();

    static bool wasReset();
    static void ackReset();

};