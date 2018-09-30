#include "packet.h"

packet::packet()
{
	opcode = 0;
	filename = NULL;
	mode = NULL;
	data = NULL;
	blkno = 0;
	errno = 0;
	errmsg = NULL;
}
packet::~packet()
{	
	opcode = 0;
	filename = NULL;
	mode = NULL;
	data = NULL;
	blkno = 0;
	errno = 0;
	errmsg = NULL;
}

int16_t packet::get_opcode()
{
	return opcode;
}
char* packet::get_filename()
{
	return filename;
}
char* packet::get_mode()
{
	return mode;
}
void* packet::get_data()
{
	return data;
}
int16_t packet::get_blkno()
{
	return blkno;
}
int16_t packet::get_errno()
{
	return errno;
}
char* packet::get_errmsg()
{
	return errmsg;
}

int packet::set_opcode(int16_t op)
{
	if (op > 0 && op < 6)
	{
		opcode = op;
		return 0;
	}
	else
		return 1;
}
int packet::set_filename(char* fn)
{
	filename = fn;
	return 0;
}
int packet::set_mode(char* md)
{
	if (md == "octet")
	{
		mode = md;
		return 0;
	}
	else
		return 1;
}
int packet::set_data(void* dat)
{
	if (sizeof(dat) <= 512)
	{
		data = dat;
		return 0;
	}
	else //make new data packet here??
		return 1;
}
int packet::set_blkno(int16_t bn)
{
	blkno = bn;
	return 0;
}
int packet::set_errno(int16_t en)
{
	errno = en;
	return 0;
}
int packet::set_errmsg(char* em)
{
	errmsg = em;
	return 0;
}
