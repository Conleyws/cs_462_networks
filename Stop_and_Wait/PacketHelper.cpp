#include "PacketHelper.h"

// Packet:
//  HEADER
//    Source - 4 Bytes
//    Dest - 4 Bytes
//    Sequence # - 4 Bytes
//    TTL (Time to live) - 4 Bytes?
//    CRC?
//  BODY
//    Message - User defined

PacketHelper::PacketHelper() {

}

int PacketHelper::getSequenceNumber(std::vector<char> packet) {

}

std::vector<char> PacketHelper::getBody(std::vector<char> packet) {

}

