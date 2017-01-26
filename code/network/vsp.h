#include "copyright.h"

#ifndef VSP_H
#define VSP_H

#define MaxPacketLength 10
#include "network.h"
#include "synchlist.h"
#include "post.h"
struct Packet{
	PacketHeader currPktHdr;
	MailHeader currMailHdr;
	const char * currData;
	Packet * next;
};
class Vsp{

public:

	void ParseMessageAndPopulate(PacketHeader pktHdr, MailHeader mailHdr,const char * message);
	void Receive(int box, PacketHeader *pktHdr, MailHeader *mailHdr, const char *data);
	void AddtoList(Packet *pack);


};

#endif
