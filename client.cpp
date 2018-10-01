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

int die(const char* err)
{
	std::cout << "fatal: " << err << ": " << errno << std::endl;
	exit(1);
}

int main(int argc, char** argv)
{
	int err;
	std::ofstream w_file;

	sock cli_sock("192.168.1.69");

	pack rrq0, get_pack, put_pack;
	rrq0.set_opcode(1);
	rrq0.set_filename("smallertest.txt");
	rrq0.set_mode("octet");

	//send file request
	cli_sock.set_send_pack(rrq0);
	if ((err = cli_sock.put()) < 0)
		die("put()");

	while (1)
	{
		//listen
		if ((err = cli_sock.get()) < 0)
			die("get()");
		std::cout << "packet get!" << std::endl;
		get_pack = cli_sock.get_recv_pack();

		//identify and respond
		switch (get_pack.get_opcode())
		{
			case 0: //invalid operation
			{
				put_pack.set_opcode(5); //error op
				put_pack.set_errno(4);
				put_pack.set_errmsg("illegal tftp opcode");
				cli_sock.set_send_pack(put_pack);
				break;
			}
			case 1: //rrq and wrq invalid on client
			case 2:
			{
				put_pack.set_opcode(5); //error op
				put_pack.set_errno(4);
				put_pack.set_errmsg("illegal tftp client opcode");
				cli_sock.set_send_pack(put_pack);
				break;
			}
			case 3: //data
			{
				//open file and write
				//send ack
				w_file.open("out-client/smallertest.txt", std::ios::out);
				if (sizeof(get_pack.get_data()) == 512) //full data, so more to come (probably...)
				{
					//DEAL WITH BIG FILE
					put_pack.set_opcode(6);
					put_pack.set_errno(666);
					put_pack.set_errmsg("file too FUCKIN big");
					cli_sock.set_send_pack(put_pack);
				}
				else
				{
					//write last bit of data
					w_file.write(get_pack.get_data(), sizeof(get_pack.get_data()));
					w_file.close();
					put_pack.set_opcode(4);
					put_pack.set_blkno(1);
				}
				break;
			}
			case 4: //ack
			{
				//send next data
				break;
			}
			case 5: //error
			{
				//show error and terminate
				std::cout << get_pack.get_errno() << get_pack.get_errmsg() << std::endl;
				break;
			}
		}
	}
	
	return 0;
}
