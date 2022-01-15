#include <iostream>
#include <array>
#include "../include/api/api.h"

#include "../include/mouse/mouse.h"

void log(const std::string& text) {
std::cerr << text << std::endl;
}

int main() {
	rwa2::Mouse mouse;
	mouse.display_walls();
	std::pair<int,int>cur{0,0};
	API::setColor(8,8,'r');
	API::setText(8,8,"Goal");
	std:: cerr << mouse.search_maze()<<"\n";
	mouse.follow_path();
}