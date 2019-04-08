#include <string.h>
#include <vector>

class PacketHelper {

  public:
    PacketHelper();
    int getSequenceNumber(std::vector<char>);
    std::vector<char> getBody(std::vector<char>);

};
