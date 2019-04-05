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


  int portno = atoi(argv[2]);
 
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


  int received = 0;
  
  // ********************************** Receiving total length to expect **************************
  received = recv(sockfd, &receivedLen, sizeof(int), 0);
  //printf("Received: %d\n", received);
  if (received < 0) {
    perror("Error receieving prime.");
  } else if (received == 0) {
    std::cout << "Socket closed on server end. Closing socket." << std::endl;
    close(sockfd);
    exit(1);
  } else {
    // Convert back to int
    len=ntohl(receivedLen);
    std::cout << "Total length to expect on read: " << len << std::endl;
  }
  
  // Convert prime using htonl
  int convertedPrime;
  int received = 0;
  int totalSent = 0;
  char *buff = new char[len];
  
  // ********************************** Begin Loop of receiving and sending information ********************
  while(currentPrime < sqrt(len*2)) {
    // Send the current prime to the client
    convertedPrime = htonl(currentPrime);
    //printf("Sending Current Prime: %d\n", currentPrime);
    sent = send(newsockfd, &convertedPrime, sizeof(int), 0);
    //sieve->printRemainingPrimes(nums, 0);
    char *buffer= new char[nums.size()];
    //bzero(buffer, len);
    std::copy(nums.begin(), nums.end(), buffer);
    int bytesLeft = len;

    // ******************************** Sending list of numbers to client ************************
    totalSent = 0;
    //printf("Sending list of numbers to client!\n");
    //sieve->printRemainingPrimes(nums, 0);
    while(totalSent < len) {
      //printf("Sending data, totalSent: %d\n", totalSent);
      sent = send(newsockfd, buffer+totalSent, bytesLeft, 0);
        /*
        if(sent < 0){
          printf("Error writing to socket\n");
        } else {
        */
      totalSent += sent;
      bytesLeft -= sent;
        //}
    }
    printf("Sent ");
    sieve->printList(nums, 0);
    printf("\n\n\n\n\n\n");
    // ******************************** Receiving the current prime from client *******************
    //printf("Receiving prime from client!\n");  
    received = recv(newsockfd, &receivedPrime, sizeof(int), 0);
  
    if (received < 0) {
      perror("Error receieving prime.");
    } else if (received == 0) {
      printf("Socket closed while retrieving current prim from client!\n");
      close(newsockfd);
      exit(1);
    } else {
      // Convert back to int
      currentPrime=ntohl(receivedPrime);
      if (currentPrime == -1) {
        //close(newsockfd);
      }else {
      }
    }
    primes.push_back(currentPrime);

    // ****************************** Receiving list of numbers from client *********************
    //printf("Receiving list of numbers from client!\n");
    bzero(buff,len);
    received = 0;  
    nums.clear();
    while(received < len) {
      //received += recv(newsockfd, &buff, len, 0);
      received += recv(newsockfd, buff, len, 0);
      if(received < 0){
        perror("Error receieving data.\n");
      } else if (received == 0) {
        printf("Socket closed while retrieving list of numbers from client!\n");
        close(newsockfd);
        exit(1);
      } else {
        nums.insert(nums.end(), buff, buff + strlen(buff));
        //sieve->printList(nums, 0);
      }
      bzero(buff, len);
    }

    //printf("Total Received: %d, Total Expected: %d\n", received, len);
    //std::vector<char> readNums(buff, buff+len);
    //nums = readNums;
    printf("Received: ");
    sieve->printList(nums, 0);
    
    
    //printf("Prime: %d\n", currentPrime);
    printf("Prime: ");
    for (std::list<int>::iterator it=primes.begin(); it != primes.end(); ++it) {
      printf("%d, ", *it);
    }
    printf("\n");

    //sieve->printRemainingPrimes(nums, 0);
    // Find the next prime
    currentPrime = sieve->findNextPrime(nums, currentPrime);
    primes.push_back(currentPrime);
    //printf("Next prime: %d\n", currentPrime);
    // Remove multiples using 
    nums = sieve->removeMultiples(nums, currentPrime);
    //printf("Removed multiples for prime: %d\n", currentPrime);
  }

  // Clean up
  delete[] buff;
  //delete[] buffer;
  printf("\nFinished\n");
  
  while(!primes.empty()) {
    printf("%d, ", primes.front());
    primes.pop_front();
  }
  sieve->printRemainingPrimes(nums, currentPrime);
 
  printf("\n");
  printf("Closing socket\n");
  close(newsockfd);
  printf("\nExiting..\n");
}
