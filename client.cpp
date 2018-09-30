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
	unsigned int cli_size = 0;
	char* send_buf = new char[516];
	char recv_buf[516];

	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sockfd < 0)
	{
		std::cout << "failed to open socket\n";
		return 1;
	}

	memset(&serv_addr, 0, sizeof(serv_addr));
	memset(&cli_addr, 0, sizeof(cli_addr));
	memset(&send_buf, 0, sizeof(send_buf));
	memset(&recv_buf, 0, sizeof(recv_buf));

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr("192.168.1.69");
	serv_addr.sin_port = htons(port);

	cli_addr.sin_family = AF_INET;
	cli_addr.sin_addr.s_addr = inet_addr("192.168.1.69");
	cli_addr.sin_port = htons(port);

	packet rrq0, recv_pak;
	rrq0.set_opcode(1);
	rrq0.set_filename("smallertest.txt");
	rrq0.set_mode("octet");
	send_buf = rrq0.encode();

	if (sendto(sockfd, send_buf, buf_len, 0, (struct sockaddr*)&serv_addr, serv_size) < 0)
	{
		std::cout << "sendto failed\n";
		std::cout << errno << "\n";
	}

	recv_len = recvfrom(sockfd, recv_buf, buf_len, 0, (struct sockaddr*)&cli_addr, &cli_size);
	if (recv_len < 0)
	{
		std::cout << "recvfrom failed\n";
		std::cout << errno << "\n";
		return 1;
	}

	std::cout << "received packet from: " << inet_ntoa(serv_addr.sin_addr) << ":" << ntohs(serv_addr.sin_port) << std::endl;

	recv_pak.decode(recv_buf);

	std::cout << recv_pak.get_opcode() << recv_pak.get_blkno() << recv_pak.get_data() << std::endl;
	std::cin.get();
	std::cout << "sorry what";
	close(sockfd);
	
	return 0;
}
