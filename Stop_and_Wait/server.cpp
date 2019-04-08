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
  int sequenceNumberReceived = 0;
  int sequenceNumberRange = 0;
  
  received = recv(newsockfd, &sequenceNumberReceived, sizeof(int), 0);
  if (received < 0) {
    perror("Error receieving sequence number range.");
  } else if (received == 0) {
    std::cout << "Socket closed on server end. Closing socket." << std::endl;
    close(sockfd);
    exit(1);
  } else {
    // Convert back to int
    sequenceNumberRange=ntohl(sequenceNumberReceived);
    std::cout << "Sequence Number Range: " << sequenceNumberRange << std::endl;
  }


  numPackets = (fileSize + bodySize - 1) / bodySize;
  std::cout << "Total number of packets: " << numPackets << std::endl;

  int totalSent = 0;
  bool finishedReceiving = false;
  std::vector<char> packet;
  int recSequenceNumber = 0;
  int expSequenceNumber = 0;
  int maxSequenceNumber = 0;
  int headerSize = 4;

  int packetSize = headerSize + bodySize;
  char buff[packetSize];
  char ack[5];
  int currentPacket = 0;
  char binSeqNum[4];
  char body[bodySize];

  std::ofstream ofs("copy.txt", std::ofstream::out);
  
  // ********************************** Begin Loop of receiving packets and sending Acks ********************
  while(currentPacket < numPackets) {
   
  //Loop
    // Read in packet
    // Check sequence #
    // Get packet body and put to file or buffer?
    //
    // Send back ACK with sequence #
  //End loop
  //Write to file?
    
    std::cout << "Expected Sequence Number: " << expSequenceNumber << std::endl;
    // ******************************** Receiving Packet ************************
    received = 0;

    received += recv(newsockfd, buff, packetSize, 0);
    if(received < 0){
      perror("Error receieving data.\n");
    } else if (received == 0) {
      printf("Socket closed while receiving packets from client!\n");
      close(newsockfd);
      exit(1);
    } else {
      std::cout << "Buff Length: " << strlen(buff) << std::endl;
      std::cout << "Received: " << buff << std::endl;
      packet.insert(packet.end(), buff, buff + strlen(buff));
    }
    // Zero out the buff for next packet
    // bzero(buff, packetSize);
    // first four = sequence number
    
    strncpy(binSeqNum, buff, 4);
    strncpy(body, &buff[4], bodySize);
    
    
    for (int index = 0; index < packetSize; index++) {
      if (index < 4) {
        // Header
        // binSeqNum.append(packet(index));
      } else if (index < bodySize + 4) {
        // body
        // body.push_back(packet(index));
      } else {
        // CRC ?
      }
    }
    
    // Convert sequence number
    //recSequenceNumber = std::stoi(binSeqNum, nullptr, 2);
    std::cout << "Packet: " << recSequenceNumber << " received." << std::endl; // Sequence number 
    
    if (expSequenceNumber == recSequenceNumber) {
      ofs << buff;
      // ofs << body;
      expSequenceNumber++;
    }
    bzero(buff, packetSize);
    //binSeqNum.clear();
    //body.clear();
    packet.clear();
   
    /*
    int sequenceNum = 4;
    // Convert num to binary
    char binSeqNum [33];
    std::itoa(sequenceNum, binSeqNum, 2);

    // Convert binary to num when received
    int recSequenceNumber;
    recSequenceNumber = std::stoi(binSeqNum, nullptr, 2);
    */
    
    // Send ACK
    sent = send(newsockfd, &recSequenceNumber, sizeof(int), 0);
    if (sent < 0) {
      perror("Error sending sequence number");
      exit(0);
    }
    
    std::cout << "Ack " << recSequenceNumber << " sent." << std::endl; //
  }
  // Close ofstream
  ofs.close();
  // Clean up
  delete[] buff;
  //delete[] buffer;
  printf("\nFinished\n");
   
  printf("\n");
  printf("Closing socket\n");
  close(newsockfd);
  printf("\nExiting..\n");
}
