#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc, char** argv)
{
	const int port = 69;		//tftp uses port 69 and
	const int buf_len = 512;	// 512 byte data chunks
	int sockfd, recv_len;
	unsigned int cli_size;
	struct sockaddr_in serv_addr, cli_addr;
	char buf[buf_len];
	//open ipv4 udp socket
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sockfd < 0)
	{
		std::cout << "failed to open socket\n";
		return 1;
	}

	serv_addr = {0};
	cli_addr = {0};

	serv_addr.sin_family = AF_INET;
	//insert current host ip into addr struct
	serv_addr.sin_addr.s_addr = inet_addr("192.168.1.69");//htonl(INADDR_ANY);
	//convert int to network byte order 
	serv_addr.sin_port = htons(port);

	std::cout << serv_addr.sin_addr.s_addr << ":" << serv_addr.sin_port << "\n"; 
	std::cout << sockfd << "\n";

	if (bind(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
	{
		std::cout << "failed to bind socket\n";
		std::cout << errno << "\n";
		return 1;
	}

	std::cout << "successfully bound " << serv_addr.sin_addr.s_addr << ":" << serv_addr.sin_port << "\n";
	
	while (1)
	{	
		std::cout << "listening..." << std::endl;

		if ((recv_len = recvfrom(sockfd, buf, buf_len, 0, (struct sockaddr*)&cli_addr, &cli_size)) < 0)
		//if ((recv_len = recvfrom(sockfd, buf, sizeof(buf), 0, NULL, NULL)) < 0)
		{
			std::cout << "recvfrom failed\n";
			std::cout << errno << "\n";
			return 1;
		}

		std::cout << "received packet from: " << inet_ntoa(cli_addr.sin_addr) << ":" << ntohs(cli_addr.sin_port) << std::endl;
		std::cout << "data: " << buf << std::endl;

		if (sendto(sockfd, buf, recv_len, 0, (struct sockaddr*)&cli_addr, cli_size) < 0)
		{
			std::cout << "sendto failed\n";
			std::cout << errno << "\n";
		}
	}

	close(sockfd);

	std::cout << "Jef\n";
	return 0;
}
