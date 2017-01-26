// nettest.cc 
//	Test out message delivery between two "Nachos" machines,
//	using the Post Office to coordinate delivery.
//
//	Two caveats:
//	  1. Two copies of Nachos must be running, with machine ID's 0 and 1:
//		./nachos -m 0 -o 1 &
//		./nachos -m 1 -o 0 &
//
//	  2. You need an implementation of condition variables,
//	     which is *not* provided as part of the baseline threads 
//	     implementation.  The Post Office won't work without
//	     a correct implementation of condition variables.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"

#include "system.h"
#include "network.h"
#include "post.h"
#include "interrupt.h"
#include "tcp.h"
#include "vsp.h"
#include "ftp.h"



// Test out message delivery, by doing the following:
//	1. send a message to the machine with ID "farAddr", at mail box #0
//	2. wait for the other machine's message to arrive (in our mailbox #0)
//	3. send an acknowledgment for the other machine's message
//	4. wait for an acknowledgement from the other machine to our 
//	    original message
//Mail test for 10 messages
/*void
MailTest(int farAddr)
{
    PacketHeader outPktHdr, inPktHdr;
    MailHeader outMailHdr, inMailHdr;
    const char *data = "Message ";
    const char *ack = "Got it!";
    char buffer[MaxMailSize];
    printf("%d\n",farAddr);
    for (int i=0;i<10;i++){
	    char counter_buff[10];
	    sprintf(counter_buff, "%d", i);
	    char * data_and_counter;
	    data_and_counter = new char(strlen(data)+strlen(counter_buff));
	    strcpy(data_and_counter,data);

	    strcat(data_and_counter,counter_buff);	
	    // construct packet, mail header for original message
	    // To: destination machine, mailbox 0
	    // From: our machine, reply to: mailbox 1

	    outPktHdr.to = farAddr;		
	    outMailHdr.to = 0;
	    outMailHdr.from = 1;
	    outMailHdr.length = strlen(data_and_counter) + 1;
	     
	    // Send the first message
	    postOffice->Send(outPktHdr, outMailHdr, data_and_counter); 

	    // Wait for the first message from the other machine
	    postOffice->Receive(0, &inPktHdr, &inMailHdr, buffer);
	    printf("Got \"%s\" from %d, box %d\n",buffer,inPktHdr.from,inMailHdr.from);
	    fflush(stdout);

	    // Send acknowledgement to the other machine (using "reply to" mailbox
	    // in the message that just arrived
	    outPktHdr.to = inPktHdr.from;
	    outMailHdr.to = inMailHdr.from;
	    outMailHdr.length = strlen(ack) + 1;
	    postOffice->Send(outPktHdr, outMailHdr, ack); 

	    // Wait for the ack from the other machine to the first message we sent.
	    postOffice->Receive(1, &inPktHdr, &inMailHdr, buffer);
	    printf("Got \"%s\" from %d, box %d\n",buffer,inPktHdr.from,inMailHdr.from);
	    fflush(stdout);
	    
     }
    // Then we're done!
    interrupt->Halt();
}*/
//Ring Topology of n-Machines
void
RingTest(int farAddr,int ownAddr)
{
	PacketHeader outPktHdr, inPktHdr;
	MailHeader outMailHdr, inMailHdr;
	const char * data = "Token";
	const char * ack = "ack";
        char buffer[MaxMailSize];
        printf("Machine %d\n",ownAddr);
	// Send the first message if machine 0
	if(ownAddr==0){
		outPktHdr.to = farAddr;		
		outMailHdr.to = 0;
		outMailHdr.from = 1;
		outMailHdr.length = strlen(data) + 1;	
		postOffice->Send(outPktHdr, outMailHdr, data); 
		// Wait for the ack from the other machine to the first message we sent.
		postOffice->Receive(1, &inPktHdr, &inMailHdr, buffer);
		printf("Got \"%s\" from %d, box %d\n",buffer,inPktHdr.from,inMailHdr.from);
		fflush(stdout);
		postOffice->Receive(0, &inPktHdr, &inMailHdr, buffer);
		printf("Got \"%s\" from %d, box %d\n",buffer,inPktHdr.from,inMailHdr.from);
		fflush(stdout);
		// Send acknowledgement to the other machine (using "reply to" mailbox
		// in the message that just arrived
		outPktHdr.to = inPktHdr.from;
		outMailHdr.to = inMailHdr.from;
		outMailHdr.length = strlen(ack) + 1;
		postOffice->Send(outPktHdr, outMailHdr, ack); 

	}
	// Wait for the first message from the other machine if not machine 0.
	else{
		postOffice->Receive(0, &inPktHdr, &inMailHdr, buffer);
		printf("Got \"%s\" from %d, box %d\n",buffer,inPktHdr.from,inMailHdr.from);
		fflush(stdout);
		// Send acknowledgement to the other machine (using "reply to" mailbox
		// in the message that just arrived
		outPktHdr.to = inPktHdr.from;
		outMailHdr.to = inMailHdr.from;
		outMailHdr.length = strlen(ack) + 1;
		postOffice->Send(outPktHdr, outMailHdr, ack); 
		// keep packet for some time
		Delay(2);
		// send forward in circle
		outPktHdr.to = farAddr;		
		outMailHdr.to = 0;
		outMailHdr.from = 1;
		outMailHdr.length = strlen(buffer) + 1;
		postOffice->Send(outPktHdr, outMailHdr, buffer); 
		// Wait for the ack from the other machine to the first message we sent.
		postOffice->Receive(1, &inPktHdr, &inMailHdr, buffer);
		printf("Got \"%s\" from %d, box %d\n",buffer,inPktHdr.from,inMailHdr.from);
		fflush(stdout);


	}
    // Then we're done!
    interrupt->Halt();

 

        
}
//TCP test in ring topology
void
TcpTest(int farAddr,int ownAddr)
{
	PacketHeader outPktHdr, inPktHdr;
	MailHeader outMailHdr, inMailHdr;
	Tcp tcp;
	const char * data = "Token";

        char buffer[MaxMailSize];
        printf("Machine %d\n",ownAddr);
	// Send the first message if machine 0
	if(ownAddr==0){
		outPktHdr.to = farAddr;		
		outMailHdr.to = 0;
		outMailHdr.from = 1;
		outMailHdr.length = strlen(data) + 1;
		bool acknowledge=false;	
		tcp.Send(outPktHdr, outMailHdr, data,&acknowledge);
		tcp.Receive(0, &inPktHdr, &inMailHdr, buffer);
		
	}
	// Wait for the first message from the other machine if not machine 0.
	else{
		tcp.Receive(0, &inPktHdr, &inMailHdr, buffer);
		// keep packet for some time
		Delay(2);
		bool acknowledge=false;
		// send forward in circle
		outPktHdr.to = farAddr;		
		outMailHdr.to = 0;
		outMailHdr.from = 1;
		outMailHdr.length = strlen(buffer) + 1;
		tcp.Send(outPktHdr, outMailHdr, buffer,&acknowledge); 
		

	}

    // Then we're done!
    interrupt->Halt();

 

        
}

