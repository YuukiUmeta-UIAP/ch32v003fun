#include <stdio.h>
#include "wch_link_base.h"

// tested
//  Having some difficulty with the base.

const uint8_t * bootloader = (const uint8_t*)
"\x21\x11\x22\xca\x26\xc8\x93\x77\x15\x00\x99\xcf\xb7\x06\x67\x45" \
"\xb7\x27\x02\x40\x93\x86\x36\x12\x37\x97\xef\xcd\xd4\xc3\x13\x07" \
"\xb7\x9a\xd8\xc3\xd4\xd3\xd8\xd3\x93\x77\x25\x00\x9d\xc7\xb7\x27" \
"\x02\x40\x98\x4b\xad\x66\x37\x33\x00\x40\x13\x67\x47\x00\x98\xcb" \
"\x98\x4b\x93\x86\xa6\xaa\x13\x67\x07\x04\x98\xcb\xd8\x47\x05\x8b" \
"\x63\x16\x07\x10\x98\x4b\x6d\x9b\x98\xcb\x93\x77\x45\x00\xa9\xcb" \
"\x93\x07\xf6\x03\x99\x83\x2e\xc0\x2d\x63\x81\x76\x3e\xc4\xb7\x32" \
"\x00\x40\xb7\x27\x02\x40\x13\x03\xa3\xaa\xfd\x16\x98\x4b\xb7\x03" \
"\x02\x00\x33\x67\x77\x00\x98\xcb\x02\x47\xd8\xcb\x98\x4b\x13\x67" \
"\x07\x04\x98\xcb\xd8\x47\x05\x8b\x69\xe7\x98\x4b\x75\x8f\x98\xcb" \
"\x02\x47\x13\x07\x07\x04\x3a\xc0\x22\x47\x7d\x17\x3a\xc4\x79\xf7" \
"\x93\x77\x85\x00\xf1\xcf\x93\x07\xf6\x03\x2e\xc0\x99\x83\x37\x27" \
"\x02\x40\x3e\xc4\x1c\x4b\xc1\x66\x2d\x63\xd5\x8f\x1c\xcb\x37\x07" \
"\x00\x20\x13\x07\x07\x20\xb7\x27\x02\x40\xb7\x03\x08\x00\xb7\x32" \
"\x00\x40\x13\x03\xa3\xaa\x94\x4b\xb3\xe6\x76\x00\x94\xcb\xd4\x47" \
"\x85\x8a\xf5\xfe\x82\x46\xba\x84\x37\x04\x04\x00\x36\xc2\xc1\x46" \
"\x36\xc6\x92\x46\x84\x40\x11\x07\x84\xc2\x94\x4b\xc1\x8e\x94\xcb" \
"\xd4\x47\x85\x8a\xb1\xea\x92\x46\xba\x84\x91\x06\x36\xc2\xb2\x46" \
"\xfd\x16\x36\xc6\xf9\xfe\x82\x46\xd4\xcb\x94\x4b\x93\xe6\x06\x04" \
"\x94\xcb\xd4\x47\x85\x8a\x85\xee\xd4\x47\xc1\x8a\x85\xce\xd8\x47" \
"\xb7\x06\xf3\xff\xfd\x16\x13\x67\x07\x01\xd8\xc7\x98\x4b\x21\x45" \
"\x75\x8f\x98\xcb\x52\x44\xc2\x44\x61\x01\x02\x90\x23\x20\xd3\x00" \
"\xf5\xb5\x23\xa0\x62\x00\x3d\xb7\x23\xa0\x62\x00\x55\xb7\x23\xa0" \
"\x62\x00\xc1\xb7\x82\x46\x93\x86\x06\x04\x36\xc0\xa2\x46\xfd\x16" \
"\x36\xc4\xb5\xf2\x98\x4b\xb7\x06\xf3\xff\xfd\x16\x75\x8f\x98\xcb" \
"\x41\x89\x05\xcd\x2e\xc0\x0d\x06\x02\xc4\x09\x82\xb7\x07\x00\x20" \
"\x32\xc6\x93\x87\x07\x20\x98\x43\x13\x86\x47\x00\xa2\x47\x82\x46" \
"\x8a\x07\xb6\x97\x9c\x43\x63\x1c\xf7\x00\xa2\x47\x85\x07\x3e\xc4" \
"\xa2\x46\x32\x47\xb2\x87\xe3\xe0\xe6\xfe\x01\x45\x61\xb7\x41\x45" \
"\x51\xb7\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff" \
"\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff" \
"\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff";

