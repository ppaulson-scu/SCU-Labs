/***************************
 *	Based on
 *	socket example, client
 *	fall 2017
 **************************/

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include "header.h"

int main (int, char *[]);

#define BUFF_LEN 100

/********************
 * main
 ********************/
int main (int argc, char *argv[])
{
	UDPSocket mysocket;
	char buff[BUFF_LEN];
	struct addrinfo hints, *res;
	int seq = 0; 

	if (argc != 5)
	{
		printf ("Usage: %s <ip of server> <port> <srcfile> <destfile> \n",argv[0]);
		return 1;
	} 

	// set up
	memset (buff, '0', sizeof (buff));
	memset (&hints, 0, sizeof (hints));

	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_flags = AI_PASSIVE;

	getaddrinfo(argv[3], argv[4], &hints, &res);

	srand(time(NULL));	

	// open socket
	if ((mysocket.sockfd = socket (res->ai_family, res->ai_socktype, res->ai_protocol)) < 0)
	{
		printf ("Error : Could not create socket \n");
		return 1;
	}
	
	mysocket.socklen = sizeof(mysocket.remoteInfo);

	//Create handshake packet and copy into buffer for transmission
	packet handshake_message;
	handshake_message.type = HANDSHAKE;
	handshake_message.seq_num = seq;
	memcpy(buff, &handshake_message, sizeof(handshake_message));
	
	//Loop until we recieve an ACK
	while(1){
		//Send the message
		sendto(mysocket.sockfd, buff, BUFF_LEN - 1, 0, res->ai_addr, res->ai_addrlen);

		//When we get the message back, reassemble the data back into a struct for ease of use
		recvfrom(mysocket.sockfd, buff, BUFF_LEN - 1, 0, &mysocket.remoteInfo, &mysocket.socklen);
		packet response;
		memcpy(&response, buff, sizeof(response));

		//Check to see if the message is correct
		if(response.seq_num == seq && response.type == HANDSHAKE){
			seq = (seq == 0) ? 1 : 0;
			break;
		}
	}

	//Create filename packet and copy into buffer
	packet filename_message; 
	filename_message.type = FILENAME;
	filename_message.checksum = calc_checksum(argv[2], sizeof(argv[2]));
	memcpy( filename_message.data, argv[2], sizeof(argv[2]));
	filename_message.numbytes = sizeof(argv[2]);
	filename_message.seq_num = seq;
	memcpy( buff, &filename_message, sizeof(filename_message));

	//Loop until we receive an ACK
	while(1){
		//Send the message
		sendto(mysocket.sockfd, buff, BUFF_LEN - 1, 0, res->ai_addr, res->ai_addrlen);

		//When we get it back, translate it back into the struct
		recvfrom(mysocket.sockfd, buff, BUFF_LEN - 1, 0, &mysocket.remoteInfo, &mysocket.socklen);
		packet response;
		memcpy(&response, buff, sizeof(response));

		//Check to see of the message is correct
		if(validate_msg(response, seq) == 1 && response.type == FILENAME){
			seq = (seq == 0) ? 1 : 0;
			break;
		}
	}
		
	//Open the file for reading
	FILE *src = fopen(argv[1], "r");

	//this do/while sends the entire file
	do{
		//For each chunk of the file, create a packet
		packet message;
		message.seq_num = seq;
		message.type = DATA;
		message.numbytes = fread(message.data, 1, 10, src);
		message.checksum = calc_checksum(message.data, message.numbytes);

		//Loop until we sent the packet
		while(1){
			//Simulate network instability by setting the checksum to 0 4/5 of the time.					
			if(rand() % 5 != 0){ 
				message.checksum = 0;
			}else{
				message.checksum = calc_checksum(message.data, message.numbytes);
			}
			memcpy(buff, &message, sizeof(message));

			//Send the message
			sendto(mysocket.sockfd, buff, BUFF_LEN - 1, 0, res->ai_addr, res->ai_addrlen);
		
			//Receive and translate
			recvfrom(mysocket.sockfd, buff, BUFF_LEN - 1, 0, &mysocket.remoteInfo, &mysocket.socklen);
			packet response;
			memcpy(&response, buff, sizeof(response));

			//Check to see if valid
			if(validate_msg(response, seq) == 1 && response.type == DATA){
				seq = (seq == 0) ? 1 : 0;
				break;
			}
		}
	//Repeat until we hit the end of file
	}while(feof(src) == 0);

	//Close file
	fclose(src);

	//Create final packet
	packet final_message;
	final_message.type = FILEEND;
	final_message.seq_num = seq;
	final_message.numbytes = 0;
	final_message.checksum = calc_checksum(final_message.data, final_message.numbytes);
	memcpy(buff, &final_message, sizeof(final_message));

	//Loop until we successfully send
	while(1){

		//Send
		sendto(mysocket.sockfd, buff, BUFF_LEN - 1, 0, res->ai_addr, res->ai_addrlen);

		//Receive
		recvfrom(mysocket.sockfd, buff, BUFF_LEN - 1, 0, &mysocket.remoteInfo, &mysocket.socklen);

		//Translate
		packet response;
		memcpy(&response, buff, sizeof(response));

		//Check for validity
		if(response.seq_num == seq && response.type == FILEEND){
			seq = (seq == 0) ? 1 : 0;
			break;
		}
	}
	
	//Close socket and return successfully
	close(mysocket.sockfd);
	return 0;	
	
}
