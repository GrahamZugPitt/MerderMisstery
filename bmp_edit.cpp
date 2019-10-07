#include <fstream>
#include <iostream>
/*  An example c++ class to modify Windows style .bmp images
*  	Created by: Erick Brindock 10/3/19
*	
*	There is only one format supported by this class to simplify its implementation.
*
*
*/

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

		// Prints the relevant header data to standard output
		void printStats(bmpHeader bmp, dibHeader dib){
			std::cout << "Filesize: " << bmp.filesize << " Offset: " << bmp.offset << std::endl;
			std::cout << "Header size: " << dib.headerSize << std::endl;
			std::cout << "width: " << dib.width << std::endl;
			std::cout << "height: " << dib.height << std::endl;
			std::cout << "color panes: " << dib.panes << std::endl;
			std::cout << "bits per pixel: " << dib.bpp << std::endl;
		}

	public:
		BMPMod(char *ifilename, char *ofilename) {
			char *byte;  // buffer to hold file
			// open input file
			infile = new std::ifstream(ifilename, std::ios::binary | std::ios::in);
			// open output file in in/out mode
			// in/out needed to avoid truncation of file when writing a byte
			outfile = new std::fstream(ofilename, std::ios::binary | std::ios::out | std::ios::in);
			if (!outfile->is_open()){
				// ouflie DNE so make it, close it and open it as iofile
				std::ofstream makefile(ofilename, std::ios::out);
				makefile.close();
				outfile = new std::fstream(ofilename, std::ios::binary | std::ios::out | std::ios::in);	
			}
			if(infile->is_open()){
				if(validateBMP(infile) != 0){
					// throw exception if file is not a valid format
					throw "Invalid File format";
				}
				if (outfile->is_open()){
					// get file size
					infile->seekg(0, std::ios::end);
					long filesize = infile->tellg();
					// reset file pointer to begining of file
					infile->seekg(0, std::ios::beg);
					// set up buffer for file
					// as long a file is not huge and only one is used at a time
					// memory use should be small
					char *buffer = new char[filesize];
					// read file to buffer
					infile->read(buffer, filesize);
					// write buffer to file
					outfile->write(buffer, filesize);
					// free memory
					delete[] buffer;
				}
			} else {
				std::cerr << "File not found" << std::endl;
			}
		}
		// Destructor
		//  Closes files if still open.
		~BMPMod(){
			if ( infile->is_open()){
				infile->close();
			}
			if ( outfile->is_open()){
				outfile->close();
			}
		}

		// ensures bmp file is valid and supported
		// Args:
		//		infile: a read only file stream
		// Returns:
		//		0: on success
		//		1: invalid file type
		//		2: invlaid format
		int validateBMP(std::ifstream *infile){
			const std::string fext("BM");
			char filetype[3] = {'\0', '\0', '\0'};
			if (infile->is_open()){
				infile->read(filetype, 2);
				if (fext.compare(filetype) != 0){
					std::cerr << "Invalid file type: " << filetype << std::endl;
					infile->close();
					return 1;
				} else {
					infile->read((char *)&bmp, sizeof(bmpHeader));
					infile->read((char *)&dib, sizeof(dibHeader));
					if (dib.headerSize != 124){
						std::cerr << "File not supported" << std::endl;
						return 2;
					}
					#ifdef DEBUG
					printStats(bmp, dib);
					#endif
				}
			}
			return 0;
		}

		// This sets the pixel at (x,y) to the colors defineed by red, green, blue, and alpha
		// Notes on bmp files:
		//		.bmp files start their coordinates with (0, 0) being the lower left corner.
		//		This is like cartesian coordinates instead of normal computer graphics coordiane systems
		//		Another quirk of the bmp format is that the bytes are written "backwards".
		//		Thus, they should written in the order blue, green, red, alpha.
		//          
		//		For example an N x M image would be stored as:
		//			(N,0)-------------------------(N,M)
		//			|								  |
		//			|								  | where p = [blue, green, red, alpha]
		//			|								  |
		//			(0,0)-p-----------------------(0,M)
		//		https://en.wikipedia.org/wiki/BMP_file_format has an ok explaination of this file format
		//		The format this class supports uses the BITMAPV5HEADER format.
		//		If you have any questions on the format, ask me although I'M NO EXPERT ON IT.
		int setPixel(int x, int y, unsigned char r, unsigned char g, unsigned char b, unsigned char a){
			if (x > dib.width) {std::cerr << "x dim exceeded" << std::endl; return 1;}
			if (y > dib.height) {std::cerr << "y dim exceeded" << std::endl; return 2;}
			outfile->seekp(bmp.offset + (y * dib.width + x) * 4, std::ios::beg);
			outfile->write((char *)&b, 1);
			outfile->write((char *)&g, 1);
			outfile->write((char *)&r, 1);
			outfile->write((char *)&a, 1);
			return 0;
		}
	// Overloaded version of swapColor allowing definition of the colors according to their specific channels.
	// Args:
	//	ri, gi, bi, ai: the red, green, blue, and alpha channels respectively of the color to be changed
	//	ro, go, bo, ao: the red, green, blue, and alpha channels respectively of the color to change it to
	unsigned int swapColor(unsigned char ri, unsigned char gi, unsigned char bi, unsigned char ai,
					unsigned char ro, unsigned char go, unsigned char bo, unsigned char ao){
		// convert the channels to the proper integer values
		unsigned int in, out;
		in = (unsigned int)ai << 24 | (unsigned int)ri << 16 | (unsigned int)gi << 8 | (unsigned int)bi;
		out = (unsigned int)ao << 24 | (unsigned int)ro << 16 | (unsigned int)go << 8 | (unsigned int)bo;
		// pass the values to the overloaded method
		return swapColor(in , out);
	}

	// Swaps all the pixels of one color to naother in the image
	//	Args:
	//	in: the color to change
	//	out: the color to change to
	//		The bits of these functions should be 0xAARRGGBB where AA is alpha RR is red
	//		GG is green, BB is blue
	//
	//	PROGRAMMING NOTE:-----------------------------------------------------------------------------
	//		It seems that the write method writes data bytewise in a "small endian" style.
	//		This means the least significant byte (BB) is written first. This is followed by (GG).
	//		Then (RR), and finally (AA).
	//
	//		Another way to think of this using pointer arithmetic.
	//		Picture and unsigned int as an array of 4 chars (char[4]).
	//		Then writing an int is like:
	//		char *cptr = &intVal;
	//		for(int i = 0; i < 4; i++){
	//			outfile->write(cptr++, 1);
	//		}
	unsigned int swapColor(unsigned int in, unsigned int out){
		// Ignore header info
		outfile->seekg(bmp.offset, std::ios::beg);
		unsigned int currPix = 0;
		while(!(outfile->eof())){
			// get pixel color
			outfile->read((char *)&currPix, sizeof(currPix));
			if(currPix == in){
				// replace pixel of desired color
				outfile->seekp((int)(outfile->tellg())-4, std::ios::beg);
				outfile->write((char *)&out, sizeof(out));
			}
		}
		// Clear end of file flag to allow further operations
		outfile->clear();
		return 0;
	}	

};


int main(int argc, char **argv){
	
	if (argc < 2){
		std::cout << "No file specified...Exiting" << std::endl;
		return 1;
	}

	BMPMod img(argv[1], argv[2]);
	unsigned char r = 255;
	unsigned char g = 255;
	unsigned char b = 0;
	unsigned char a = 255;
	
	unsigned int in = 0xFF0000FF;
	unsigned int out = 0xFFAAAAAA;
	img.swapColor(in, out);

	in = 0xFF00FF00;
	out = 0xFFFFFFFF;
	img.swapColor(in, out);

	in = 0xFFFFFF00;
	out = 0xFF674434;
	img.swapColor(in, out);
	//img.swapColor(0,0, 0, 255, 0, 0, 255, 125);

	//img.setPixel(1,1, r, g, b, a);
	//img.setPixel(1,2, r, g, b, a);
}