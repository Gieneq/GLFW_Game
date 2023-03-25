#include "ImageLoader.h"

#include <iostream>
#include <Settings.h>
#include <IO.h>

//store image data in GPU memory
bool store_in_gpu_memory(std::vector<unsigned char>& image, ImageData& data) {
    GLuint texture_id;
    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, data.width, data.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.data());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    
    if(texture_id == 0) {
        std::cout << "Error creating texture" << std::endl;
        return false;
    } else {
        std::cout << "Texture created successfully with id: " << texture_id << "." << std::endl;
    }

    data.texture_id = texture_id;

    glBindTexture(GL_TEXTURE_2D, 0);
    return true;
}


ImageLoader& ImageLoader::get_loader() {
    static ImageLoader loader{};
    return loader;
}


ImageData& ImageLoader::get_image(ImageId key_name) {
    return ImageLoader::images[key_name];
}


bool ImageLoader::has_image(ImageId key_name) {
    return ImageLoader::images.find(key_name) != ImageLoader::images.end();
}


bool ImageLoader::load_image(ImageId key_name, std::string relative_path) {

    // Load file and decode image.
    std::vector<unsigned char> image;
    unsigned width, height;
    std::string path = get_absolute_path(Settings::Resources::ASSETS_PATH + relative_path);
    unsigned error = lodepng::decode(image, width, height, path.c_str());

    // If there's an error, display it.
    if(error != 0) {
        std::cout << "error " << error << ": " << lodepng_error_text(error) << "for file: " << path << std::endl;
        return false;
    }

    // If image already exists, return false
    if(has_image(key_name)) {
        std::cout << "Image with key_name: " << key_name << " already exists" << std::endl;
        return false;
    }

    ImageData image_data{width, height};
    if(!store_in_gpu_memory(image, image_data)) {
        std::cout << "Error storing image in GPU memory" << std::endl;
        return false;
    }

    image.clear();
    ImageLoader::images[key_name] = image_data;
    std::cout << "Loaded image: '" << key_name << "' with width: " << width << " and height: " << height << ", from: " << path << std::endl;
    return true;
}









// GLuint loadBMP_custom(const char * imagepath){

// 	printf("Reading image %s\n", imagepath);

// 	// Data read from the header of the BMP file
// 	unsigned char header[54];
// 	unsigned int dataPos;
// 	unsigned int imageSize;
// 	unsigned int width, height;
// 	// Actual RGB data
// 	unsigned char * data;

// 	// Open the file
// 	FILE * file = fopen(imagepath,"rb");
// 	if (!file)							    {printf("%s could not be opened. Are you in the right directory ? Don't forget to read the FAQ !\n", imagepath); getchar(); return 0;}

// 	// Read the header, i.e. the 54 first bytes

// 	// If less than 54 bytes are read, problem
// 	if ( fread(header, 1, 54, file)!=54 ){ 
// 		printf("Not a correct BMP file\n");
// 		return 0;
// 	}
// 	// A BMP files always begins with "BM"
// 	if ( header[0]!='B' || header[1]!='M' ){
// 		printf("Not a correct BMP file\n");
// 		return 0;
// 	}
// 	// Make sure this is a 24bpp file
// 	if ( *(int*)&(header[0x1E])!=0  )         {printf("Not a correct BMP file\n");    return 0;}
// 	if ( *(int*)&(header[0x1C])!=24 )         {printf("Not a correct BMP file\n");    return 0;}

// 	// Read the information about the image
// 	dataPos    = *(int*)&(header[0x0A]);
// 	imageSize  = *(int*)&(header[0x22]);
// 	width      = *(int*)&(header[0x12]);
// 	height     = *(int*)&(header[0x16]);

// 	// Some BMP files are misformatted, guess missing information
// 	if (imageSize==0)    imageSize=width*height*3; // 3 : one byte for each Red, Green and Blue component
// 	if (dataPos==0)      dataPos=54; // The BMP header is done that way

// 	// Create a buffer
// 	data = new unsigned char [imageSize];

// 	// Read the actual data from the file into the buffer
// 	fread(data,1,imageSize,file);

// 	// Everything is in memory now, the file wan be closed
// 	fclose (file);

// 	// Create one OpenGL texture
// 	GLuint textureID;
// 	glGenTextures(1, &textureID);
	
// 	// "Bind" the newly created texture : all future texture functions will modify this texture
// 	glBindTexture(GL_TEXTURE_2D, textureID);

// 	// Give the image to OpenGL
// 	glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);

// 	// OpenGL has now copied the data. Free our own version
// 	delete [] data;

// 	// Poor filtering, or ...
// 	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
// 	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); 

// 	// ... nice trilinear filtering.
// 	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
// 	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
// 	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
// 	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); 
// 	glGenerateMipmap(GL_TEXTURE_2D);

// 	// Return the ID of the texture we just created
// 	return textureID;
// }