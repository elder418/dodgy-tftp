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
				if (strcmp(get_pack.get_filename(), "smalltest.txt") == 0) //see if reqd 'file' exists
				{
					std::streampos r_size;
					char* r_mem;
					//open file with ptr at end
					r_file.open(get_pack.get_filename(), std::ios::in | std::ios::ate);
					r_size = r_file.tellg(); //get file size
					if (r_size > 512)
					{
						pack_count = r_size / 512;
						if (r_size % 512 != 0)
							pack_count++;
						
						std::cout << "expected data packets: " << pack_count << std::endl;
						
						//send first 512 chuck
						r_mem = new char[512];
						r_file.seekg(0, std::ios::beg);
						r_file.read(r_mem, 512);
						//file offset + 512;
						put_pack.mk_DATA(1, r_mem);
						srv_sock.set_send_pack(put_pack);
						cur_blk = 1;
						//big block do split
						//r_mem = new char[512];
						//if (r_size % 512 == 0)
						//{
							//evenly divisible into 512 block chunks
						//	int pack_count = r_size / 512;
						//	int file_offset = 0;
						//	for (int i = 0; i < pack_count; i++)
						//	{
						//		r_file.seekg(file_offset, std::ios::beg);
						//		r_file.read(r_mem, 512);
						//		file_offset += 512;
						//		//create data packet
						//		put_pack.mk_DATA(i+1, r_mem);
						//		srv_sock.set_send_pack(put_pack);
						//		if ((err = srv_sock.put()) < 0)
						//		{
						//			std::cout << "fatal: put(): " << errno << std::endl;
						//			return 1;
						//		}

						//		std::cout << "packet sent (" << put_pack.get_blkno() << ")" << std::endl;
						//	}
						//}
						//else 
						//{
						//	//discard remainder
						//	int pack_count = (r_size / 512) + 1;
						//	int tail_size = r_size % 512;
						//	int file_offset = 0;
						//	for (int i = 1; i < pack_count; i++)
						//	{
						//		r_file.seekg(file_offset, std::ios::beg);
						//		r_file.read(r_mem, 512);
						//		file_offset += 512;
						//		//create data packet
						//		put_pack.mk_DATA(i, r_mem);
						//		srv_sock.set_send_pack(put_pack);
						//		if ((err = srv_sock.put()) < 0)
						//		{
						//			std::cout << "fatal: put(): " << errno << std::endl;
						//			return 1;
						//		}
//
//								std::cout << "packet sent (" << put_pack.get_blkno() << ")" << std::endl;
//							}
//							r_file.seekg(file_offset, std::ios::beg);
//							r_file.read(r_mem, tail_size);
							//create data packet
//							put_pack.mk_DATA(put_pack.get_blkno()+1, r_mem);
//							srv_sock.set_send_pack(put_pack);
//							if ((err = srv_sock.put()) < 0)
//							{
//								std::cout << "fatal: put(): " << errno << std::endl;
//								return 1;
//							}
//
//							std::cout << "packet sent (" << put_pack.get_blkno() << ")" << std::endl;
//						}
					}
					else
					{
						r_mem = new char[r_size];
						r_file.seekg(0, std::ios::beg);
						r_file.read(r_mem, r_size);
						r_file.close();
						//create data packet
						put_pack.mk_DATA(1, r_mem);
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
					int file_offset = cur_blk * 512;
					r_file.seekg(file_offset, std::ios::beg);
					r_file.read(r_mem, 512);
					put_pack.mk_DATA(cur_blk, r_mem);
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
