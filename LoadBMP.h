#pragma once


class LoadBMP {
private:
	char* path;
	unsigned char header[54];
	unsigned int dataPos;
	unsigned int width, height;
	unsigned int imageSize; // width*height*3
	unsigned char* data = nullptr;
	char* MSG;
public:
	LoadBMP(char* path);
	~LoadBMP();
	void setPath(char* path);
	char* getPath();
	unsigned int getWidth();
	unsigned int getHeight();
	bool loadData();
	const char* getMSG();
	const unsigned char* getData();
};

