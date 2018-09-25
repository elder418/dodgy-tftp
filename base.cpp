#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc, char** argv)
{
	int sockfd, port;
	struct sockaddr_in serv_addr;
	char* buf = "old message huhuhu";
	//open ipv4 udp socket
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sockfd < 0)
	{
		std::cout << "failed to open socket\n";
		return 1;
	}

	port = 420;
	serv_addr.sin_family = AF_INET;
	//insert current host ip into addr struct
	serv_addr.sin_addr.s_addr = inet_addr("192.168.1.69");
	//convert int to network byte order 
	serv_addr.sin_port = htons(port);

	std::cout << serv_addr.sin_addr.s_addr << ":" << serv_addr.sin_port << "\n"; 
	std::cout << sockfd << "\n";
	
	if (bind(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
	{
		std::cout << "failed to bind socket\n";
		std::cout << errno;
		return 1;
	}

	std::cout << "successfully bound " << serv_addr.sin_addr.s_addr << ":" << serv_addr.sin_port << "\n";

	sendto(sockfd, "sending jefs", sizeof("sending jefs"), 0, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
	recvfrom(sockfd, buf, sizeof(buf), 0, (struct sockaddr*)&serv_addr, (unisnged int*)sizeof(serv_addr));

	std::cout << "MSG: " << buf << "\n";

	close(sockfd);

	std::cout << "Jef\n";
	return 0;
}
