#include <iostream>
#include <fstream>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "pack.h"
#include "sock.h"

int main(int argc, char** argv)
{
	std::ifstream r_file;
	//open ipv4 udp socket
	int err;

	sock srv_sock("192.168.1.69");
	if ((err = srv_sock.sock_bind()) < 0)
	{
		std::cout << "fatal: sock_bind(): " << errno << std::endl;
		return 1;
	}

	std::cout << "successfully bound." << std::endl;

	if ((err = srv_sock.get()) < 0)
	{
		std::cout << "fatal: get(): " << errno << std::endl;
		return 1;
	}

	std::cout << "packet get!" << std::endl;

	pack get_pack = srv_sock.get_recv_pack(); //copy socket packet member into memory
	std::cout << get_pack.get_opcode() << std::endl;

//	while (1)
//	{
//		memset(&recv_buf, 0, sizeof(recv_buf));
//		memset(&send_buf, 0, sizeof(send_buf));
//		memset(&cli_addr, 0, sizeof(cli_addr));
//		std::cout << "listening..." << std::endl;
//
//		recv_len = recvfrom(sockfd, recv_buf, buf_len, 0, (struct sockaddr*)&cli_addr, &cli_size);
//		if (recv_len < 0)
//		{
//			std::cout << "recvfrom failed\n";
//			std::cout << errno << "\n";
//			return 1;
//		}
//
//		std::cout << "received packet from: " << inet_ntoa(cli_addr.sin_addr) << ":" << ntohs(cli_addr.sin_port) << std::endl;
//
//		recv_pak.decode(recv_buf);
//		std::cout << recv_pak.get_opcode() << recv_pak.get_filename() << recv_pak.get_mode() << std::endl;
//		switch (recv_pak.get_opcode())
//		{
//			case 0: //empty/invalid
//				std::cout << "invalid opcode\n"; //send error
//				break;
//			case 1: //RRQ
//			{
//				if (strcmp(recv_pak.get_filename(), "smallertest.txt") == 0) //see if req'd "file" exists
//				{	
//					std::streampos r_size;
//					char* r_mem;
//					//open file
//					r_file.open("smallertest.txt", std::ios::in | std::ios::ate | std::ios::binary);
//					r_size = r_file.tellg();
//					if (r_size > 512)
//					{
//						//BIG BLOCK SPLIT IT UP GEGYEGYEGYHUEUEHUE
//					}
//					else
//					{
//						r_mem = new char[r_size]; //CLEAN THIS UP SOMEWHERE D00d
//						r_file.seekg(0, std::ios::beg);
//						r_file.read(r_mem, r_size);
//						r_file.close();
//						//create data packet
//						send_pak.set_opcode(3);
//						send_pak.set_blkno(1);
//						send_pak.set_data(r_mem);
//						file_done = true;
//					}
//					send_buf = send_pak.encode();
//				}
//				else
//				{
					//send error
//				}
//			}
//			case 2: //wrq
//				break;
//			case 3: //data
//				break;
//			case 4: //ack
//				break;	
//		}
//		
//		std::cout << send_pak.get_opcode() << send_pak.get_blkno() << send_pak.get_data() << std::endl;
//		if (sendto(sockfd, send_buf, buf_len, 0, (struct sockaddr*)&cli_addr, cli_size) < 0)	
//		{
//			std::cout << "sendto failed\n";
//			std::cout << errno << "\n";
//		}
//	}

	std::cout << "Jef\n";
	return 0;
}
