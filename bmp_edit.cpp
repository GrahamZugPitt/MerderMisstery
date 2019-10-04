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
		//		Another quirk of bmp file is that the pixel data is written backwards
		//		This means pixels are written alpha, blue, green, red
		//
		//		For example an N x M image would be stored as:
		//			(N,0)-------------------------(N,M)
		//			|								  |
		//			|								  | where p = [alpha, blue, green , red]
		//			|								  |
		//			(0,0)-p-----------------------(0,M)
		//		https://en.wikipedia.org/wiki/BMP_file_format has an ok explaination of this file format
		//		The format this class supports uses the BITMAPV5HEADER format.
		//		If you have any questions on the format, ask me although I'M NO EXPERT ON IT.
		int setPixel(int x, int y, const char *red, const char *green, const char *blue, const char *alpha){
			outfile->seekp(bmp.offset + (y * dib.width + x) * 4, std::ios::beg);
			outfile->write(alpha, 1);
			outfile->write(blue, 1);
			outfile->write(green, 1);
			outfile->write(red, 1);
			outfile->seekp(0, std::ios::beg);
		}
};


int main(int argc, char **argv){
	
	if (argc < 2){
		std::cout << "No file specified...Exiting" << std::endl;
		return 1;
	}

	BMPMod img(argv[1], argv[2]);
	char r, g, b, a;
	r = 255;
	g = 255;
	b = 255;
	a = 255;
	img.setPixel(1,1, &r, &g, &b, &a);
	
}