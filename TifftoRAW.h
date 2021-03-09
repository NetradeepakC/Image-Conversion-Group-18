#include<stdio.h>
# pragma pack(1)
unsigned short endian;
unsigned int IFD_entry_counter;
struct image//Needed for all c files
{
	unsigned char* image_data_R;
	unsigned char* image_data_G;
	unsigned char* image_data_B;
	unsigned short height;
	unsigned short width;
};
struct image image1;
unsigned long de_endianator(unsigned char new_byte,unsigned long *old_value,int i);
unsigned long input(unsigned int n,FILE *fp);
unsigned int header(FILE *fp);
unsigned int IFD(FILE *fp);
int IFD_Entry(FILE* fp);
int Round(double n);
int Downscale_Width(unsigned char* p,int sample_no);
int Downscale_Height(unsigned char* p,int sample_no);
int ReadtoRAW(FILE* fp);
