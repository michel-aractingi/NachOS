#include "copyright.h"
#include "ftp.h"
#include "system.h"
#include <strings.h> 
#include <string>
#include "post.h"
#include "network.h"
#include "tcp.h"
#include "interrupt.h"

void Ftp::send(PacketHeader pktHdr, MailHeader mailHdr,char* filename){
	std::string basepath("/home/shah/NachOS/code/network/ServerFiles/");
	std::string filen(filename);
	basepath+=filen;
	FILE *file;
	char *code = new char[MaxFilePacketLength];

	file = fopen((char*)basepath.c_str(), "rb");
	if(!file){	
		printf("Error in Openning File\n");
		fflush(stdout);
		interrupt->Halt();		
	}	
	int count=0;
	do 
	{
		code[count] = (char)fgetc(file);
		count++;
		if(count == MaxFilePacketLength){
			mailHdr.length = strlen(code) + 1;
			postOffice->Send(pktHdr, mailHdr,code);	
			printf("%s\n",code);
			fflush(stdout);
			count=0;	
		}

	} while(code[count-1] != EOF);
	if(count<MaxFilePacketLength){
		code[count-1]='\0';
		postOffice->Send(pktHdr, mailHdr,code);
	}
	mailHdr.length = strlen("##EndOfMessage##") + 1;
	postOffice->Send(pktHdr, mailHdr,"##EndOfMessage##");
	


}
void Ftp::receive(int box, PacketHeader *pktHdr, MailHeader *mailHdr, const char *data){
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
	FILE *file;
	data = message.c_str();

	file = fopen("/home/shah/NachOS/code/network/ClientFiles/testfile", "wb");
	for(int i=0;data[i]!='\0';i++){
		fputc((char)data[i],file);
	}

	//printf("Received Message  after joining is : %s\n\n",message.c_str());
	
}

