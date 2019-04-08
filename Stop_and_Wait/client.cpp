#include "client.h"
using namespace std;
int main(int argc, char **argv) {

  // Picking
  int option;
  bool hasPicked = false;
  
  // Statistics
  int totalPacketSize = 0;
  int totalTime = 0;
  int numAcks = 0;
  double rttTime = 0.0;
  int throughput = 0;
  int md5 = 0;

  // File
  string fileName;
  ifstream inputFile;

  // Input Parameters
  int packetSize;
  int maxSequence;
  int numPackets = 0;
  int sequenceNumber = 0;

  // Initial variables
  int currentAck = 0;

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
        printf("Choose a valid option\n");
      }
    }
  } else {
    std::cout << "Using default values" << std::endl;
    packetSize = 1024;
    maxSequence = 8;
  }
  
  // Get file location
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
  //TODO Generate header data
  char header[5] = "0000";


  // Size of file to be send and number of packets 
  inputFile.seekg(0, inputFile.end);
  int fileSize = inputFile.tellg();
  inputFile.seekg(0, inputFile.beg);
  std::cout << "Size of file: " << fileSize << std::endl;
  int packetsToSend = (fileSize / packetSize) + 1;
  std::cout << "Packets to send: " << packetsToSend << std::endl;
  // Get first set of data from file
  char *buff = new char[packetSize];
  int dataGot = 0;
  char c;
  while(dataGot < packetSize){
    c = inputFile.get();
    buff[dataGot] = c;
    dataGot++;
  }

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
    server = gethostbyname("thing1.cs.uwec.edu");

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

  // ****************************** Sending the total size to expect ******************************
  int received = 0;
  int sent = 0;
  int totalSent = 0;
  std::vector<char> nums; 
  // Size to send
  int len = nums.size() + 1;

  // Add header information
  char dataToSend[packetSize+4];
  strcat(dataToSend, header);
  strcat(dataToSend, buff);
  std::cout << "Sending: " << dataToSend << std::endl;
  // Send the amount the client should expect to receive  
  sent = send(sockfd, &buff, packetSize, 0);
  if (sent < 0) {
    perror("Error sending packet size");
    exit(0);
  }
 
  bool finishedSending = false;

  // ********************************** Begin Loop of receiving and sending information *********************

  while (!finishedSending) {
    // Clear buffer 
    bzero(buff,packetSize);
    
    // Send Packet
    


    // If timeout - resend packet ? Don't increase sequence #
    // Use total sequence # to get body for each packet
    
    

    // ******************************** Receiving Ack *********************
    received = 0;
    nums.clear();
      
    received += recv(sockfd, buff, packetSize, 0);
    if(received < 0){
      perror("Error receieving data.\n");
    } else if (received == 0) {
      std::cout << "Socket closed!" << std::endl;
      close(sockfd);
      exit(1);
    }
    std::cout << "Received ACK: " << buff[0] << std::endl;
    // Make sure ack is for correct packet
    if((int)buff[0] == currentAck){
    // Correct ack
      currentAck++;
    } else {
    // Not correct ack, send packet again
  
    }


    bzero(buff, len);
    // TODO Send next packet 
    std::cout << "Sending Packet with sequence number: " << std::endl; // Sequence Number
    //sent = send(sockfd, packet, sizeof(int), 0);

    totalSent = 0;

    char *buffer= new char[nums.size()];
    //bzero(buffer, len);
    std::copy(nums.begin(), nums.end(), buffer);
    int bytesLeft = len;
    
    // ******************************** Sending list of numbers to server *************************
    //printf("Sending numbers to server!\n");
    //sieve->printRemainingPrimes(nums, 0);
    while(totalSent < len) {
      //printf("Sending data, totalSent: %d\n", totalSent);
      sent = send(sockfd, buffer+totalSent, bytesLeft, 0);
      if(sent < 0){
        std::cout << "Error writing to socket" << std::endl;
      } else {
        totalSent += sent;
        bytesLeft -= sent;
      }
    }
    std::cout << std::endl << std::endl;
  }

  // Clean up
  delete[] buff;
  // Print Information
  std::cout << "Finished!" << std::endl;
  std::cout << "Total Packet Size: " << totalPacketSize << " bytes" << std::endl;
  std::cout << "Number of packets sent: " << numPackets << std::endl;
  std::cout << "Total elapsed time: " << std::endl; //time << 
  std::cout << "Throughput (Mbps): " << throughput << std::endl;
  std::cout << "md5sum: " << md5 << std::endl;
  
  // Close and exit
  close(sockfd);
  exit(1);
}
