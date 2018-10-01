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

	while (1)
	{
		if ((err = srv_sock.get()) < 0)
		{
			std::cout << "fatal: get(): " << errno << std::endl;
			return 1;
		}

		std::cout << "packet get!" << std::endl;

		pack get_pack = srv_sock.get_recv_pack(); //copy socket packet member into memory
		//identify packet by opcode and deal w/
		//for now just echo huehue
		switch (get_pack.get_opcode())
		{
			case 0: //invalid operation
				//create error packet with error code 4 = illegal TFTP operation
				break;
		}
		pack put_pack = get_pack;
		srv_sock.set_send_pack(put_pack);
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
