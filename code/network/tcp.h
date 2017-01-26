#include "copyright.h"

#ifndef TCP_H
#define TCP_H

#define REEMISSIONS 50

#include "network.h"
#include "synchlist.h"



class Tcp{
public:
	PacketHeader currPktHdr;
	MailHeader currMailHdr;
	const char* currData;
	void Send(PacketHeader pktHdr, MailHeader mailHdr, const char *data, bool* ackn);    
	void Receive(int box, PacketHeader *pktHdr, MailHeader *mailHdr, char *data);
	void SendAgain();
	void ReceiveAck();

private:
	Semaphore *Ack;




};

#endif
