#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include"TifftoRAW.h"
unsigned long de_endianator(unsigned char new_byte,unsigned long *old_value,int i)
{
	if(endian==0)
	{
		*old_value+=new_byte*pow(256,i);//Small Endian
	}
	else
	{
		*old_value=(*old_value)*256+new_byte;//Big endian
	}
}
unsigned long input(unsigned int n,FILE *fp)
{
	unsigned long m=0;
	unsigned char temp=0;
	for(int i=0;i<n;i++)
	{
		fread(&temp,1,1,fp);//Reading 1 byte at a time
		de_endianator(temp,&m,i);//Concatenating new byte to current vallue according to endianess
	}
	return m;
}
unsigned int header(FILE *fp)
{
	fread(&endian,1,1,fp);//Reading endianess
	endian=(endian==77)?1:0;
	input(3,fp);//Next byte again tells the endianess, the 2 after that are to check whether the endianness is correct
	return input(4,fp);//return offset to first IFD
}
unsigned int IFD(FILE *fp)
{
	IFD_entry_counter=input(2,fp);//First 2 bytes denote the number of IFD entries
}
int IFD_Entry(FILE* fp)
{
	short tag_type=input(2,fp);
	input(2,fp);//Needn't store field value as it is already known
	switch(tag_type)//Currently only 2 IFD entries need to be accounted for as the image is assumed to be 3 samples per pixel,8 bits per sample
	{
		case 256:
			input(4,fp);//Tells the number of values, needn't store as an image can only have one width
			image1.width=input(2,fp);
			input(2,fp);//four bytes are allocated to each value stored in tiff, but as field value is short only first 2 are used
			break;
		case 257:
			input(4,fp);//Tells the number of values, needn't store as an image can only have one height
			image1.height=input(2,fp);
			input(2,fp);//four bytes are allocated to each value stored in tiff, but as field value is short only first 2 are used
			break;
		default:
		    input(8,fp);
	}
}
int Round(double n)//Rounding fraction to closest integer
{
	if(((int)(n*10))%10>=5)//checks if the first digit after decimal point>=5
	{
		return (int)n+1;
	}
	else
	{
		return (int)n;
	}
}
//Using a technique similar in concept to SSAA to dowscale
//Note: Aspect Ratio is not preserved for large files, and will be shrunk down to a 256x256 image in order to fascilitate transfer to ico
int Downscale_Width(unsigned char* p,int sample_no)
{
	unsigned char new_image[256][image1.height];
	int i,j,a,b,k;
	unsigned int sum;
	for(i=0;i<256;i++)
	{
		a=Round(i*image1.width/256.0);
		b=Round((i+1)*image1.width/256.0);//Pixels from b to a will be combined
		for(j=0;j<image1.height;j++)
		{
			sum=0;
			for(k=a;k<b;k++)
			{
				sum+=(*(p+j*image1.width+k))*(*(p+j*image1.width+k));//squaring and adding to account for pixel intensity
			}
			sum/=(b-a);//Finding average of pixel intensities
			sum=sqrt(sum);
			new_image[i][j]=(unsigned char)sum;
		}
	}
	switch(sample_no)
	{
		case 0:
			image1.image_data_R=(unsigned char*)realloc(image1.image_data_R,image1.height*256);//Assigning a new memory location to the array
			for(i=0;i<256;i++)
			{
				for(j=0;j<image1.height;j++)
				{
					*(image1.image_data_R+j*256+i)=new_image[i][j];//transferring data to it
				}
			}
		break;
		case 1://Similar to one above
			image1.image_data_G=(unsigned char*)realloc(image1.image_data_G,image1.height*256);
			for(i=0;i<256;i++)
			{
				for(j=0;j<image1.height;j++)
				{
					*(image1.image_data_G+j*256+i)=new_image[i][j];
				}
			}
		break;
		case 2://Similar to one above
			image1.image_data_B=(unsigned char*)realloc(image1.image_data_B,image1.height*256);
			for(i=0;i<256;i++)
			{
				for(j=0;j<image1.height;j++)
				{
					*(image1.image_data_B+j*256+i)=new_image[i][j];
				}
			}
		break;
	}
	return 0;
}
int Downscale_Height(unsigned char* p,int sample_no)//similar to one above, height instead of width
{
	unsigned char new_image[image1.width][256];
	int i,j,a,b,k;
	unsigned int sum;
	for(i=0;i<256;i++)
	{
		a=Round(i*image1.height/256.0);
		b=Round((i+1)*image1.height/256.0);
		for(j=0;j<image1.width;j++)
		{
			sum=0;
			for(k=a;k<b;k++)
			{
				sum+=(*(p+k*image1.width+j))*(*(p+k*image1.width+j));
			}
			sum/=(b-a);
			sum=sqrt(sum);
			new_image[j][i]=(unsigned char)sum;
		}
	}
	switch(sample_no)
	{
		case 0:
			image1.image_data_R=(unsigned char*)realloc(image1.image_data_R,image1.width*256);
			for(i=0;i<image1.width;i++)
			{
				for(j=0;j<256;j++)
				{
					*(image1.image_data_R+j*image1.width+i)=new_image[i][j];
				}
			}
		break;
		case 1:
			image1.image_data_G=(unsigned char*)realloc(image1.image_data_G,image1.height*256);
			for(i=0;i<image1.width;i++)
			{
				for(j=0;j<256;j++)
				{
					*(image1.image_data_G+j*image1.width+i)=new_image[i][j];
				}
			}
		break;
		case 2:
			image1.image_data_B=(unsigned char*)realloc(image1.image_data_B,image1.height*256);
			for(i=0;i<image1.width;i++)
			{
				for(j=0;j<256;j++)
				{
					*(image1.image_data_B+j*image1.width+i)=new_image[i][j];
				}
			}
		break;
	}
	return 0;
}
int ReadtoRAW(FILE *fp)//The function that will be called in order to canvert Tiff to a set of arrays
{
	unsigned int offset=header(fp);//Reading offset from header
	//Image data between header and IFD
	int length=(offset-8)/3;
	image1.image_data_R=(unsigned char*)malloc(length);
	image1.image_data_G=(unsigned char*)malloc(length);
	image1.image_data_B=(unsigned char*)malloc(length);
	for(unsigned int i=0;i<offset-8;i++)
	{
		if(i%3==0)
		{
			*(image1.image_data_R+i/3)=input(1,fp);
		}
		else if(i%3==1)
		{
			*(image1.image_data_G+i/3)=input(1,fp);
		}
		else
		{
			*(image1.image_data_B+i/3)=input(1,fp);
		}
	}
	IFD(fp);//Accounting for the IFD
	while(IFD_entry_counter>0)//Reading IFD entries while counting down no of IFD entries remaining
	{
		IFD_Entry(fp);
		IFD_entry_counter--;
	}
	input(4,fp);
	if(image1.width>256)
	{
		for(int i=0;i<3;i++)
		{
			Downscale_Width((i==0)?image1.image_data_R:(i==1)?image1.image_data_G:image1.image_data_B,i);//Downscaling the width of three samples
		}
		image1.width=256;
	}
	if(image1.height>256)
	{
		for(int i=0;i<3;i++)
		{
			Downscale_Height((i==0)?image1.image_data_R:(i==1)?image1.image_data_G:image1.image_data_B,i);//Downscaling the height of three samples
		}
		image1.height=256;
	}
	return 0;
}
