#ifndef _texture_bmp_h
#define _texture_bmp_h

class TextureBMP
{
private:
	unsigned char* data;	//实际数据
	unsigned int width;		//图片宽度
	unsigned int height;	//图片高度

public:
	TextureBMP(const char* imagePath);
	~TextureBMP();
	unsigned int Width();
	unsigned int Height();
	unsigned int ImageSize();
	unsigned int PixelColor(int y,int x);
};

#endif