/**
 * @file util.h
 * @author Zeid Kootbally (zeidk@umd.edu)
 * @brief Components used by multiple classes but not required to be a class member can be placed in this file
 * @version 0.1
 * @date 2021-10-24
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef UTIL_H
#define UTIL_H

 /*!
 * @brief Definition of an enum to store the different directions
 * 
 */

enum direction {
    NORTH = 0, /*!< North direction */
    EAST = 1, /*!< East direction */
    SOUTH = 2, /*!< South direction */
    WEST = 3 /*!< West direction */
};

#endif
