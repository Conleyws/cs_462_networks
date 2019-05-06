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
  // Header
  int headerSize = 65;
  int seqNumSize = 33;
  int crcSize = 32;

  int packetSize = headerSize + bodySize;
  char buff[packetSize];
  char ack[headerSize];
  int currentPacket = 0;
  char binSeqNum[seqNumSize];
  char binCRC[crcSize];
  char body[bodySize];
  bool firstLoop = false;
  int lastBodySize = fileSize % bodySize;
  if (lastBodySize == 0) {
    lastBodySize = bodySize;
  }
  std::cout << "Standard Packet Size: " << packetSize << std::endl;
  std::cout << "Last Body Size: " << lastBodySize << std::endl;

  std::ofstream ofs("copyFile", std::ios::binary);
  //TODO Window Size option
  int windowSize = 8;
  int bitsLeft = packetSize;
  std::map<int, std::vector<char>> dataMap; 
  int windowStart = 0;
  int windowEnd = 0;
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
    
    std::cout << std::endl << "Expected Sequence Number: " << expSeqNum << std::endl;
    // ******************************** Receiving Packet ************************
    received = 0;

    
    if (currentPacket == numPackets - 1) {
      // Last file
      //packetSize = lastBodySize + headerSize;
      bodySize = lastBodySize;
    }
    bitsLeft = headerSize + bodySize;
    while (received < bitsLeft) {
      received += recv(newsockfd, buff, bitsLeft, 0);
      if(received < 0){
        perror("Error receieving data.\n");
      } else if (received == 0) {
        printf("Socket closed while receiving packets from client!\n");
        close(newsockfd);
        exit(1);
      } else {
        // std::cout << "Received: " << received << " bits." << std::endl;
        // std::cout << "Buff Length: " << strlen(buff) << std::endl;
        //std::cout << "Received: " << buff << std::endl;
        packet.insert(packet.end(), buff, buff + received);
        //bzero(buff, received);
      }

    }
    std::copy(packet.begin(), packet.end(), buff);
    packet.clear();
    
    // Grabbing Sequence Number
    strncpy(binSeqNum, buff, seqNumSize);
    std::string strSeqNum(binSeqNum);

    // Grabbing CRC
    strncpy(binCRC, buff+33, crcSize);
    std::string strCRC(binCRC);
    
    
    
    for (int i = 0; i < bodySize; i++) {
      body[i] = buff[headerSize+i];
    }

    boost::crc_32_type crc;
    crc.process_bytes(body, bodySize);
    boost::uint32_t checksum = crc.checksum();
    std::cout << "Checksum: " << checksum << std::endl;
    
    //if (crc != checksum) {
      // Issue with packet, don't send ACK
    //}

    //std::cout << "Using STOI on: " << strSeqNum << std::endl;
    recSeqNum = std::stoi(strSeqNum, nullptr, 2); 
    
    // Convert sequence number
    std::cout << "Packet: " << recSeqNum << " received." << std::endl; // Sequence number 
    bzero(buff, packetSize); 

    // Send ACK
    sent = send(newsockfd, &binSeqNum, seqNumSize, 0);
    if (sent < 0) {
      perror("Error sending sequence number");
      exit(0);
    }
    
    std::cout << "Ack " << recSeqNum << " sent." << std::endl;
    body[bodySize] = '\0';
    std::cout << "Adding data to map at recSeqNum[" << recSeqNum << "]\nData: " << body << std::endl;
    std::vector<char> dataVec(body, body+bodySize);
    dataMap[recSeqNum] = dataVec;
    bzero(body, bodySize);
    if (expSeqNum == recSeqNum) {
      expSeqNum++;
      currentPacket++;
      if (expSeqNum == maxSeqNum) {
        expSeqNum = 0;
      }
    }
    //TODO have an update for GBN
    //Update window for sliding window
    std::cout << "seqNum: " << recSeqNum << std::endl;
    std::cout << "WindowEnd: " << windowEnd << std::endl;
    std::cout << "WindowStart: " << windowStart << std::endl;
    if(windowEnd == maxSeqNum-1){
      std::cout << "WidowEnd == maxSeqNum" << std::endl;
      windowEnd=0;
    } else {
      windowEnd++;
    }
    std::cout << "Updated Window" << std::endl;
    std::cout << "WindowStart: " << windowStart << std::endl;
    std::cout << "WinodwEnd: " << windowEnd << std::endl;
    if(recSeqNum == windowEnd && firstLoop == true){
      std::cout << "SeqNum = WindowStart. Writing oldest data to file (seqNum[" << windowStart << "])" << std::endl;
      std::cout << "Writing data: " << dataMap[windowStart].data() << std::endl;
      ofs.write(dataMap[windowStart].data(), bodySize);
      //dataMap.erase(windowStart);
      if(windowStart == maxSeqNum-1){
        windowStart = 0;
      } else {
        windowStart++;
      }
    }else if(firstLoop==false){
      if(recSeqNum == maxSeqNum-1){
        std::cout << "Finished filling up window" << std::endl;
        firstLoop = true;
      }    
    }
  }
  std::cout << "Recieved all the data, writing the rest of the window" << std::endl << std::endl;
  //TODO write remaining items in window once finished receieving
  while(windowStart != windowEnd){
    std::cout << "Window start: " << windowStart << std::endl;
    std::cout << "Window end: " << windowEnd << std::endl;
    std::cout << "Writing data seqNum[" << windowStart << "] to file" << std::endl;
    std::cout << "Writing to file: " << dataMap[windowStart].data() << std::endl;
    ofs.write(dataMap[windowStart].data(), bodySize);
    //dataMap.erase(windowStart);
    if(windowStart == maxSeqNum-1){
      windowStart = 0;
    } else {
      windowStart++;
    }
  }
  //ofs.write(dataMap[windowStart].data(), bodySize);
  ofs.close();
  // Clean up
  printf("\nFinished\n");
  printf("\n");
  printf("Closing socket\n");
  close(newsockfd);
  printf("\nExiting..\n");
}
