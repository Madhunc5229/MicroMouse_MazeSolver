#include "../include/mouse/mouse.h"
#include "../include/api/api.h"
#include "../include/util/util.h"
#include <string>
#include <algorithm>
#include <iostream>

void rwa2::Mouse::display_walls()
{
    for (int x = 0; x < m_maze_width; x += 1)
    {
        for (int y = 0; y < m_maze_height; y += 1)
        {
            if (m_maze.at(x).at(y).is_wall(direction::NORTH))
            {
                //display this wall in the simulator
                API::setWall(x, y, 'n');
            }

            if (m_maze.at(x).at(y).is_wall(direction::EAST))
            {
                //display this wall in the simulator
                API::setWall(x, y, 'e');
            }

            if (m_maze.at(x).at(y).is_wall(direction::SOUTH))
            {
                //display this wall in the simulator
                API::setWall(x, y, 's');
            }

            if (m_maze.at(x).at(y).is_wall(direction::WEST))
            {
                //display this wall in the simulator
                API::setWall(x, y, 'w');
            }
            //display the number of walls surrounding the current node
            API::setText(x, y, std::to_string(m_maze.at(x).at(y).compute_number_of_walls()));
        }
    }
}

void rwa2::Mouse::move_forward()
{   
    //make the mouse move forward in the simulator
    API::moveForward();
}

void rwa2::Mouse::turn_left()
{   
    //rotate the mouse 90 degrees CCW in the simulator
    API::turnLeft();
}

void rwa2::Mouse::turn_right()
{   
    //rotate the mouse 90 degrees CW in the simulator
    API::turnRight();
}

bool rwa2::Mouse::find_vector(std::vector<std::pair<int, int>> vect, std::pair<int, int> src)
{   
    //checks if each element in the vector is equal and returns a boolean in accordance to that
    for (int i = 0; i < vect.size(); i++)
    {
        if (vect.at(i)==src)
        {
            return true;
        }
    }
    return false;
}

bool rwa2::Mouse::change_direction(std::pair<int, int> current_node, std::pair<int, int> next_node)
{
    //checks if the direction change prescribe is to the North by comparing the current and next node values
    if (current_node.first == next_node.first && current_node.second < next_node.second && (!m_maze[current_node.first][current_node.second].is_wall(NORTH)))
    {

        m_new_direction = 0;
        return true;
    }
    //checks if the direction change prescribe is to the East by comparing the current and next node values
    else if (current_node.first < next_node.first && current_node.second == next_node.second && (!m_maze[current_node.first][current_node.second].is_wall(EAST)))
    {
        m_new_direction = 1;
        return true;
    }
    //checks if the direction change prescribe is to the South by comparing the current and next node values
    else if (current_node.first == next_node.first && current_node.second > next_node.second && (!m_maze[current_node.first][current_node.second].is_wall(SOUTH)))
    {
        m_new_direction = 2;
        return true;
    }
    //checks if the direction change prescribe is to the West by comparing the current and next node values
    else if (current_node.first > next_node.first && current_node.second == next_node.second && (!m_maze[current_node.first][current_node.second].is_wall(WEST)))
    {
        m_new_direction = 3;
        return true;
    }
    else
    {
    //returns false if there is no change in direction needed
        return false;
    }
}

