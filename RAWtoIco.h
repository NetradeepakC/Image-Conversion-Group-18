#pragma pack (1)
struct ICONDIR{
    unsigned short int reserved;
    unsigned short int imgType;
    unsigned short int no_of_imgs;
};

struct ICONDIRENTRY{
    unsigned char width;
    unsigned char height;
    unsigned char colpal;
    unsigned char reserved;
    unsigned short int colorPlanes;
    unsigned short int bitsPerPixel;
    unsigned int size_bitmapdata; //bitmapinfoheader + imagesdata + maskdata
    unsigned int offset_bmp;
};

struct BITMAPINFOHEADER{
    unsigned int header_size; //size of this header = 40bytes
    int width;
    int height;
    unsigned short int colorPlanes; //01
    unsigned short int bitsPerPixel; //32 ie 20 in hex
    unsigned int compMethod; //0
    unsigned int sizeofrawbitmapdata;  //imagepixel data + maskpixel data 
    unsigned int horRes;  
    unsigned int vertRes;
    unsigned int colorPalatte;
    unsigned int impColors;
};
void rawtoico();
//struct image
//{
//	unsigned char* image_data_R;
//	unsigned char* image_data_G;
//	unsigned char* image_data_B;
//	unsigned short height;
//	unsigned short width;
//};
//struct image image1;
