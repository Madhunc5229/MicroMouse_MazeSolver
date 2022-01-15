/**
 * @file node.h
 * @author Zeid Kootbally (zeidk@umd.edu)
 * @brief This file contains a class to represent a node in a maze.
 * @version 0.1
 * @date 2021-10-24
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef NODE_H
#define NODE_H

#include <array>

namespace rwa2 {
    /**
     * @brief Class to represent a node (cell) in a maze.
     *
     *A node is just a space delimited by 4 walls
     *
     */
    class Node
    {
    public:
        Node() {
            for (int i = 0; i < 4; i += 1) {
                m_walls[i] = false;
            }
        }
        /**
         * @brief Set the walls of a cell
         *
         * @param direction NORTH, EAST, SOUTH, or WEST
         * @param is_wall true if there is a wall, otherwise false
         */
        void set_wall(int direction, bool is_wall);
        /**
         * @brief Return whether or not there is a wall in a cell
         *
         * @param direction Direction to set for wall (NORTH, EAST, SOUTH, or WEST)
         * @return true if there is a wall in the given direction in the cell
         * @return false if there is no wall in the given direction in the cell
         */
        bool is_wall(int direction) const ;

        /**
         * @brief Compute the number of walls surrounding a node
         * 
         * @return int Number of walls surrounding a node
         */
        int compute_number_of_walls() const;
        
    private:
        std::array<bool, 4> m_walls; // stores boolean value of whether or not there is a wall in 4 directions

    };
}
#endif