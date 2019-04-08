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
  
  // ********************************** Receiving packet body size to expect **************************
  int packetSizeReceived = 0;
  int packetSize = 0;
  
  received = recv(newsockfd, &packetSizeReceived, sizeof(int), 0);
  if (received < 0) {
    perror("Error receieving packet body size.");
  } else if (received == 0) {
    std::cout << "Socket closed on server end. Closing socket." << std::endl;
    close(sockfd);
    exit(1);
  } else {
    // Convert back to int
    packetSize=ntohl(packetSizeReceived);
    std::cout << "Total Packet Body Size: " << packetSize << std::endl;
  }

  numPackets = (fileSize + packetSize - 1) / packetSize;
  std::cout << "Total number of packets: " << numPackets << std::endl;

  int totalSent = 0;
  bool finishedReceiving = false;
  char *buff = new char[packetSize];
  std::vector<char> packet;
  int sequenceNumber = 0;
  
  // ********************************** Begin Loop of receiving packets and sending Acks ********************
  while(!finishedReceiving) {
   
  //Loop
    // Read in packet
    // Check sequence #
    // Get packet body and put to file or buffer?
    //
    // Send back ACK with sequence #
  //End loop
  //Write to file?
    char *buffer= new char[packet.size()];
    
    std::cout << "Expected Sequence Number: " << sequenceNumber << std::endl;
    // ******************************** Receiving Packet ************************
    bzero(buff, packetSize);
    received = 0;  
    packet.clear();

    received += recv(newsockfd, buff, packetSize, 0);
    if(received < 0){
      perror("Error receieving data.\n");
    } else if (received == 0) {
      printf("Socket closed while retrieving list of numbers from client!\n");
      close(newsockfd);
      exit(1);
    } else {
      packet.insert(packet.end(), buff, buff + strlen(buff));
    }
    bzero(buff, packetSize);

    std::cout << "Packet : " << sequenceNumber << " received." << std::endl; // Sequence number 

    // Send ACK
    /*
    sent = send(sockfd, &ack, sizeof(int), 0);
    if (sent < 0) {
      perror("Error sending expected size");
      exit(0);
    }
    */
    std::cout << "Ack " << sequenceNumber << " sent." << std::endl; //
    
  }

  // Clean up
  delete[] buff;
  //delete[] buffer;
  printf("\nFinished\n");
   
  printf("\n");
  printf("Closing socket\n");
  close(newsockfd);
  printf("\nExiting..\n");
}
