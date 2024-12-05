#include <fstream>
#include <iostream>
#include <string>

int main() {
  int floor = 0;
  int i = 0;
  std::string str;
  // Read from the text file
  std::ifstream MyReadFile("../input.txt");

  // Use a while loop together with the getline() function to read the file line by line
  while (std::getline(MyReadFile, str)) {
    // Output the text from the file
    try {
      while(str[i]) {
        if(str[i] == '(')
          floor++;
        if(str[i] == ')')  
          floor--;
        i++;
      }
    } catch (const std::invalid_argument&) {
      std::cout << "invalid_argument: " << std::endl;
    }
  }

  // Close the file
  MyReadFile.close();
  
  std::cout << "Floor: " << floor << '\n';
  return 0;
}