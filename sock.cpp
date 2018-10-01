#include "sock.h"

sock::sock()
{
	port = 69;
	cli_size = sizeof(cli_addr);
	send_buf = new char[buf_len];
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	memset(&srv_addr, 0, sizeof(srv_addr));
	memset(&cli_addr, 0, sizeof(cli_addr));
	memset(&send_buf, 0, sizeof(send_buf));
	srv_addr.sin_family = AF_INET;
	srv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	srv_addr.sin_port = htons(port);
}
sock::sock(const char* ip)
{
	port = 69;
	cli_size = sizeof(cli_addr);
	send_buf = new char[buf_len];
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	memset(&srv_addr, 0, sizeof(srv_addr));
	memset(&cli_addr, 0, sizeof(cli_addr));
	memset(&send_buf, 0, sizeof(send_buf));
	srv_addr.sin_family = AF_INET;
	srv_addr.sin_addr.s_addr = inet_addr(ip);
	srv_addr.sin_port = htons(port);	
}

sock::~sock()
{
	delete send_buf;
	// clean up u lazy bitch
}

int sock::set_send_pack(pack spak)
{
	send_pack = spak;
	return 0;
}
int sock::set_recv_pack(pack rpak)
{
	recv_pack = rpak;
	return 0;
}
pack sock::get_send_pack()
{
	return send_pack;
}
pack sock::get_recv_pack()
{
	return recv_pack;
}

int sock::sock_bind()
{
	if (bind(sockfd, (struct sockaddr*)&srv_addr, sizeof(srv_addr)) < 0)
	{
		return errno;
	}
	return 0;
}

int sock::get() //get remote packet, decode into memory
{
	memset(&recv_buf, 0, sizeof(recv_buf));
	recv_len = recvfrom(sockfd, recv_buf, buf_len, 0, (struct sockaddr*)&cli_addr, &cli_size);
	if (recv_len < 0)
		return errno;
	recv_pack.decode(recv_buf);
}

int sock::put() //encode packet, send to remote
{
	memset(&send_buf, 0, sizeof(send_buf));
	send_buf = send_pack.encode();
	if (sendto(sockfd, send_buf, buf_len, 0, (struct sockaddr*)&cli_addr, cli_size) < 0)
		return errno;
	return 0;
}
