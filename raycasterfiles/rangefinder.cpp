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
Name: draw_rectangle 
Description: Draws a solid-colored rectangle on a 1D framebuffer image.
The rectangle starts at (x_pos, y_pos) and spans rect_width × rect_height pixels.


Purpose: Enables pixel-level rectangle drawing into a linear framebuffer. 
--------------------------------------*/
void draw_rectangle(std::vector<uint32_t> &image, const size_t image_width, const size_t image_height, const size_t x_pos, const size_t y_pos, const size_t rect_width, const size_t rect_height, const uint32_t color){
	assert(image.size() == image_width * image_height);
	for(size_t i=0; i<rect_width;i++){
		for(size_t j=0; j<rect_height; j++){
			size_t cx = x_pos+i;
			size_t cy = y_pos+j;
			assert(cx<image_width && cy<image_height);
			image[cx + cy*image_width] = color;
		}
	}

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
	
	
	/*
	 Additions to main() function: A map structure with size variables has been added.
	 This map is represented by 16 16 character strings and a null terminator;. Empty
	 spaces represent an empty space on the map, 0, 1, 2, and 3, represent different 
	 types of wall textures to be included. 
	*/
	const size_t map_width = 16; 
	const size_t map_height = 16;
	const char map[] =  "0000222222220000"\
                      	    "1              0"\
                            "1      11111   0"\
                    	    "1     0        0"\
                       	    "0     0  1110000"\
                            "0     3        0"\
                            "0   10000      0"\
                            "0   0   11100  0"\
                            "0   0   0      0"\
                            "0   0   1  00000"\
                            "0       1      0"\
                            "2       1      0"\
                            "0       0      0"\
                            "0 0000000      0"\
                            "0              0"\
                            "0002222222200000"; 
	assert(sizeof(map) == map_width * map_height+1);
	
	float player_x = 3.456; //player x position 
	float player_y = 2.345; // player y position 
	float player_a = 1.523; //direction of the players gaze 
	

	for(size_t j=0; j<window_height; j++){
		for(size_t i=0; i<window_width; i++){
			uint8_t r = 255*j/float(window_height);
			uint8_t g = 255*i/float(window_width);
			uint8_t b = 0;
			framebuffer[i+j*window_height] = packcolor(r, g, b);
		}
	}
	
	


	/*
	 This loop is added to parse through the map for the variable stored at its
	 16x16 indexes. Based on the value scanned it then draws a rectangle into the 
	 frame buffer using the draw_rectangle function based on the corresponding 
	 value. 
	
	 This loop iterates over a 2D map (flattened into a 1D array of size map_width 
	 × map_height). For each non-space character found in the map, it calculates 
	 the screen position and draws a rectangle on the framebuffer using the 
	 draw_rectangle function. The rectangle's size is determined by dividing the 
	 window dimensions by the map dimensions, effectively scaling the 2D map to fit 
	 the screen.
	*/
	const size_t rect_width = window_width/map_width;
	const size_t rect_height = window_height/map_height;

	for(size_t j=0; j<map_height; j++){
		for(size_t i=0; i<map_width; i++){
			if(map[i+j*map_width]==' ') continue;
			size_t rect_x = i*rect_width;
			size_t rect_y = j*rect_height;
			draw_rectangle(framebuffer, window_width, window_height, rect_x, rect_y, rect_width, rect_height, packcolor(0, 255, 255));
		}

	}

	draw_rectangle(framebuffer, window_width, window_height, player_x*rect_width, player_y*rect_height, 5, 5, packcolor(255,255,255));//draw the player rectangle

	for(float t=0; t<20; t+=.05){
		float cx = player_x + t*cos(player_a);
		float cy = player_y + t*sin(player_a);
		if(map[int(cx)+int(cy)*map_width] != ' ')break;
		size_t pix_x = cx*rect_width;
		size_t pix_y = cy*rect_height;
		framebuffer[pix_x + pix_y*window_width] = packcolor(255, 255, 255);
	}
	drop_ppm_image("./outRangefinder.ppm", framebuffer, window_width, window_height);
	return 0;
}
