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

// Test out message delivery, by doing the following:
//	1. send a message to the machine with ID "farAddr", at mail box #0
//	2. wait for the other machine's message to arrive (in our mailbox #0)
//	3. send an acknowledgment for the other machine's message
//	4. wait for an acknowledgement from the other machine to our 
//	    original message
/*
void
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
MailTest(int farAddr)
{
	PacketHeader outPktHdr, inPktHdr;
	MailHeader outMailHdr, inMailHdr;
	const char * data = "Token";
	const char * ack = "ack";
        char buffer[MaxMailSize];

	// Send the first message if machine 0
	if(farAddr==1){
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
		for(int i=0;i<2550000;i++);
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
