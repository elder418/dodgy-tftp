#include <iostream>
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
	return 1;
}

int main(int argc, char** argv)
{
	int err;

	sock cli_sock("192.168.1.69");

	pack rrq0, recv_pak;
	rrq0.set_opcode(1);
	rrq0.set_filename("smallertest.txt");
	rrq0.set_mode("octet");

	std::cout << rrq0.get_opcode() << rrq0.get_filename() << std::endl;

	//send file request
	cli_sock.set_send_pack(rrq0);
	if ((err = cli_sock.put()) < 0)
		return die("put()");

	while (1)
	{
		if ((err = cli_sock.get()) < 0)
			return die("get()");
		recv_pak = cli_sock.get_recv_pack();
		std::cout << recv_pak.get_opcode() << recv_pak.get_filename() << recv_pak.get_mode() << std::endl;
	}
	
	return 0;
}
