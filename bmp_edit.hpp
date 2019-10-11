#include <fstream>
#include <iostream>
class BMPMod{
private:
	struct bmpHeader{
		int filesize;     // The total file size
		int reservedVals; // idk. (You don't need this)
		int offset;		  // The byte where the pixel array starts
	}bmp;

	struct dibHeader{
		int headerSize;  // Used to determine formating (Don't worry about it)
		int width;       // The width of the image (x dimension)
		int height;		 // The height of the image (y dimension)
		short panes;	 // Should be 1 (Not sure why, but should be 1)
		short bpp;		 // The number of bits per pixel (32 for our format)
	}dib;

	std::ifstream *infile; // The orignal file
	std::fstream *outfile; // The output file

	void printStats(bmpHeader bmp, dibHeader dib);

public:
	BMPMod(std::string ifilename, std::string ofilename);
	
	~BMPMod();
	
	int validateBMP(std::ifstream *infile);

	int setPixel(int x, int y, unsigned char r, unsigned char g, unsigned char b, unsigned char a);

	unsigned int swapColor(unsigned char ri, unsigned char gi, unsigned char bi, unsigned char ai,
					unsigned char ro, unsigned char go, unsigned char bo, unsigned char ao);

	unsigned int swapColor(unsigned int in, unsigned int out);
};