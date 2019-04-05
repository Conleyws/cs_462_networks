#include <stdio.h>
#include <list>
#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <pthread.h>
#include <cmath>
#include <iterator>
#include <vector>

class Sieve {

  public:
    Sieve();
    std::vector<char> makeInitialList(int);
    int findNextPrime(std::vector<char>, int); 
    void printList(std::vector<char>, int); 
    //std::list<int> findPrimes(std::list<int>);
    std::vector<char> removeMultiples(std::vector<char>, int);
    void printRemainingPrimes(std::vector<char>, int);
  private:
    //void setupConnections();
    std::list<int> currentPrimes;    
    int currentPrime;
};
