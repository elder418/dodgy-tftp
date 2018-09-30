#include <iostream>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "packet.h"

int main(int argc, char** argv)
{
	const int port = 69;
	const int buf_len = 512;
	int sockfd, recv_len;
	struct sockaddr_in cli_addr, serv_addr;
	unsigned int serv_size = sizeof(serv_addr);
	char buf[buf_len];

	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sockfd < 0)
	{
		std::cout << "failed to open socket\n";
		return 1;
	}

	memset(&serv_addr, 0, sizeof(serv_addr));
	memset(&cli_addr, 0, sizeof(cli_addr));
	memset(&buf, 0, sizeof(buf));

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr("192.168.1.69");
	serv_addr.sin_port = htons(port);

	packet rrq0;
	rrq0.set_opcode(1);
	rrq0.set_filename("testfile");
	rrq0.set_mode("octet");

	strcpy(buf, "naman yef");

	if (sendto(sockfd, buf, sizeof(buf), 0, (struct sockaddr*)&serv_addr, serv_size) < 0)
	{
		std::cout << "sendto failed\n";
		std::cout << errno << "\n";
	}

	close(sockfd);
	
	return 0;
}
