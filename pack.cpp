#include "pack.h"

pack::pack()
{
	opcode = 0;
	filename = new char[24];
	mode = new char[8];
	data = new char[512];
	blkno = 0;
	errno = 0;
	errmsg = new char[64];
}
pack::~pack()
{	
//	opcode = 0;
//	if (filename != NULL)
//		delete filename;
//	if (mode != NULL)
//		delete mode;
//	if (data != NULL)
//		delete data;
//	blkno = 0;
//	errno = 0;
//	if (errmsg != NULL)
//		delete errmsg;
}

void pack::zero()
{
	opcode = 0;
	filename = new char[24];
	memset(&filename, 0, 24);
	mode = new char[8];
	memset(&mode, 0, 8);
	data = new char[512];
	memset(&data, 0, 512);
	blkno = 0;
	errno = 0;
	errmsg = new char[64];
	memset(&errmsg, 0, 64);
}

char* pack::encode()
{
	char* ret = new char[516];
	switch (opcode)
	{
		case 0:	//empty packet
			return NULL;
			break; //pointless?
		case 1: //RRQ
		{
			ret[0] = opcode & 0xff00;
			ret[1] = opcode & 0x00ff;
			int x = 2;
			for (char* i = filename; *i; i++)
			{
				ret[x] = *i;
				x++;
			}
			ret[x] = 0;
			x++;
			for (char* i = mode; *i; i++)
			{
				ret[x] = *i;
				x++;
			}
			//fill rest of memory with 0
			while (x <= 516)
			{
				ret[x] = 0;
				x++;
			}
			return ret;
			break;
		}
		case 2: //WRQ
		{
			ret[0] = opcode & 0xff00;
			ret[1] = opcode & 0x00ff;
			int x = 2;
			for (char* i = filename; *i; i++)
			{
				ret[x] = *i;
				x++;
			}
			ret[x] = 0;
			x++;
			for (char* i = mode; *i; i++)
			{
				ret[x] = *i;
				x++;
			}
			//fill rest of memory with 0
			while (x <= 516)
			{
				ret[x] = 0;
				x++;
			}
			return ret;
			break;
		}
		case 3: //DATA
		{
			ret[0] = opcode & 0xff00;
			ret[1] = opcode & 0x00ff;
			ret[2] = blkno & 0xff00;
			ret[3] = blkno & 0x00ff;
			int x = 4;
			for (char* i = data; *i; i++)
			{
				ret[x] = *i;
				x++;
			}
			//fill rest of memory with 0
			while (x <= 516)
			{
				ret[x] = 0;
				x++;
			}
			return ret;
			break;
		}
		case 4: //ACK
		{
			ret[0] = opcode & 0xff00;
			ret[1] = opcode & 0x00ff;
			ret[2] = blkno & 0xff00;
			ret[3] = blkno & 0x00ff;
			int x = 4;
			//fill rest of memory with 0
			while (x <= 516)
			{
				ret[x] = 0;
				x++;
			}
			return ret;
			break;
		}
		case 5: //ERROR
		{
			ret[0] = opcode & 0xff00;
			ret[1] = opcode & 0x00ff;
			ret[2] = errno & 0xff00;
			ret[3] = errno & 0x00ff;
			int x = 4;
			for (char* i = errmsg; *i; i++)
			{
				ret[x] = *i;
				x++;
			}
			//fill rest of memory with 0
			while (x <= 516)
			{
				ret[x] = 0;
				x++;
			}
			return ret;
			break;
		}
		default:
			return NULL;
			break;
	}
}
void pack::decode(char* recv)
{
	int x = 0;
	opcode = recv[0] | recv[1];
	switch (opcode)
	{
		case 0:
			break;
		case 1: //RRQ
		{
			int i = 0;
			char* fn;
			x = 2;
			while (recv[x] != 0)
			{
				filename[x-2] = recv[x];
				x++;
			}
			x++; //skip null terminator
			while (recv[x] != 0)
			{
				mode[i] = recv[x];
				x++;
				i++;
			}
			break;
		}
		case 2: //WRQ
		{
			int i = 0;
			x = 2;
			while (recv[x] != 0)
			{
				filename[x-2] = recv[x];
				x++;
			}
			x++; //skip null terminator
			while (recv[x] != 0)
			{
				mode[i] = recv[x];
				x++;
				i++;
			}
			break;
		}
		case 3: //DATA
		{
			blkno = recv[2] | recv[3];
			x = 4;
			while (recv[x] != 0)
			{
				data[x-4] = recv[x];
				x++;
			}
			break;
		}
		case 4: //ACK
		{
			blkno = recv[2] | recv[3];
			break;
		}
		case 5: //ERROR
		{
			errno = recv[2] | recv[3];
			x = 4;
			while (recv[x] != 0)
			{
				errmsg[x-4] = recv[x];
				x++;
			}
		}
	}
}

int16_t pack::get_opcode()
{
	return opcode;
}
char* pack::get_filename()
{
	return filename;
}
char* pack::get_mode()
{
	return mode;
}
char* pack::get_data()
{
	return data;
}
int16_t pack::get_blkno()
{
	return blkno;
}
int16_t pack::get_errno()
{
	return errno;
}
char* pack::get_errmsg()
{
	return errmsg;
}

int pack::set_opcode(int16_t op)
{
	if (op > 0 && op < 6)
	{
		opcode = op;
		return 0;
	}
	else
		return 1;
}
int pack::set_filename(char* fn)
{
	filename = fn;
	return 0;
}
int pack::set_mode(char* md)
{
	mode = md;
	return 0;
}
int pack::set_data(char* dat)
{
	if (sizeof(dat) <= 512)
	{
		data = dat;
		return 0;
	}
	else //make new data packet here??
		return 1;
}
int pack::set_blkno(int16_t bn)
{
	blkno = bn;
	return 0;
}
int pack::set_errno(int16_t en)
{
	errno = en;
	return 0;
}
int pack::set_errmsg(char* em)
{
	errmsg = em;
	return 0;
}

void pack::mk_RRQ(char* fn, char* mde)
{
	this->set_opcode(1);
	this->set_filename(fn);
	this->set_mode(mde);
}

void pack::mk_WRQ(char* fn, char* mde)
{
	this->set_opcode(2);
	this->set_filename(fn);
	this->set_mode(mde);
}

void pack::mk_DATA(int16_t blk, char* dat)
{
	this->set_opcode(3);
	this->set_blkno(blk);
	this->set_data(dat);
}

void pack::mk_ACK(int16_t blk)
{
	this->set_opcode(4);
	this->set_blkno(blk);
}

void pack::mk_ERROR(int16_t en, char* em)
{
	this->set_opcode(5);
	this->set_errno(en);
	this->set_errmsg(em);
}
