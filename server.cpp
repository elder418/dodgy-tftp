#include <iostream>
#include <fstream>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "packet.h"

int main(int argc, char** argv)
{
	const int port = 69;		//tftp uses port 69 and
	const int buf_len = 516;	// 512 byte data chunks
	int sockfd, recv_len;
	struct sockaddr_in serv_addr, cli_addr;
	unsigned int cli_size = sizeof(cli_addr);
	//char buf[buf_len];
	char* send_buf = new char[516];
	char recv_buf[516];
	std::ifstream r_file;
	//open ipv4 udp socket
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sockfd < 0)
	{
		std::cout << "failed to open socket\n";
		return 1;
	}

	//serv_addr = {0};
	//cli_addr = {0};
	memset(&serv_addr, 0, sizeof(serv_addr));
	memset(&cli_addr, 0, sizeof(cli_addr));
	memset(&send_buf, 0, sizeof(send_buf));
	memset(&recv_buf, 0, sizeof(recv_buf));

	serv_addr.sin_family = AF_INET;
	//insert current host ip into addr struct
	serv_addr.sin_addr.s_addr = inet_addr("192.168.1.69");//htonl(INADDR_ANY);
	//convert int to network byte order 
	serv_addr.sin_port = htons(port);

	if (bind(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
	{
		std::cout << "failed to bind socket\n";
		std::cout << errno << "\n";
		return 1;
	}

	std::cout << "successfully bound " << inet_ntoa(serv_addr.sin_addr) << ":" << ntohs(serv_addr.sin_port) << "\n";

	packet recv_pak, send_pak;

	while (1)
	{
		memset(&recv_buf, 0, sizeof(recv_buf));
		memset(&send_buf, 0, sizeof(send_buf));
		memset(&cli_addr, 0, sizeof(cli_addr));
		std::cout << "listening..." << std::endl;

		recv_len = recvfrom(sockfd, recv_buf, buf_len, 0, (struct sockaddr*)&cli_addr, &cli_size);
		if (recv_len < 0)
		{
			std::cout << "recvfrom failed\n";
			std::cout << errno << "\n";
			return 1;
		}

		std::cout << "received packet from: " << inet_ntoa(cli_addr.sin_addr) << ":" << ntohs(cli_addr.sin_port) << std::endl;

		recv_pak.decode(recv_buf);
		std::cout << recv_pak.get_opcode();
		switch (recv_pak.get_opcode())
		{
			case 0: //empty/invalid
				std::cout << "invalid opcode\n"; //send error
				break;
			case 1: //RRQ
			{
				if (strcmp(recv_pak.get_filename(), "smallertest.txt") == 0) //see if req'd "file" exists
				{	
					std::streampos r_size;
					char* r_mem;
					//open file
					r_file.open("smallertest.txt", std::ios::in | std::ios::ate | std::ios::binary);
					r_size = r_file.tellg();
					if (r_size > 512)
					{
						//BIG BLOCK SPLIT IT UP GEGYEGYEGYHUEUEHUE
					}
					else
					{
						r_mem = new char[r_size]; //CLEAN THIS UP SOMEWHERE D00d
						r_file.seekg(0, std::ios::beg);
						r_file.read(r_mem, r_size);
						r_file.close();
						//create data packet
						send_pak.set_opcode(3);
						send_pak.set_blkno(1);
						send_pak.set_data(r_mem);
					}
					send_buf = send_pak.encode();
				}
				else
				{
					//send error
				}
			}
		}

		if (sendto(sockfd, send_buf, buf_len, 0, (struct sockaddr*)&cli_addr, cli_size) < 0)	
		{
			std::cout << "sendto failed\n";
			std::cout << errno << "\n";
		}
	}

	close(sockfd);

	std::cout << "Jef\n";
	return 0;
}