//DFS algorithm for computing the path that is to be followed by the mouse
bool rwa2::Mouse::search_maze()
{   
    //First checks if the current node itself is the goal node or not
    if (m_cords != m_goal_node)
    {
        if (m_stk.empty())
        {
            //pushing the first node into the stack
            m_stk.push(m_cords);
        }
    }
    else
    {
        //if the current node is the goal node, set the goal node color to green and return true
        API::setColor(m_goal_node.first, m_goal_node.second,'g');
        return true;
    }
    if ((!find_vector(m_visited, m_cords)))
    {   
        //make a list of unique visited nodes
        m_visited.push_back(m_cords);
    }
    if ((!m_maze[m_cords.first][m_cords.second].is_wall(0)) && (!find_vector(m_visited, {m_cords.first, m_cords.second + 1})))
    {
        //if there is no wall to the North, push the node to the stack and update the current node values
        API::setColor(m_x, m_y, 'w'); //set the colour of the visited node to white
        m_y += 1; //increment the 'x' coordinate
        m_cords.second = m_y;
        m_stk.push(m_cords);
        m_backtracking_counter = 0; //backtracking counter at zero still
    }
    else if ((!m_maze[m_cords.first][m_cords.second].is_wall(1)) && (!find_vector(m_visited, {m_cords.first + 1, m_cords.second})))
    {
        //if there is no wall to the East, push the node to the stack and update the current node values
        API::setColor(m_x, m_y, 'w');
        m_x += 1;
        m_cords.first = m_x;
        m_stk.push(m_cords);
        m_backtracking_counter = 0;
    }
    else if ((!m_maze[m_cords.first][m_cords.second].is_wall(2)) && (!find_vector(m_visited, {m_cords.first, m_cords.second - 1})))
    {
        //if there is no wall to the South, push the node to the stack and update the current node values
        API::setColor(m_x, m_y, 'w');
        m_y -= 1;
        m_cords.second = m_y;
        m_stk.push(m_cords); 
        m_backtracking_counter = 0;
    }
    else if ((!m_maze[m_cords.first][m_cords.second].is_wall(3)) && (!find_vector(m_visited, {m_cords.first - 1, m_cords.second})))
    {
        //if there is no wall to the West, push the node to the stack and update the current node values
        API::setColor(m_x, m_y, 'w');
        m_x -= 1;
        m_cords.first = m_x;
        m_stk.push(m_cords);
        m_backtracking_counter = 0;
    }
    else
    {   
        //this is the case where backtracking is required (no further mouse movement possible)
        if (!m_stk.empty())
        {
            //if the current stack is empty, change the current node to the previous one on the visted list
            m_cords = m_visited[(m_visited.size() - 2 - m_backtracking_counter)];
            m_backtracking_counter++;
            //change the current location
            m_x = m_cords.first;
            m_y = m_cords.second;
            //push the visited node to the stack and search the maze again for a path
            m_stk.push(m_cords);
            search_maze();
        }
        else
        {
            return false;
        }
    }

    if (!m_stk.empty())
    {   
        //pop the node on the top in case a path doesn't exist from that point and retry the algorithm
        m_cords = m_stk.top();
        search_maze();
    }
    else
    {
        return false;
    }
    //return false if DFS cannot find any path
    return false;
}

void rwa2::Mouse::follow_path()
{   
    //to make the mouse move along the path the nodes in the stack should be accessed bottom-up
    reverse_stack(m_stk);
    while (!(m_stk.top() == m_goal_node)) //continuously check if the top of the stack is the goal node
    {   
        //check for walls whilst traversing
        check_walls();
        std::pair<int, int> current_node = m_stk.top(); 
        m_stk.pop(); //assign the current node and the next node 
        std::pair<int, int> next_node = m_stk.top();
        if (path_feasibilty(current_node, next_node)) //traverse from the current node to the next if the movement is possible
        {
            if (change_direction(current_node, next_node)) //checks if a direction change has to be made whilst moving
            {
                if (m_direction!=m_new_direction){ //
                    mouse_move();
                    m_direction= m_new_direction; //update the mouse direction after moving
                }
                move_forward();
            }

        }
        else 
            {   
            //if movement along the given path is not possible, clear the stack and the visited list, and assign the current node values for the DFS to recompute the path from
                empty_my_stack();
                m_visited.clear();
                m_cords.first=current_node.first;
                m_cords.second=current_node.second;
                m_x=current_node.first;
                m_y=current_node.second;
                search_maze();
                follow_path();
            }
        
    }
    
}

