#include <iostream>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "pack.h"
#include "sock.h"

int print_hex(char* buf)
{
	for (int i = 0; i < strlen(buf); i++)
		printf("%x", buf[i]);
	return 0;
}

int main(int argc, char** argv)
{
	FILE* r_file;
	pack get_pack, put_pack;
	//open ipv4 udp socket
	int err;
	int pack_count = 0;
	int cur_blk = 0;

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


		get_pack = srv_sock.get_recv_pack(); //copy socket packet member into memory
		std::cout << "g:" << get_pack.get_opcode() << std::endl;
		//identify packet by opcode and deal w/
		switch (get_pack.get_opcode())
		{
			case 0: //invalid operation
			{
				put_pack.mk_ERROR(4, "illegal tftp opcode");
				srv_sock.set_send_pack(put_pack);
				break;
			}
			case 1: //RRQ
			{
				//read request, check for file, open and put in packet
				//send first data
				if (strcmp(get_pack.get_filename(), "testo.bmp") == 0) //see if reqd 'file' exists
				{
					std::streampos r_size;
					char* r_mem;
					//open file with ptr at end
					r_file = fopen(get_pack.get_filename(), "rb");
					fseek(r_file, 0, SEEK_END);
					r_size = ftell(r_file);
					rewind(r_file);
					if (r_size > 512)
					{
						pack_count = r_size / 512;
						if (r_size % 512 != 0)
							pack_count++;
						
						std::cout << "expected data packets: " << pack_count << std::endl;
						
						//send first 512 chuck
						r_mem = new char[512];
						fread(r_mem, 1, 512, r_file);
						//file offset + 512;
						put_pack.mk_DATA(1, r_mem);
						print_hex(put_pack.get_data());
						std::cout << std::endl << strlen(put_pack.get_data()) << std::endl;
						srv_sock.set_send_pack(put_pack);
						cur_blk = 1;
					}
					else
					{
						r_mem = new char[r_size];
						fread(r_mem, 1, r_size, r_file);
						//create data packet
						put_pack.mk_DATA(1, r_mem);
						print_hex(put_pack.get_data());
						std::cout << std::endl <<  strlen(put_pack.get_data()) << std::endl;
						srv_sock.set_send_pack(put_pack);
						cur_blk = 1;
					}
				}
				else //error packet, file not found.
				{
					put_pack.mk_ERROR(1, "file not found");
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
				//check if blocks are synced
				if (get_pack.get_blkno() == cur_blk && cur_blk < pack_count)
				{
					cur_blk++;
					char* r_mem = new char[512];
					//int file_offset = cur_blk * 512;
					fread(r_mem, 1, 512, r_file);
					put_pack.mk_DATA(cur_blk, r_mem);
					print_hex(put_pack.get_data());
					std::cout << std::endl << strlen(put_pack.get_data()) << std::endl;
					srv_sock.set_send_pack(put_pack);
				}
				else if (get_pack.get_blkno() == pack_count)
				{
					//last data acknowledged, done
					return 1;
					break;
				}
				else
				{
					put_pack.mk_ERROR(5, "unknown transfer id (blocks desynced?)");
					srv_sock.set_send_pack(put_pack);
				}
				break;
			}
			case 5: //ERROR
			{
				//recv error, cout cleanup and close?
				std::cout << get_pack.get_errno() << get_pack.get_errmsg() << std::endl;
				return 1;
				break;
			}
		}
		if ((err = srv_sock.put()) < 0)
		{
			std::cout << "fatal: put(): " << errno << std::endl;
			return 1;
		}
		std::cout << "s:" << put_pack.get_opcode() << std::endl;
	}
	std::cout << "Jef\n";
	return 0;
}
