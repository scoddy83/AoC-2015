#include <fstream>
#include <iostream>
#include <string>
#include <vector>

struct point {
  int x;
  int y;
};

point move(char direction) {
  point p = {0, 0};
  switch (direction)
  {
  case '^':
    return p = {0,1};
  case '>':
    return p = {1,0};
  case 'v':
    return p = {0,-1};
  case '<':
    return p = {-1,0};
  }
  return p;
}

int main() {
  int sum = 0;
  std::string str;
  point p = {0, 0};
  std::vector<point> path;
  int i = 0;

  // Read from the text file
  std::ifstream MyReadFile("../input.txt");

  // Start Point
  path.push_back(p);
  std::cout << "move: " << i << "; x: " << path[i].x << "; y: " << path[i].y << std::endl;
  // Use a while loop together with the getline() function to read the file line by line
  while (std::getline(MyReadFile, str)) {
    // Output the text from the file
    try {
      while(str[i]) {
        // Get Current Position
        p.x = path[i].x;
        p.y = path[i].y;
        // Set New Position
        p.x += move(str[i]).x;
        p.y += move(str[i]).y;
        
        path.push_back(p);
        std::cout << "move: " << i << "; x: " << path[i+1].x << "; y: " << path[i+1].y << std::endl;
        i++;
      }
    } catch (const std::invalid_argument&) {
      std::cout << "invalid_argument: " << std::endl;
    }
  }

  // Close the file
  MyReadFile.close();
  
  std::cout << "summe: " << sum << '\n';
  return 0;
}
