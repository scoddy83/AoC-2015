#include <fstream>
#include <iostream>
#include <string>
#include <vector>

struct point {
  int x;
  int y;
  int parcel;
};

point move(char direction) {
  point p = {0, 0, 0};
  switch (direction)
  {
  case '^':
    return p = {0,1,0};
  case '>':
    return p = {1,0,0};
  case 'v':
    return p = {0,-1,0};
  case '<':
    return p = {-1,0,0};
  }
  return p;
}

bool addParcel(std::vector<point> path, point const *p) {
  for(std::size_t i = 0; i < path.size(); i++) {
    //std::cout << "move-i: " << i << "; x : y = " << path[i].x << " : " << path[i].y << std::endl;
    if(p->x == path[i].x && p->y == path[i].y) {
      path[i].parcel++;
      return true;
    }
  }
  return false;
}

int main() {
  int sum = 0;
  std::string str;
  point pSanta = {0, 0, 1};
  point pRobot = {0, 0, 1};
  point p = {0, 0, 2};
  std::vector<point> path;
  int i = 0;

  // Read from the text file
  std::ifstream MyReadFile("../input.txt");

  // Start Point
  path.push_back(p);
  // Use a while loop together with the getline() function to read the file line by line
  while (std::getline(MyReadFile, str)) {
    // Output the text from the file
    try {
      while(str[i]) {
        // Set New Position
        if(i%2 != 0) {
          pSanta.x += move(str[i]).x;
          pSanta.y += move(str[i]).y;
          p = pSanta;
        } else {
          pRobot.x += move(str[i]).x;
          pRobot.y += move(str[i]).y;
          p = pRobot;
        }
        
        //std::cout << "move: " << i << "; x : y = " << p.x << " : " << p.y << std::endl;
        if(!addParcel(path, &p)) {
          path.push_back(p);
        }
        //std::cout << "move: " << i << "; x : y = " << p.x << " : " << p.y << std::endl;
        i++;
      }
    } catch (const std::invalid_argument&) {
      std::cout << "invalid_argument: " << std::endl;
    }
  }

  // Close the file
  MyReadFile.close();

  sum = path.size();
  
  std::cout << "summe: " << sum << '\n';
  return 0;
}
