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
#include <unistd.h>

#include "Sieve.h"

class server {
  public:
    void printPrimes(std::list<int>);

  private:
    std::list<int> findPrimes(std::list<int>);
    std::list<int> currentPrimes;    
    int currentPrime;
};