void rwa2::Mouse::mouse_move()
{   
    //based on the value of the new direction, move the mouse on the simulator
    int n = m_new_direction - m_direction;
    switch (n) //using case-switch to change the direction based on the value of the new direction
    {
    case 1:
    case -3:
        turn_right();
        break;
    case 2:
    case -2:
        turn_right();
        turn_right();
        break;
    case 3:
    case -1:
        turn_left();
        break;
    default:
        break;
    }
}

void rwa2::Mouse::empty_my_stack()
{   
    //emptying the stack to compute a new path (continuously 'pop' the elements out!)
    while (!m_stk.empty())
    {
        API::setColor(m_stk.top().first, m_stk.top().second, 'k');
        m_stk.pop();
    }
}

void rwa2::Mouse::check_walls()
{   
    //assigning key-value pairs for each direction to take into account, the integer value of the new direction
    std::vector<std::pair<char, int>> current_dir = {{'n', 0}, {'e', 1}, {'s', 2}, {'w', 3}};
    std::vector<std::pair<char, int>> right_dir = {{'e', 1}, {'s', 2}, {'w', 3}, {'n', 0}};
    std::vector<std::pair<char, int>> left_dir = {{'w', 3}, {'n', 0}, {'e', 1}, {'s', 2}};


    if (API::wallFront())
    {   
        //if a wall is found to the front, first update the maze and then set the wall in the maze
        m_maze[m_stk.top().first][m_stk.top().second].set_wall(current_dir[m_new_direction].second, true);
        API::setWall(m_stk.top().first, m_stk.top().second, current_dir[m_new_direction].first);
    }
    if (API::wallRight())
    {   
        //if a wall is found to the right, first update the maze and then set the wall in the maze
        m_maze[m_stk.top().first][m_stk.top().second].set_wall(right_dir[m_new_direction].second, true);
        API::setWall(m_stk.top().first, m_stk.top().second, right_dir[m_new_direction].first);
    }
    if (API::wallLeft())
    {   
        //if a wall is found to the left, first update the maze and then set the wall in the maze
        m_maze[m_stk.top().first][m_stk.top().second].set_wall(left_dir[m_new_direction].second, true);
        API::setWall(m_stk.top().first, m_stk.top().second, left_dir[m_new_direction].first);
    }
}

void rwa2::Mouse::reverse_stack(std::stack<std::pair<int, int>> &stk)
{   
    //invert the stack by going through each individual element
    std::stack<std::pair<int, int>> reverse_stk;
    std::pair<int, int> temp_value;
    while (stk.size())
    {
        temp_value = stk.top();
        stk.pop();
        reverse_stk.push(temp_value);
    }
    stk = reverse_stk;
}

bool rwa2::Mouse::path_feasibilty(std::pair<int, int> current_node, std::pair<int, int> next_node)
{   
    //if there is no wall to the North and the computed succeeding node is to the North, return True
    if ((current_node.first == next_node.first) && (current_node.second<next_node.second) && (!m_maze[current_node.first][current_node.second].is_wall(NORTH)))
    {
        return true;
    }
    //if there is no wall to the East and the computed succeeding node is to the East, return True
    else if (current_node.first < next_node.first && current_node.second == next_node.second && (!m_maze[current_node.first][current_node.second].is_wall(EAST)))
    {
        return true;
    }
    //if there is no wall to the South and the computed succeeding node is to the South, return True
    else if (current_node.first == next_node.first && current_node.second > next_node.second && (!m_maze[current_node.first][current_node.second].is_wall(SOUTH)))
    {
        return true;
    }
    //if there is no wall to the West and the computed succeeding node is to the West, return True
    else if (current_node.first > next_node.first && current_node.second == next_node.second && (!m_maze[current_node.first][current_node.second].is_wall(WEST)))
    {
        return true;
    }
    else
    //if the aforementioned is not the case, just return false!
    {
        return false;
    }
}

