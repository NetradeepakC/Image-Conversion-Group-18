#include"RAWtoIco.h"
#include"TifftoRAW.h"
#include<stdio.h>
#include<stdlib.h>
void rawtoico()
{
	FILE* fp1; int i,j,l;
    fp1=fopen("1.ico", "wb");

	struct ICONDIR icon_dir;
    icon_dir.reserved=0;
    icon_dir.imgType=1;
    icon_dir.no_of_imgs=1;

    fwrite(&icon_dir, sizeof(icon_dir), 1, fp1);

    struct ICONDIRENTRY icon_entry;
	if(image1.width==256) {icon_entry.width=0;}
    else if(image1.width>0) {icon_entry.width=image1.width;}
    if(image1.height==256) {icon_entry.height=0;}
	else if(image1.height>0) {icon_entry.height=image1.height;}
    icon_entry.colpal=0;
    icon_entry.reserved=0;    
    icon_entry.colorPlanes=1;
    icon_entry.bitsPerPixel=32;
    icon_entry.size_bitmapdata= 40 + (image1.width*4*image1.height) + (image1.width*4*image1.height)/32;
    icon_entry.offset_bmp=22;

    fwrite(&icon_entry, sizeof(icon_entry), 1,fp1);

    struct BITMAPINFOHEADER bitmapinfo = {40, image1.width, 2*image1.height, 1, 32, 0,(image1.width*4*image1.height) + (image1.width*4*image1.height)/32 , 7874 ,7874 ,0,0};

	fwrite(&bitmapinfo, sizeof(struct BITMAPINFOHEADER), 1,fp1);

    for(i=image1.width*(image1.height-1);i>=0;i-=image1.width)
	{
		for(j=i;j<i+image1.width;j++)
		{
			fputc(image1.image_data_B[j],fp1);
			fputc(image1.image_data_G[j],fp1);
			fputc(image1.image_data_R[j],fp1);
			fputc(255,fp1);
		}
	}

	for(l=0;l<(image1.height*image1.width)/8;l++)
	{
		fputc(0,fp1);
	}

    fclose(fp1);
}
