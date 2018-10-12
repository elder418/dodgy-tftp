#ifndef _pack_guard
#define _pack_guard
#include <string.h>
#include <sys/types.h>
class pack
{
	public:
		pack();
		~pack();

		void zero();

		char* encode();
		void decode(char*);

		int16_t get_opcode();
		char* get_filename();
		char* get_mode();
		char* get_data(); //data being in this class is QUESTIONABLE
		int16_t get_blkno();
		int16_t get_errno();
		char* get_errmsg();

		int set_opcode(int16_t);
		int set_filename(char*);
		int set_mode(char*);
		int set_data(char*);
		int set_blkno(int16_t);
		int set_errno(int16_t);
		int set_errmsg(char*);

		void mk_RRQ(char* fn, char* mde);
		void mk_WRQ(char* fn, char* mde);
		void mk_DATA(int16_t blk, char* dat);
		void mk_ACK(int16_t blk);
		void mk_ERROR(int16_t en, char* em);

	private:
		int16_t opcode;
		char* filename;
		char* mode;
		char* data;
		int16_t blkno;
		int16_t errno;
		char* errmsg;
};
#endif
