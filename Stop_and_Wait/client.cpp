#include "client.h"
using namespace std;
using namespace std::chrono;
int main(int argc, char **argv) {

  // Picking
  int option;
  bool hasPicked = false;
  
  // Statistics
  int totalPacketSize = 0;
  int totalTime = 0;
  int numAcks = 0;
  double rtt = 0.0;
  int throughput = 0;
  int md5 = 0;

  // File
  string fileName;
  ifstream inputFile;

  // Input Parameters
  int packetSize;
  int maxSeqNum = 1;
  int clientWinSize = 1;
  int serverWinSize = 1;
  int numPackets = 0;
  int bodySize = 0;
  // Initial variables
  int currentAck = 0;
  int currentPacket = 0;

  std::cout << "[1] Use Defaults" << std::endl << "[2] Customize Options" << std::endl;
  
  while(!hasPicked) {
    try {
      std::cin >> option;
    } catch (int e) {
      printf("Invalid input type; you must use a number\n");
    }    
    hasPicked = true;
  }
  if (to_string(option) == "2") {
    hasPicked = false;
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
          std::cin >> bodySize; 
          while (bodySize < 1){
            std::cout << "Error: You must choose a number greater than 0" << std::endl;
            std::cin >> bodySize;
          }
        } catch (int e) {
          std::cout << "Invalid input for size of packet!" << std::endl;
        }
        hasPicked = true;
        break;
      case 2:
        std::cout << "Running Go Back N" << std::endl;
        std::cout << "Enter max sequence number:" << std::endl;
        try{
          std::cin >> maxSeqNum; 
          while (maxSeqNum < 1) {
            std::cout << "Error: Sequence number must be a positive value!" << std::endl << "Enter a valid sequence number:" << std::endl;
            std::cin >> maxSeqNum;
          }
        } catch (int e) {
          std::cout << "Invalid input for sequence number!" << std::endl;
        }
        std::cout << "Enter window size:" << std::endl;
        try{
          std::cin >> clientWinSize; 
          while (clientWinSize < 1 || clientWinSize > maxSeqNum) {
            std::cout << "Error: Window size must be a positive value!" << std::endl << "Enter a valid window size:" << std::endl;
            std::cin >> clientWinSize;
          }
        } catch (int e) {
          std::cout << "Invalid input for window size!" << std::endl;
        }       
        hasPicked = true;
        break;
      case 3:
        std::cout << "Running Selective Repeate" << std::endl;
        
        std::cout << "Enter max sequence number:" << std::endl;
        try{
          std::cin >> maxSeqNum; 
          while (maxSeqNum < 1) {
            std::cout << "Error: Sequence number must be a positive value!" << std::endl << "Enter a valid sequence number:" << std::endl;
            std::cin >> maxSeqNum;
          }
        } catch (int e) {
          std::cout << "Invalid input for sequence number!" << std::endl;
        }
        
        std::cout << "Enter client window size:" << std::endl;
        try{
          std::cin >> clientWinSize; 
          while (clientWinSize < 1 || clientWinSize > maxSeqNum) {
            std::cout << "Error: Window size must be a positive value!" << std::endl << "Enter a valid window size:" << std::endl;
            std::cin >> clientWinSize;
          }
        } catch (int e) {
          std::cout << "Invalid input for window size!" << std::endl;
        }
        
        std::cout << "Enter server window size:" << std::endl;
        try{
          std::cin >> serverWinSize; 
          while (serverWinSize < 1 || serverWinSize > maxSeqNum) {
            std::cout << "Error: Sequence number must be a positive value!" << std::endl << "Enter a valid window size:" << std::endl;
            std::cin >> serverWinSize;
          }
        } catch (int e) {
          std::cout << "Invalid input for server window size!" << std::endl;
        }

        hasPicked = true;
        break;
      default:
        printf("Choose a valid option\n");
      }
    }
  } else {
    std::cout << "Using default values" << std::endl;
    bodySize = 1024;
    maxSeqNum = 8;
  }
  
  
  int headerSize = 65;
  int packetLoss = -1;
  int packetDamage = -1;
  int ackLoss = -1;
  
  std::cout << "Force Errors?" <<std::endl
	    << "[0] No Errors" << std::endl 
	    << "[1] Packet Loss" << std::endl 
	    << "[2] Damaged Packet" << std::endl
	    << "[3] Ack Lost" << std::endl;
  
  hasPicked = false;
  while(!hasPicked) {
      try {
        std::cin >> option;
      } catch (int e) {
        printf("Invalid input type; you must use a number\n");
      }

    switch(option) {
		case 0:
			std::cout << "Continuing without errors." << std::endl;
			hasPicked = true;
			break;
		case 1:
			std::cout << "Losing Packet" << std::endl << "Enter packet number to lose: " << std::endl;
			try{
			  std::cin >> packetLoss; 
			  while (packetLoss < 1 || packetLoss > maxSeqNum){
				std::cout << "Error: You must choose a number greater than 0 but less than the max sequence number. Default is 8" << std::endl;
				std::cin >> packetLoss;
			  }
			} catch (int e) {
			  std::cout << "Invalid input for packet number! You did something seriously wrong." << std::endl;
			}
			hasPicked = true;
			break;
		case 2:
			std::cout << "Damaging Packet" << std::endl << "Enter packet number to damage: " << std::endl;
			try{
			  std::cin >> packetDamage; 
			  while (packetDamage < 1 || packetDamage > maxSeqNum){
				std::cout << "Error: You must choose a number greater than 0 but less than the max sequence number. Default sequence number is 8." << std::endl;
				std::cin >> packetDamage;
			  }
			} catch (int e) {
			  std::cout << "Invalid input for packet number! You did something seriously wrong." << std::endl;
			}
			hasPicked = true;
			break;
		case 3:
			std::cout << "Losing Ack" << std::endl << "Enter ack number to lose: " << std::endl;
			try{
			  std::cin >> packetDamage; 
			  while (packetDamage < 1 || packetDamage > maxSeqNum){
				std::cout << "Error: You must choose a number greater than 0 but less than the max sequence number. Default sequence number is 8." << std::endl;
				std::cin >> packetDamage;
			  }
			} catch (int e) {
			  std::cout << "Invalid input for packet number! You did something seriously wrong." << std::endl;
			}
			hasPicked = true;
			break;
		default: 
			std::cout << "Choose a valid option!" << std::endl;
	}
  }
  
  
  packetSize = bodySize+headerSize;
  // Get file location
  hasPicked = false;
  std::cout << "Input file name:" << std::endl;
  while(!hasPicked) {
    try {
      std::cin >> fileName;  
      inputFile.open(fileName.c_str(), std::ios::in | std::ios::binary); 
      if(inputFile){
        hasPicked = true;
      } else {
        std::cout << "Invalid file name. Try again!" << std::endl;
      }
    } catch (int e) {
      std::cout << "Error opening file. Try Again!" << std::endl;
    }
  }
  

  // Size of file to be send and number of packets 
  inputFile.seekg(0, inputFile.end);
  int fileSize = inputFile.tellg();
  inputFile.seekg(0, inputFile.beg);
  std::cout << "Size of file to send: " << fileSize << std::endl;

  // Socket setup
  int sockfd;
  struct sockaddr_in serv_addr;
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0) {
    fprintf(stderr, "ERROR opening socket\n");
    exit(1);
  }
  int portno = atoi(argv[1]);
  memset(&serv_addr, '0', sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(portno);

  try{
    struct hostent *server;
    server = gethostbyname("thing3.cs.uwec.edu");

    if(server == NULL) {
      std::cout << "ERROR, no such host!" << std::endl;
      exit(1);
    }
    memcpy(&serv_addr.sin_addr.s_addr, server->h_addr, server->h_length);
    unsigned int seconds = 3;
    while (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
      //std::cout << "Error connecting to " << serv_addr << " on socket " << sockfd << std::endl;
      sleep(seconds); 
    }
  }catch (const std::exception &exc){
    // catch anything thrown within try block that derives from std::exception
    std::cout << "Error connecting to server" << std::endl;
    std::cerr << exc.what();
  }
  auto start = high_resolution_clock::now();

  // ****************************** Sending file size ******************************
  int received = 0;
  int sent = 0;
  
  numPackets = (fileSize+bodySize-1) / bodySize;
  int convertedFileSize = htonl(fileSize);
  // Send the file size 
  std::cout << "Sending file size: " << fileSize << std::endl;
  sent = send(sockfd, &convertedFileSize, sizeof(int), 0);
  if (sent < 0) {
    perror("Error sending file size");
    exit(0);
  }
  
  // ****************************** Sending body size ******************************
  int convertedBodySize = htonl(bodySize);
  // Send the packet size 
  std::cout << "Sending body size: " << bodySize << std::endl;
  sent = send(sockfd, &convertedBodySize, sizeof(int), 0);
  if (sent < 0) {
    perror("Error sending body size");
    exit(0);
  }

  // ****************************** Sending sequence number range ******************************
  int convertedMaxSequence = htonl(maxSeqNum);
  // Send the max sequence 
  std::cout << "Sending max sequence: " << maxSeqNum << std::endl;
  sent = send(sockfd, &convertedMaxSequence, sizeof(int), 0);
  if (sent < 0) {
    perror("Error sending max sequence");
    exit(0);
  }

  // Sending Lost Ack, always sends but might be -1
  int convertedAckLoss = htonl(ackLoss);
  std::cout << "Sending ack to lose: " << ackLoss << std::endl;
  sent = send(sockfd, &convertedAckLoss, sizeof(int), 0);
  if (sent < 0) {
    perror("Error sending lost ack number");
    exit(0);
  }
  
  int lastBodySize = fileSize % bodySize;
  if (lastBodySize == 0) {
    lastBodySize = bodySize;
  }
  
  int seqSize = 33;
  int ackSize = 33;
  char ack[ackSize]; // Only contains sequence number right now
  
  // Used for sequence numbers
  std::string binSeqNum;
  char* charSeqNum;
  int recSeqNum = -1;
  int expSeqNum = 0;
  int oldSeqNum = 0;
 
  bool getNextData = true;
  char * buff = new char[bodySize];
  int dataGot = 0;
  int bitsLeft = packetSize;
  int totalSent = 0;
  
  
  int startSeqNum = 0;
  int endSeqNum = clientWinSize;
  int tempEndSeqNum = endSeqNum;
  int curSeqNum = 0;
  char cache[maxSeqNum][bodySize];

  bool endGreaterThan = true;
  bool inRange = false;

  // ********************************** Begin Loop of receiving and sending information *********************
  while (currentPacket < numPackets) {

    // Loop through window
    while (curSeqNum != endSeqNum || currentPacket != numPackets) {

      if (currentPacket == numPackets - 1) {
        bodySize = lastBodySize;
        packetSize = headerSize + bodySize;
      }

      inputFile.read(buff, bodySize);
      
      // Convert sequence number to from int to char array
      std::bitset<33> bs (curSeqNum);
      binSeqNum = bs.to_string();
      charSeqNum = &binSeqNum[0u];
      
      // Get CRC
      std::cout << "Calculating CRC" << std::endl;
      boost::crc_32_type crc;
      
      crc.process_bytes(buff, bodySize);
      boost::uint32_t checksum = crc.checksum();
      std::cout << "Checksum: " << checksum << std::endl;
      std::cout << "uint32_t Checksum: " << checksum << std::endl;
      std::bitset<32> bits(checksum);
      std::cout << "Bits Checksum: " << bits.to_string() <<std::endl;

      // Add header information
      char dataToSend[packetSize];
      bzero(dataToSend, packetSize);
      strcat(dataToSend, charSeqNum);
      strcat(dataToSend, &(bits.to_string())[0u]);
      for (int index = 0; index < bodySize; index++) {
        dataToSend[65+index] = buff[index];
        cache[curSeqNum][index] = buff[index]; 
      }
      // std::cout << "Sending packet with sequence number: " << expSeqNum << std::endl;
      // std::cout << "Packet: " << dataToSend << std::endl;

      totalSent = 0;
      bitsLeft = packetSize;
      while (totalSent < packetSize) {
        sent = send(sockfd, &dataToSend + totalSent, bitsLeft, 0);
        if (sent < 0) {
          perror("Error sending packet size");
          exit(0);
        }
        // std::cout << "Sent: " << sent << " bits." << std::endl;
        totalSent += sent;
        bitsLeft -= sent;
      }
      std::cout << "Packet " << curSeqNum << " sent." << std::endl;
      
      // Reset for next send 
      bzero(buff, bodySize);
      bzero(dataToSend, packetSize);
      curSeqNum = (curSeqNum + 1) % maxSeqNum;
      currentPacket++;
    }
    
    // ********************************** End of Sending ********************

    // ********************************** Prep before receiving ********************
    curSeqNum = (curSeqNum - clientWinSize) % maxSeqNum;

    if (curSeqNum >= endSeqNum) {
      // Look for acks that are less end than but greater than current
      endGreaterThan = false;
    } else {
      // Look for acks that are greater than exp but less than old
      endGreaterThan = true;
    }

    // ******************************** Receiving Ack *********************
    //TODO CHANGE PACKETSIZE TO JUST BE THE ACK

    while (curSeqNum != endSeqNum || currentPacket != numPackets) {
      received = recv(sockfd, ack, ackSize, 0);
      if(received < 0){
        perror("Error receieving data.\n");
      } else if (received == 0) {
        std::cout << "Socket closed while receiving Ack from server!" << std::endl;
        close(sockfd);
        exit(1);
      }

      // std::cout << "Using STOI on: " << ack << std::endl;
      recSeqNum = std::stoi(ack, nullptr, 2);
      std::cout << "Ack " << recSeqNum << " received." << std::endl << std::endl;

      // Make sure ack is for correct packet
      // Check if in range of oldSeqNum / expSeqNum
      if (endGreaterThan) {
        if(recSeqNum <= endSeqNum || recSeqNum >= curSeqNum) {
         inRange = true; 
        } else {
          inRange = false;
        }
      } else {
        if(recSeqNum >= endSeqNum || recSeqNum <= curSeqNum) {
          inRange = true;
        } else {
          inRange = false;
        }
      }

      if (inRange) {
        // Correct ack
        // Move Window if recSeqNum is curSeqNumber 
        if (recSeqNum == curSeqNum) {
          tempEndSeqNum = (tempEndSeqNum + 1) % maxSeqNum;
          // Print window
          curSeqNum = (curSeqNum + 1) % maxSeqNum;
          std::cout << "Current Window = [";
          for (int index = curSeqNum; index != tempEndSeqNum; index + 1 % maxSeqNum) {
            std::cout << index;
            if (index + 1 != tempEndSeqNum) {
              std::cout << ", ";
            }
          }
          std::cout << "]" << std::endl;
        }

        // Clear Cache of recSeqNum
        bzero(cache[recSeqNum], bodySize);
        expSeqNum++;
        if (expSeqNum == maxSeqNum) {
          expSeqNum = 0;
        }
        bzero(ack, ackSize);
      } else {
      // Not correct ack, send packet again
        std::cout << "Incorrect ack, sending packet again" << std::endl;
        getNextData = false;
        sleep(1); 
      }

      currentPacket++;
    }
    endSeqNum = tempEndSeqNum;
  }
  
  std::cout << "Finished!" << std::endl;
  delete[] buff; 
  auto end = high_resolution_clock::now();
  auto elapsedTime = duration_cast<microseconds>(end-start);
  totalTime = elapsedTime.count();
  throughput = fileSize/(totalTime/1000);
  rtt = totalTime/currentPacket;
  
  // Print Information
  std::cout << "Total Packet Size: " << totalPacketSize << " bytes" << std::endl;
  std::cout << "Number of packets sent: " << currentPacket << std::endl;
  std::cout << "Total elapsed time: " << totalTime << std::endl; //time << 
  std::cout << "Throughput (Mbps): " << throughput << std::endl;
  std::cout << "Round trip time: " << rtt << std::endl;
  std::cout << "md5sum: " << md5 << std::endl;
  
  // Close and exit
  inputFile.close();
  close(sockfd);
  exit(1);
}
