#include <stdio.h>
#include <string.h>

char nes_chars[] = { 'A', 'P', 'Z', 'L', 'G', 'I', 'T', 'Y', 'E', 'O', 'X', 'U', 'K', 'S', 'V', 'N' };

unsigned int nes_char_int(char c) // inefficient but effective
{
	for(int i=0; i < 16; i++)
	{
		if( c == nes_chars[i] )
			return i;
	}
	return 0;
}

void hex_to_nes(unsigned int address, unsigned int data, unsigned int compare)
{
	int n[8];
	for(int i=0; i < 8; i++)
	{
		n[i] = 0;
	}

	n[0] |= 7 & data; //
	n[0] |= 8 & (data >> 4); //
	
	n[1] |= 8 & ( address >> 4 );
	n[1] |= 7 & ( data >> 4 ); //

	n[2] |= 7 & (address >> 4);

	n[3] |= 7 & (address >> 12);
	n[3] |= 8 & address;

	n[4] |= 7 & address;
	n[4] |= 8 & (address >> 8);

	n[5] |= 7 & (address >> 8);
	n[5] |= 8 & compare; //

	n[6] |= 7 & compare;
	n[6] |= 8 & (compare >> 4);

	n[7] |= 7 & (compare >> 4);
	n[7] |= 8 & data;

	for(int i=0; i < 8; i++)
	{
		printf("%c", nes_chars[ n[i] ] );
	}
	printf("\n");
}

void hex_to_nes6(unsigned int address, unsigned int data)
{
	int n[8];
	for(int i=0; i < 6; i++)
	{
		n[i] = 0;
	}

	n[0] |= 7 & data;
	n[0] |= 8 & (data >> 4);
	
	n[1] |= 8 & ( address >> 4 );
	n[1] |= 7 & ( data >> 4 );

	n[2] |= 7 & (address >> 4);
	n[2] |= 8;

	n[3] |= 7 & (address >> 12);
	n[3] |= 8 & address;

	n[4] |= 7 & address;
	n[4] |= 8 & (address >> 8);

	n[5] |= 7 & (address >> 8);
	n[5] |= 8 & data;

	for(int i=0; i < 6; i++)
	{
		printf("%c", nes_chars[ n[i] ] );
	}
	printf("\n");
}

void nes_to_hex(char *code)
{
	int data, address, compare;
	int n[8];
	int len = strlen(code);

	for( int i=0; i < len; i++)
	{
		n[i] = nes_char_int(code[i]);
	}

	if( len == 6 | len == 8 )
	{
		address = 0x8000 + 
			((n[3] & 7) << 12) |
			((n[5] & 7) << 8) | ((n[4] & 8) << 8) |
			((n[2] & 7) << 4) | ((n[1] & 8) << 4) |
			 (n[4] & 7)       |  (n[3] & 8);
	}

	if( len == 6 )
	{
		data = ((n[1] & 7) << 4) | ((n[0] & 8) << 4) | (n[0] & 7) | (n[5] & 8);
		printf("address data : 0x%04x 0x%02x", address, data);
	}
	else if( len == 8 )
	{
		 data = ((n[1] & 7) << 4) | ((n[0] & 8) << 4) | (n[0] & 7) | (n[7] & 8);
		 compare = ((n[7] & 7) << 4) | ((n[6] & 8) << 4) | (n[6] & 7) |  (n[5] & 8);
		printf("address data compare : 0x%04x 0x%02x 0x%02x\n", address, data, compare);
	}
}

void usage(char *self)
{
	printf("Usage: %s { address data [compare] | nescode }\n"
		"  e.g.:\n"
		"  %s 0xd1dd 0x014\n"
		"  %s GOSSIP ; %s ZEXPYGLA\n"
	, self, self, self, self);
}

int main(int argc, char **argv)
{
	unsigned int address, data, compare;
	if( argc == 4 )
	{
		sscanf(argv[1], "0x%x", &address);
		sscanf(argv[2], "0x%x", &data);
		sscanf(argv[3], "0x%x", &compare);
		hex_to_nes(address, data, compare);
	}
	else if( argc == 3 ) // for six char code
	{
		sscanf(argv[1], "0x%x", &address);
		sscanf(argv[2], "0x%x", &data);
		hex_to_nes6(address, data);
	}
	else if( argc == 2 ) // assume GG code
	{
		nes_to_hex(argv[1]);
	}
	else
	{
		usage(argv[0]);
	}
	return 0;
}