int bootloader_len = 512;


int main( int argc, char ** argv )
{
	int i;
	int status;
	uint8_t rbuff[1024];

	if( argc != 2 )
	{
		fprintf( stderr, "Usage: wch_write_simple [binfile]\n" );
	}
	
	FILE * f = fopen( argv[1], "rb" );
	fseek( f, 0, SEEK_END );
	int len = ftell( f );
	fseek( f, 0, SEEK_SET );
	int padlen = ((len-1) & (~0x3f)) + 0x40;
	char * image = malloc( padlen );
	memset( image, 0xff, padlen );
	status = fread( image, len, 1, f );
	fclose( f );
	
	if( status != 1 )
	{
		fprintf( stderr, "Error: File I/O Fault.\n" );
		return -10;
	}
	if( len > 16384 )
	{
		fprintf( stderr, "Error: Image for CH32V003 too large\n" );
		return -9;
	}
	
	libusb_device_handle * devh = wch_link_base_setup();
	int transferred;
	wch_link_command( devh, "\x81\x06\x01\x01", 4, 0, 0, 0 );
	wch_link_command( devh, "\x81\x06\x01\x01", 4, 0, 0, 0 ); // Not sure why but it seems to work better when we request twice.

	// This contains the write data quantity, in bytes.  (The last 2 octets)
	// Then it just rollllls on in.
	char rksbuff[11] = { 0x81, 0x01, 0x08, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
	rksbuff[9] = len >> 8;
	rksbuff[10] = len & 0xff;
	wch_link_command( devh, rksbuff, 11, 0, 0, 0 );
	
	wch_link_command( devh, "\x81\x02\x01\x05", 4, 0, 0, 0 );
	
	int pplace = 0;
	for( pplace = 0; pplace < bootloader_len; pplace += 64 )
	{
		WCHCHECK( libusb_bulk_transfer( devh, 0x02, (uint8_t*)(bootloader+pplace), 64, &transferred, WCHTIMEOUT ) );
	}
	
	for( i = 0; i < 10; i++ )
	{
		wch_link_command( devh, "\x81\x02\x01\x07", 4, &transferred, rbuff, 1024 );
		if( transferred == 4 && rbuff[0] == 0x82 && rbuff[1] == 0x02 && rbuff[2] == 0x01 && rbuff[3] == 0x07 )
		{
			break;
		}
	} 
	if( i == 10 )
	{
		fprintf( stderr, "Error, confusing respones to 02/01/07\n" );
		exit( -109 );
	}
	
	wch_link_command( devh, "\x81\x02\x01\x02", 4, 0, 0, 0 );

	for( pplace = 0; pplace < padlen; pplace += 64 )
	{
		WCHCHECK( libusb_bulk_transfer( devh, 0x02,image+pplace, 64, &transferred, WCHTIMEOUT ) );
	}

	// Waiting or something on 2.46.2???????  I don't know why the main system does this.
//	WCHCHECK( libusb_bulk_transfer( devh, 0x82, rbuff, 1024, &transferred, 2000 ) ); // Ignore respone.

	// Issue reset (this is optional)
	wch_link_command( devh, "\x81\x0b\x01\x01", 4, 0, 0, 0 );

	// Closeout
	wch_link_command( devh, "\x81\x0d\x01\xff", 4, 0, 0, 0);
}
