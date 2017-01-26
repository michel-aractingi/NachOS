#include "copyright.h"
#include "vsp.h"
#include "system.h"
#include <strings.h> 
#include <string>
#include "post.h"
#include "network.h"
#include "tcp.h"
#include "interrupt.h"

Packet * Head=NULL;
void Vsp::ParseMessageAndPopulate(PacketHeader pktHdr, MailHeader mailHdr,const char* message){
	std::string str(message);

	//Tokenize the Message into N packets depending on the size of messages
	for (unsigned i = 0; i < str.length(); i += MaxPacketLength){

		mailHdr.length = strlen(str.substr(i, MaxPacketLength).c_str()) + 1;
		postOffice->Send(pktHdr, mailHdr,str.substr(i, MaxPacketLength).c_str());
		printf("%s\n",str.substr(i, MaxPacketLength).c_str());

	}
	mailHdr.length = strlen("##EndOfMessage##") + 1;
	postOffice->Send(pktHdr, mailHdr,"##EndOfMessage##");


}
void Vsp::AddtoList(Packet * pack){

		if(Head==NULL){
			
			Head=pack;
			Head->next=NULL;		
		}
		else{
			Packet *temp= Head;
			while(temp->next!=NULL){
				printf("%s\n",temp->currData);				
				temp=temp->next;
			}
		
			temp->next = pack;
		}


}

void Vsp::Receive(int box, PacketHeader *pktHdr, MailHeader *mailHdr, const char *data){

	std::string message("");
	for(;;){
		
		postOffice->Receive(box, pktHdr, mailHdr,(char*) data);
		printf("Got \"%s\" from %d, box %d\n",data,pktHdr->from,mailHdr->from);
		fflush(stdout);

		std::string str(data);
		if(str=="##EndOfMessage##")
			break;
		message+=data;
	}
	printf("Received Message  after joining is : %s\n\n",message.c_str());
	data = message.c_str();
}


