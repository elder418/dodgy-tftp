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
	pack get_pack, put_pack;
	//open ipv4 udp socket
	int err;

	sock srv_sock("192.168.1.69");
	if ((err = srv_sock.sock_bind()) < 0)
	{
		std::cout << "fatal: sock_bind(): " << errno << std::endl;
		return 1;
	}

	std::cout << "successfully bound." << std::endl;

	while (1)
	{
		if ((err = srv_sock.get()) < 0)
		{
			std::cout << "fatal: get(): " << errno << std::endl;
			return 1;
		}

		std::cout << "packet get!" << std::endl;

		get_pack = srv_sock.get_recv_pack(); //copy socket packet member into memory
		//identify packet by opcode and deal w/
		//for now just echo huehue
		switch (get_pack.get_opcode())
		{
			case 0: //invalid operation
			{
				//create error packet with error code 4 = illegal TFTP operation
				put_pack.set_opcode(5); //error operation
				put_pack.set_errno(4);
				put_pack.set_errmsg("illegal tftp opcode");
				srv_sock.set_send_pack(put_pack);
				break;
			}
			case 1: //RRQ
			{
				//read request, check for file, open and put in packet
				//send data
				if (strcmp(get_pack.get_filename(), "smallertest.txt") == 0) //see if reqd 'file' exists
				{
					std::streampos r_size;
					char* r_mem;
					//open file with ptr at end
					r_file.open("smallertest.txt", std::ios::in | std::ios::ate);
					r_size = r_file.tellg(); //get file size
					if (r_size > 512)
					{
						put_pack.set_opcode(5); //error operation
						put_pack.set_errno(666); //boot too big
						put_pack.set_errmsg("file too big");
						srv_sock.set_send_pack(put_pack);
						//big block do split
					}
					else
					{
						r_mem = new char[r_size];
						r_file.seekg(0, std::ios::beg);
						r_file.read(r_mem, r_size);
						r_file.close();
						//create data packet
						put_pack.set_opcode(3);
						put_pack.set_blkno(1);
						put_pack.set_data(r_mem);
						srv_sock.set_send_pack(put_pack);
					}
				}
				else //error packet, file not found.
				{
					put_pack.set_opcode(5); //error operation
					put_pack.set_errno(1); //file not found
					put_pack.set_errmsg("file not found");
					srv_sock.set_send_pack(put_pack);
				}
				break;
			}
			case 2: //WRQ
			{
				//write request, open file, read data, put in file
				//send ack
				break;
			}
			case 3: //DATA
			{
				//data block, track numbers, put in file
				//send ack
				break;
			}
			case 4: //ACK
			{
				//ack, block number being acknowledged, 0 for wrq special case
				//send next data block?
				break;
			}
			case 5: //ERROR
			{
				//recv error, cout cleanup and close?
				break;
			}
		}
		if ((err = srv_sock.put()) < 0)
		{
			std::cout << "fatal: put(): " << errno << std::endl;
			return 1;
		}

		std::cout << "packet sent" << std::endl;
	}
	std::cout << "Jef\n";
	return 0;
}
