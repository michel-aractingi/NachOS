#include "copyright.h"

#ifndef FTP_H
#define FTP_H

#define MaxFilePacketLength 10
#include "network.h"
#include "synchlist.h"
#include "post.h"


class Ftp{

public:

	void send(PacketHeader pktHdr, MailHeader mailHdr, char * filename);
	void receive(int box, PacketHeader *pktHdr, MailHeader *mailHdr, const char *data);
	

};

#endif
