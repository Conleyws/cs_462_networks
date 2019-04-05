#include "PacketHelper.h"

PacketHelper::PacketHelper() {

}

std::vector<char> PacketHelper::makeInitialList(int maxValue) {
  int totalIndices;
  if (maxValue % 2 == 0) {
    // Even number
    totalIndices = maxValue / 2;
  } else {
    // Odd number
    totalIndices = (maxValue + 1) / 2;
    
  }
  printf("Total Indices: %d\n", totalIndices);
  std::vector<char> bits(totalIndices, '1');
  return bits;
}



