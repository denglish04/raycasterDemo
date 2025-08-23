#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>
#include <cassert>

/*-------------------------------------
Name: packcolor
Description: This function packs separate RGBA components into a single 32-bit integer
	in ABGR format, with alpha as the highest byte and red as the lowest byte. 

Purpose: This renderer builds a 2d array of pixels into an image in in memory, saving
	or writing out to an image file or displaying to the screen is done per pixel, 
	usually a single uint32_t and often using formats (e.g. BMP, PNG, PPM) that 
	expect packed pixels
--------------------------------------*/
uint32_t packcolor(const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a=255){
	return (a<<24) + (b<<16) + (g<<8) + r;
}


/*-------------------------------------
Name: unpack_color
Description: This function separates an ABGR component stored as a single 32-bit 
	integer into 4 RGBA components stored with 8-bit integers, with alpha as 
	the highest byte and red as the lowest byte. 

Purpose: This is useful when needing to modify, inspect, or output individual 
	color components from a packed pixel. For example, when applying 
	image effects, blending, or exporting to formats that expect raw RGB(A) 
	values. 
--------------------------------------*/
void unpack_color(const uint32_t &color, uint8_t &r, uint8_t &g, uint8_t &b, uint8_t &a){
	r = (color >> 0) & 255;
	g = (color >> 8) & 255; 
	b = (color >> 16) & 255; 
	a = (color >> 24) & 255; 
}


/*-------------------------------------
Name: drop_ppm_image
Description: Takes a filename, size variables, and a 2d image represented by a 
	flattened 1D of packed 32-bit pixels. This function writes the pixel data 
	to a PPM image file by using the RGB channels retrieved from unpacking each 
	pixel in the image array. 
	
Purpose: To export the renderer's pixel buffer as a standard image file. The PPM 
	(Portable Pixmap) format is simple and widely supported, making it 
	useful for debugging or viewing output without needing external image 
	libraries.
--------------------------------------*/
void drop_ppm_image(const std::string filename, const std::vector<uint32_t> &image, const size_t width, const size_t height){
	assert(image.size() == width *height);
	std::ofstream ofs(filename, std::ios::binary);
	ofs << "P6\n" << width << " " << height << "\n255\n";
	for (size_t i = 0; i < width*height; i++){
		uint8_t r, g, b, a;
		unpack_color(image[i], r, g, b, a);
		ofs << static_cast<char>(r) << static_cast<char>(g) << static_cast<char>(b); 
	}
	ofs.close();
}


/*-------------------------------------
Name: main
Description: Initializes a 512×512 framebuffer and fills it with a vertical–horizontal 
    gradient where the red channel varies by row and the green channel varies 
    by column. Each pixel is stored as a packed 32-bit color value. The final 
    framebuffer is then saved as a binary PPM (P6) image named "out.ppm".

Purpose: Acts as a minimal test program for the software renderer, demonstrating 
    the use of the framebuffer, color packing, and image output
--------------------------------------*/
int main(){
	const size_t window_width = 512; 
	const size_t window_height = 512;
	std::vector<uint32_t> framebuffer(window_width * window_height, 255);

	for(size_t j=0; j<window_height; j++){
		for(size_t i=0; i<window_width; i++){
			uint8_t r = 255*j/float(window_height);
			uint8_t g = 255*i/float(window_width);
			uint8_t b = 0;
			framebuffer[i+j*window_height] = packcolor(r, g, b);
		}
	}
	
	drop_ppm_image("./out.ppm", framebuffer, window_width, window_height);
	return 0;
}
