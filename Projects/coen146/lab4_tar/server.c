 /**************************
 *     Based on:
 *     socket example, server
 *     fall 2017
 ***************************/

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <netdb.h>
#include <math.h>
#include "header.h"

#define BUFF_LEN 100

#define ACK_FAIL_CHANCE 2

int main (int, char *[]); 

/*	int is_duplicate (packet p1, packet p2)
 *	
 *	Checks to see of p1 and p2 are identical packets.
 *	@param {packet} p1 First packet to be compared
 *	@param {packet} p2 Second packet to be compared
 *	@return {int} 1 if the packets are identical, 0 if not.
 */
int is_duplicate(packet p1, packet p2){
	if(p1.seq_num != p2.seq_num || p1.checksum != p2.checksum || p1.numbytes != p2.numbytes || p1.type != p2.type) return 0;
	int i; 
	for(i = 0; i < p1.numbytes; i++){
		if(p1.data[i] != p2.data[i]) return 0;
	}
	return 1;
}


/*********************
 * main
 *********************/
int main (int argc, char *argv[])
{
	int rcvd_bytes; 
	char buff[BUFF_LEN];
	struct addrinfo hints, *res;
	UDPSocket mysocket;
	int seq = 0;
	char *filename;
	FILE *dest;
	packet prev_message;

	// set up
	memset (buff, '0', sizeof (buff)); 
	memset (&hints, 0, sizeof (hints));

	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_flags = AI_PASSIVE;

	getaddrinfo(NULL, argv[1], &hints, &res);
	
	// create socket, bind, and listen
	if((mysocket.sockfd = socket (res->ai_family, res->ai_socktype, 0)) < 0){
		printf("socket creation error\n");
		return 0;
	}
		
	if((bind (mysocket.sockfd, res->ai_addr, res->ai_addrlen)) < 0){
		printf("socket bind error\n");
		return 0;
	}

	srand(time(NULL));
	
	mysocket.socklen = sizeof mysocket.remoteInfo;
	while(1){
		//Reset buffer and recieve new message
		memset(buff, '0', sizeof(buff));
		rcvd_bytes = recvfrom(mysocket.sockfd, buff, BUFF_LEN - 1, 0, &mysocket.remoteInfo, &mysocket.socklen);
		
		//reassemble packet
		packet message;
		memcpy(&message, buff, sizeof(message));

		//Is this the handshake packet?
		if(message.type == HANDSHAKE){
			//send handshake ACK
			seq = (seq == 0) ? 1 : 0;
			memcpy(&prev_message, buff, sizeof(prev_message));
			sendto(mysocket.sockfd, buff, BUFF_LEN - 1, 0, &mysocket.remoteInfo, mysocket.socklen);
			continue;
		}

		//Since this is more than a handshake packet (and therefore we care about the data), check to see if the data is valid.
		if(validate_msg(message, seq) == 0 || is_duplicate(message, prev_message) == 1){
			//If it isn't, or if the packet is a duplicate, resend the previous ACK.
			memcpy(buff, &prev_message, sizeof(prev_message));
			sendto(mysocket.sockfd, buff, BUFF_LEN - 1, 0, &mysocket.remoteInfo, mysocket.socklen);
			continue;
		}

		//Is this a filename packet?
		if(message.type == FILENAME){
			//If so, open the file designated by filename for writing and flip the sequence bit, then send an ACK.
			filename = message.data;
			filename[message.numbytes] = '\0';
			dest = fopen(filename, "w");
			seq = (seq == 0) ? 1 : 0;
			memcpy(&prev_message, buff, sizeof(prev_message));
			sendto(mysocket.sockfd, buff, BUFF_LEN - 1, 0, &mysocket.remoteInfo, mysocket.socklen);
		}

		//Is this a data packet?
		if(message.type == DATA){
			//If so, write the data to the currently open file, send an ACK, and flip the sequence bit.
			fwrite(message.data, 1, message.numbytes, dest);
			seq = (seq == 0) ? 1 : 0;
			memcpy(&prev_message, buff, sizeof(prev_message));
			if(rand() % 10 >= ACK_FAIL_CHANCE){
				sendto(mysocket.sockfd, buff, BUFF_LEN - 1, 0, &mysocket.remoteInfo, mysocket.socklen);
			}
		}

		//Is this a fileend packet?
		if(message.type == FILEEND){
			//If so, then we're done. Send the final ack and break out of the loop.
			seq = (seq == 0) ? 1 : 0;
			sendto(mysocket.sockfd, buff, BUFF_LEN - 1, 0, &mysocket.remoteInfo, mysocket.socklen);
			break;
		}
	}

	//Close socket and file and return successfully
	fclose(dest);
	close(mysocket.sockfd);
	return 0;
}














