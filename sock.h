#include <iostream>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "pack.h"

class sock
{
	public:
		sock();
		sock(const char* ip);
		~sock();

		int set_send_pack(pack spak);
		int set_recv_pack(pack rpak);
		pack get_send_pack();
		pack get_recv_pack();
		char* get_recv_buf();

		int sock_bind();

		int get();
		int put();

		//char* encode();
		//void decode(char*);

		//int16_t get_opcode();
		//char* get_filename();
	//	char* get_mode();
		//char* get_data(); //data being in this class is QUESTIONABLE
	//	int16_t get_blkno();
	//	int16_t get_errno();
	//	char* get_errmsg();
//
//		int set_opcode(int16_t);
//		int set_filename(char*);
//		int set_mode(char*);
//		int set_data(char*);
//		int set_blkno(int16_t);
//		int set_errno(int16_t);
//		int set_errmsg(char*);

	private:
		int port;
		const int buf_len = 516;
		int sockfd;
		int recv_len;
		struct sockaddr_in srv_addr;
		struct sockaddr_in cli_addr;
		unsigned int cli_size;
		char* send_buf;
		char recv_buf[516];
		pack send_pack;
		pack recv_pack;
//		int16_t opcode;
//		char* filename;
//		char* mode;
//		char* data;
//		int16_t blkno;
//		int16_t errno;
//		char* errmsg;
};
