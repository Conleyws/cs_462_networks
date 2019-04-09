#include "server.h"

int main(int argc, char **argv) {
  if (argc < 1){
    printf("You are missing an argument\n arg[0] = portno\n");
    exit(1);
  }

  //get socket
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0) {
    printf("ERROR opening socket\n");
    exit(1);
  }


  int portno = atoi(argv[1]);
 
  struct sockaddr_in serv_addr;
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  serv_addr.sin_port = htons(portno);

  if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
    printf("ERROR on binding\n");
    exit(1);
  }

  listen(sockfd, 4);
  
  int newsockfd;
  struct sockaddr_in cli_addr;
  socklen_t clilen = sizeof(cli_addr);

  printf("Waiting for client... \n");
  newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
  printf("Connected\n");

  if (newsockfd < 0) {
    printf("ERROR on accpting");
    exit(1);
  }

  // Number of packets received in network byte order
  int fileSize = 0;
  int fileSizeReceived = 0;
  // Number of packets received in host byte order
  int numPackets = 0;
  // Status code upon receiving anything
  int received = 0;
  // Status code upon sending anything
  int sent = 0;

  // ********************************** Receiving file size to expect **************************
  received = recv(newsockfd, &fileSizeReceived, sizeof(int), 0);
  if (received < 0) {
    perror("Error receieving number of packets.");
  } else if (received == 0) {
    std::cout << "Socket closed on server end. Closing socket." << std::endl;
    close(sockfd);
    exit(1);
  } else {
    // Convert back to int
    fileSize=ntohl(fileSizeReceived);
    std::cout << "File Size: " << fileSize << std::endl;
  }
  
  // ********************************** Receiving Packet Body Size **************************
  int bodySizeReceived = 0;
  int bodySize = 0;
  
  received = recv(newsockfd, &bodySizeReceived, sizeof(int), 0);
  if (received < 0) {
    perror("Error receieving packet body size.");
  } else if (received == 0) {
    std::cout << "Socket closed on server end. Closing socket." << std::endl;
    close(sockfd);
    exit(1);
  } else {
    // Convert back to int
    bodySize=ntohl(bodySizeReceived);
    std::cout << "Total Packet Body Size: " << bodySize << std::endl;
  }

  // ********************************** Receiving Sequence Number Range **************************
  int recMaxSeqNum = 0;
  int maxSeqNum = 0;
  
  received = recv(newsockfd, &recMaxSeqNum, sizeof(int), 0);
  if (received < 0) {
    perror("Error receieving sequence number range.");
  } else if (received == 0) {
    std::cout << "Socket closed on server end. Closing socket." << std::endl;
    close(sockfd);
    exit(1);
  } else {
    // Convert back to int
    maxSeqNum=ntohl(recMaxSeqNum);
    std::cout << "Sequence Number Range: " << maxSeqNum << std::endl;
  }


  numPackets = (fileSize + bodySize - 1) / bodySize;
  std::cout << "Total number of packets: " << numPackets << std::endl;

  int totalSent = 0;
  bool finishedReceiving = false;
  std::vector<char> packet;
  int recSeqNum = 0;
  int expSeqNum = 0;
  int headerSize = 33;

  int packetSize = headerSize + bodySize;
  char buff[packetSize];
  char ack[headerSize];
  int currentPacket = 0;
  char binSeqNum[headerSize];
  char body[bodySize];

  int lastPacketSize = fileSize % bodySize;
  std::cout << "Standard Packet Size: " << packetSize << std::endl;
  std::cout << "Last Packet Size: " << lastPacketSize << std::endl;

  std::ofstream ofs("copy.txt", std::ofstream::out);
  
  // ********************************** Begin Loop of receiving packets and sending Acks ********************
  while(currentPacket < numPackets) {
    
  bzero(buff, packetSize); 
   
  //Loop
    // Read in packet
    // Check sequence #
    // Get packet body and put to file or buffer?
    //
    // Send back ACK with sequence #
  //End loop
  //Write to file?
    
    std::cout << "Expected Sequence Number: " << expSeqNum << std::endl;
    // ******************************** Receiving Packet ************************
    received = 0;

    received = recv(newsockfd, buff, packetSize, 0);
    if(received < 0){
      perror("Error receieving data.\n");
    } else if (received == 0) {
      printf("Socket closed while receiving packets from client!\n");
      close(newsockfd);
      exit(1);
    } else {
      std::cout << "Buff Length: " << strlen(buff) << std::endl;
      std::cout << "Received: " << buff << std::endl;
      //packet.insert(packet.end(), buff, buff + strlen(buff));
    }
    // Zero out the buff for next packet
    // bzero(buff, packetSize);
    // first four = sequence number
    
    strncpy(binSeqNum, buff, headerSize);
    binSeqNum[headerSize] = '\0';
    std::string strSeqNum(binSeqNum);
    strncpy(body, &buff[headerSize], bodySize);

    std::cout << "Using STOI on: " << strSeqNum << std::endl;
    recSeqNum = std::stoi(strSeqNum, nullptr, 2); 
    
    // Convert sequence number
    std::cout << "Packet: " << recSeqNum << " received." << std::endl; // Sequence number 
    bzero(buff, packetSize); 

    // Send ACK
    sent = send(newsockfd, &binSeqNum, headerSize, 0);
    if (sent < 0) {
      perror("Error sending sequence number");
      exit(0);
    }
    
    std::cout << "Ack " << recSeqNum << " sent." << std::endl; //
    if (currentPacket == numPackets - 1) {
      // Last file
      body[lastPacketSize] = '\0';
    }

    if (expSeqNum == recSeqNum) {
      ofs << body;
      // ofs << body;
      expSeqNum++;
      currentPacket++;
      if (expSeqNum == maxSeqNum) {
        expSeqNum = 0;
      }
    }
  }
  // Close ofstream
  ofs.close();
  // Clean up
  //delete[] buff;
  //delete[] buffer;
  printf("\nFinished\n");
   
  printf("\n");
  printf("Closing socket\n");
  close(newsockfd);
  printf("\nExiting..\n");
}
