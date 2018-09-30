#include <string.h>
#include <sys/types.h>
class packet
{
	public:
		packet();
		~packet();

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

	private:
		int16_t opcode;
		char* filename;
		char* mode;
		char* data;
		int16_t blkno;
		int16_t errno;
		char* errmsg;
};
