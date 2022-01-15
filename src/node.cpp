#include "../include/node/node.h"  
#include "../include/util/util.h"
#include "../include/api/api.h"
#include <iostream>

// #include "../include/mouse/mouse.h"

void rwa2::Node::print_m_walls(){
    for(int i=0; i<4; i++){
        std::cerr<< "printing m_walls of current node: " << m_walls[i] << std::endl;
        }
    }

void rwa2::Node::set_wall(int direction, bool is_wall) {

    // for(int i=0; i<4; i++){
    //     std::cerr << m_walls.at(i)<< std::endl;
    // }
    m_walls.at(direction) = is_wall;

}

bool rwa2::Node::is_wall(int direction) const {   
    return m_walls.at(direction);
}

int rwa2::Node::compute_number_of_walls() const {
    int weight{ 0 };
    int weight_n{ 0 };
    int weight_s{ 0 };
    int weight_e{ 0 };
    int weight_w{ 0 };
    if (is_wall(direction::NORTH))
        weight_n = 1;
    if (is_wall(direction::EAST))
        weight_s = 1;
    if (is_wall(direction::SOUTH))
        weight_e = 1;
    if (is_wall(direction::WEST))
        weight_w = 1;
    return weight + weight_n + weight_s + weight_e + weight_w;
}