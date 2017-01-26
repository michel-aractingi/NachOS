#include "copyright.h"

#include "system.h"
#include <strings.h> 
#include "post.h"
#include "network.h"
#include "tcp.h"
#include "interrupt.h"

static void SendDone(int arg)
{ Tcp *tcp = (Tcp *)arg; tcp->SendAgain(); }

static void ReceiverHelper(int arg)
{ Tcp* tcp = (Tcp *) arg; tcp->ReceiveAck(); }


bool Ackn = false;
void Tcp::ReceiveAck(){
	printf("Thread is here\n");
	PacketHeader inPktHdr;
	MailHeader inMailHdr;
	char buffer[MaxMailSize];
	postOffice->Receive(1, &inPktHdr, &inMailHdr, buffer);	
	Ackn=true;
	this->Ack->V();
	printf("Got \"%s\" from %d, box %d\n",buffer,inPktHdr.from,inMailHdr.from);
	fflush(stdout);

}
void Tcp::SendAgain(){
	if(Ackn==false){
		this->Ack->V();

	}	
        interrupt->SetLevel(interrupt->getLevel());

}

void Tcp::Send(PacketHeader pktHdr, MailHeader mailHdr, const char *data,bool * ackn){
		
	this->Ack = new Semaphore("message sent", 0);;
	this->currPktHdr = pktHdr;
	this->currMailHdr = mailHdr;
	this->currData = data;	
	Thread *t = new Thread("Receiver worker");
	t->Fork(ReceiverHelper, (int) this);
	for(int i=0;i<REEMISSIONS;i++){
		postOffice->Send(pktHdr, mailHdr, data); 
		printf("Message Sent! Try number %d \n",i+1);
		fflush(stdout);
		interrupt->SetLevel(interrupt->getLevel());
		interrupt->Schedule(SendDone, (int)this, 200000, TimerInt);
		this->Ack->P();
		if(Ackn==true){
			printf("Pleaseee Work!!! :( \n");
			break;
		}

	}
	

}

void Tcp::Receive(int box, PacketHeader *pktHdr, MailHeader *mailHdr, char *data){

	const char* ack = "ack";
	postOffice->Receive(box, pktHdr, mailHdr, data);
	printf("Got \"%s\" from %d, box %d\n",data,pktHdr->from,mailHdr->from);
	fflush(stdout);
	// Send acknowledgement to the other machine (using "reply to" mailbox
	// in the message that just arrived
	PacketHeader outPktHdr;
	MailHeader outMailHdr;
	outPktHdr.to = pktHdr->from;
	outMailHdr.to = mailHdr->from;
	outMailHdr.length = strlen(ack) + 1;
	postOffice->Send(outPktHdr, outMailHdr, ack); 
	printf("Ack Sent\n");


}
