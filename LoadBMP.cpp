#define _CRT_SECURE_NO_WARNINGS
#include "LoadBMP.h"
#include <stdio.h>

LoadBMP::LoadBMP(char* path) {
	this->path = path;
}
LoadBMP::~LoadBMP() {
	if (data != nullptr)
		delete data;
}
void LoadBMP::setPath(char* path) {
	this->path = path;
}
char* LoadBMP::getPath() {
	return this->path;
}
unsigned int LoadBMP::getWidth()
{
	return width;
}
const unsigned char* LoadBMP::getData()
{
	return data;
}
unsigned int LoadBMP::getHeight()
{
	return height;
}
bool LoadBMP::loadData() {
	if (data != nullptr)
		delete data;

	if (this->path == nullptr) {
		MSG = (char*)"경로가 존재하지 않습니다.\n";
		return false;
	}
	FILE* file = fopen(this->path, "rb");
	if (!file) {
		MSG = (char*)"파일을 열 수 없습니다.\n";
		return false;
	}
	if (fread(header, 1, 54, file) != 54) {
		MSG = (char*)"BitMap 파일이 아닙니다.";
		return false;
	}
	if (header[0] != 'B' || header[1] != 'M') {
		MSG = (char*)"BitMap 파일이 아닙니다.";
		return false;
	}
	dataPos = *(unsigned int*)&(header[0x0A]);
	imageSize = *(unsigned int*)&(header[0x22]);
	width = *(unsigned int*)&(header[0x12]);
	height = *(unsigned int*)&(header[0x16]);
	if (imageSize == 0)
		imageSize = width * height * 3;
	if (dataPos == 0)
		dataPos = 54;
	data = new unsigned char[imageSize];
	fread(data, 1, imageSize, file);
	fclose(file);
	unsigned char tmp;
	for (unsigned int i = 0; i < imageSize; i += 3) {
		tmp = data[i];
		data[i] = data[i + 2];
		data[i + 2] = tmp;
	}
	return true;
}

const char* LoadBMP::getMSG()
{
	return MSG;
}
