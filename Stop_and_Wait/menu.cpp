#include "menu.h"

int main(int argc, char **argv) {
  int option;
  bool hasPicked = false;
  string fileName;
  string line;
  int totalTime = 0;
  ifstream inputFile;
  long packetSize;
  long maxSequence;
  
  std::cout << "Choose a networking protocol:" << std::endl << "[1] Stop and Wait" << std::endl << "[2] Go Back N?" << std::endl << "[3] Selective Repeat?" << std::endl;
  // Loop until a valid option is input.
  while(!hasPicked) {
    try {
      std::cin >> option;
    } catch (int e) {
      printf("Invalid input type; you must use a number\n");
    }

  switch(option) {
    case 1:
      std::cout << "Running Stop and Wait" << std::endl << "Enter packet size in bits: " << std::endl;
      try{
        std::cin >> packetSize; 
        while (packetSize < 1){
          std::cout << "Error: You must choose a number greater than 0" << std::endl;
          std::cin >> packetSize;
        }
      } catch (int e) {
        std::cout << "Invalid input for size of packet!" << std::endl;
      }
      std::cout << "Enter max sequence number:" << std::endl;
      try{
        std::cin >> maxSequence; 
        while (maxSequence < 1) {
          std::cout << "Error: Sequence number must be a positive value!" << std::endl << "Enter a valid sequence number:" << std::endl;
          std::cin >> maxSequence;
        }
      } catch (int e) {
        std::cout << "Invalid input for sequence number!" << std::endl;
      } 
      hasPicked = true;
      break;
    case 2:
      std::cout << "Running Go Back N" << std::endl;
      hasPicked = true;
      break;
    case 3:
      std::cout << "Running Selective Repeate" << std::endl;
      hasPicked = true;
      break;
    default:
      printf("Choose a proper option\n");
    }
  }
  hasPicked = false;
  std::cout << "Input file name:" << std::endl;
  while(!hasPicked) {
    try {
      std::cin >> fileName;  
      inputFile.open(fileName.c_str(), ios:: in); 
      if(inputFile){
        hasPicked = true;
      } else {
        std::cout << "Invalid file name. Try again!" << std::endl;
      }
    } catch (int e) {
      std::cout << "Error opening file. Try Again!" << std::endl;
    }
  }

//-------------- Convert all process info (from file) to type Process  ------------------------

  int i;
  int value;
  while (std::getline(inputFile, line)) {
  // Do not look at header info
      stringstream ss(line);
      string substr;
      Process process;
      while(std::getline(ss, substr, '\t')) {
        try {
          value = stoi(substr);
        } catch (int e) {
          cout << substr << " is an invalid value\n";
        }
        // Remove commands with negative numbers besides the deadline for opiton 1 and 3 
        if(value < 0 && !(i == 4 && (option == 1 || option == 3))) {
          i = 0;
          numLines--;
          //cout << "Invalid process " << line << "\n";
          break;
        } /*else if(value < 0){
          cout << "Didn't remove " << line << "\n";
        }*/

        i++;
    }
       ++numLines;
  }
  numLines--;
  
  // Create new instance of client with variables retrieved from inputs or move this into client...
  exit(0);
}
