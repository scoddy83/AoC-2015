#include <fstream>
#include <iostream>
#include <string>
#include <array>
#include <algorithm>

void find_str(std::string s, std::string del, std::array<int, 3> &numbers) {
    // Use find function to find 1st position of delimiter.
    int i = 0;
    int end = s.find(del); 
    while (end != -1) { // Loop until no delimiter is left in the string.
        numbers[i] = std::stoi(s.substr(0, end));
        s.erase(s.begin(), s.begin() + end + 1);
        end = s.find(del);
        i++;
    }
    numbers[i] = std::stoi(s.substr(0, end));
}

int calcSqareFeet(std::array<int, 3> numbers) {
  int sqareFeet = 0;
  std::array<int, 3> spare;
  sqareFeet = numbers[0] * numbers[1] * 2 + numbers[1] * numbers[2] * 2 + numbers[0] * numbers[2] * 2;
  spare[0] = numbers[0] * numbers[1];
  spare[1] = numbers[1] * numbers[2];
  spare[2] = numbers[0] * numbers[2];
  std::sort(spare.begin(), spare.end());
  return sqareFeet + spare[0];
}

int main() {
  int sum = 0;
  std::string str;
  std::array<int, 3> numbers;
  // Read from the text file
  std::ifstream MyReadFile("../input.txt");

  // Use a while loop together with the getline() function to read the file line by line
  while (std::getline(MyReadFile, str)) {
    // Output the text from the file
    try {
      find_str(str, "x", numbers);
      sum += calcSqareFeet(numbers);
    } catch (const std::invalid_argument&) {
      std::cout << "invalid_argument: " << std::endl;
    }
  }

  // Close the file
  MyReadFile.close();
  
  std::cout << "summe: " << sum << '\n';
  return 0;
}