void
VspTest(int farAddr,int ownAddr)
{
	if(ownAddr == 0){
		Vsp *vsp= new Vsp();
		PacketHeader outPktHdr;
		MailHeader outMailHdr;
		outPktHdr.to = farAddr;	
		outPktHdr.from = ownAddr;
		outMailHdr.to = 0;
		outMailHdr.from = 1;
		vsp->ParseMessageAndPopulate(outPktHdr,outMailHdr,"heeeeeellllllllllllllllllllllllllllllllllllllllooooooooooooooooo");
		//vsp->Send(outPktHdr,outMailHdr,);
	}
	else{
		PacketHeader inPktHdr;
		MailHeader inMailHdr;
		Vsp *vsp = new Vsp();
		char buffer[MaxMailSize];
		vsp->Receive(0, &inPktHdr, &inMailHdr, buffer);
	}
	interrupt->Halt();
}

void 
MailTest(int farAddr,int ownAddr)
{
	if(ownAddr==0){
		PacketHeader outPktHdr;
		MailHeader outMailHdr;
		outPktHdr.to = farAddr;	
		outMailHdr.to = 0;
		outMailHdr.from = 1;
		Ftp ftp;
		ftp.send(outPktHdr,outMailHdr,(char*)"/home/shah/NachOS/code/network/test");
	}
	else{
		PacketHeader inPktHdr;
		MailHeader inMailHdr;
		Ftp ftp;
		char buffer[MaxMailSize];
		ftp.receive(0, &inPktHdr, &inMailHdr, buffer);
	}
	interrupt->Halt();

}
