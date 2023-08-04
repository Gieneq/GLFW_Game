#include "Loader.h"

#include <iostream>
#include "Settings.h"
#include "pugixml.hpp"
#include "lodepng.h"
#include "IO.h"
#include "TransformComponent.h"
#include "SpriteComponent.h"
#include "ControllableComponent.h"

bool Loader::loadAssets() {
    std::cout << "Loading assets data..." << std::endl;
    
    auto texture_id = Loader::getLoader().loadTextureFromAssets("tiles.png");
    // auto texture_data = Loader::getLoader().getTextureDataByID(texture_id);
    std::string texture_name{"some_tiles"};
    if(Loader::getLoader().registerTextureStringName(texture_name, texture_id)) {
        std::cout << "Registered texture: " << texture_id << "as " << texture_name << std::endl;
    }
    else {
        std::cout << "Failed to register texture name: " << texture_id << std::endl;
    }

    std::cout << "Loader done!" << std::endl;
    return true;
}

bool Loader::loadWorld(World& world) {
    std::cout << "Loading world data..." << std::endl;
    
    try
    {
        Loader::getLoader().__load_map(world, "testmap");
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    


    int size = 10;

    for(int y = -size; y < size+1; y++) {
        for(int x = -size; x < size+1; x++) {
            Entity *junk = new Entity();
            auto color_comp = new ColorComponent(junk);
            color_comp->rect.top_left.x = static_cast<float>(x);
            color_comp->rect.top_left.y = static_cast<float>(y);
            color_comp->r = 0.5F + (y % 2) / 2.0F;
            color_comp->g = 0.5F + (x % 2) / 2.0F;
            junk->addComponent(color_comp);
            world.entities.push_back(junk);
        }
    }


    return true;
}

bool Loader::loadPlayer(World& world) {
    std::cout << "Loading player data..." << std::endl;
    Entity* player = &world.player;

   /**
     * Create players look.
     * Load texture or replace with placeholde color.
     */
    auto playersTextureID = Loader::getLoader().getTextureIDByName("some_tiles");

    if(playersTextureID) {
        std::cout << "has image" << std::endl;
        auto texture = new TextureComponent(player, *playersTextureID);
        // std::cout << "texture: " << texture->image_id.id << ": " << Loader::get_loader().get_image(texture->image_id).texture_id << std::endl;
        player->addComponent(texture);
        auto trsf_ctrl = new TransformComponent(player, texture->rect.top_left);
        player->addComponent(trsf_ctrl);
    } else {
        std::cout << "no image" << std::endl;
        auto color = new ColorComponent(player);
        color->r = 1.0F;
        color->g = 0.0F;
        color->b = 1.0F;
        color->rect.top_left.x = 0.5F;
        player->addComponent(color);

        auto trsf_ctrl = new TransformComponent(player, color->rect.top_left);
        player->addComponent(trsf_ctrl);
    }

    // todo add some templating or whatever - it is not obvious
    // that player sam some component to be controlled
    auto wsad_ctrl = new WSADControllableComponent(player);
    player->addComponent(wsad_ctrl);

    world.entities.push_back(player);

    return true;
}

std::optional<TextureData> Loader::getTextureDataByID(TextureId key_texture_id) {
    if (hasTextureDataWithID(key_texture_id)) {
        return textureDatas[key_texture_id];
    }
    return std::nullopt;
}

std::optional<TextureId> Loader::getTextureIDByName(const std::string& name) {
    if (hasTextureIDWithName(name)) {
        return texturesIDsRegister[name];
    }
    return std::nullopt;
}

TextureData Loader::storeInGPUMemory(std::vector<unsigned char>& pixels, int width, int height, std::string abs_path) {
    GLuint tid;
    glGenTextures(1, &tid);
    glBindTexture(GL_TEXTURE_2D, tid);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels.data());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);
    
    if(tid == 0) {
        std::cout << "Error creating texture in GPU" << std::endl;
        return TextureData::corrupted();
    }
 
    TextureData texture_data{abs_path, width, height, tid};
    std::cout << "Texture created successfully with id: " << texture_data.id << "." << std::endl;
    return texture_data;
}

TextureId Loader::loadTextureFromAssets(std::string res_path) {
    std::string absolute_path = IO::get_absolute_path(Settings::Resources::ASSETS_PATH + res_path);
    return load_image(absolute_path);
}

bool Loader::registerTextureStringName(const std::string& name, const TextureId& id) {
    if (texturesIDsRegister.find(name) != texturesIDsRegister.end()) {
        return false;
    }
    texturesIDsRegister[name] = id;
    return true;    
}


// bool Loader::load_image(ImageId key_name, std::string relative_path) {
TextureId Loader::load_image(std::string abs_path) {

    // Load file and decode image.
    std::vector<unsigned char> image;
    unsigned width, height;
    std::string absolute_path = IO::get_absolute_path(abs_path);
    // std::string absolute_path = IO::get_absolute_path(Settings::Resources::ASSETS_PATH + relative_path);
    unsigned error = lodepng::decode(image, width, height, absolute_path.c_str());

    // If there's an error, display it.
    if(error != 0) {
        std::cout << "error " << error << ": " << lodepng_error_text(error) << "for file: " << absolute_path << std::endl;
        return false;
    }

    // // If image already exists, return false
    // if(has_image(key_name)) {
    //     std::cout << "Image with key_name: " << key_name << " already exists" << std::endl;
    //     return false;
    // }

    // GLuint texture_id{0};
    auto texture_data = storeInGPUMemory(image, width, height, absolute_path);
    if(!texture_data.id.hasID()) {
        std::cout << "Error storing image in GPU memory" << std::endl;
        return false;
    }
    
    // ImageData image_data{};
    // image_data.texture_id = texture_id;
    // image_data.absolute_path = absolute_path;
    // image_data.relative_path = relative_path;
    // image_data.width = width;
    // image_data.height = height;

    image.clear();
    Loader::textureDatas[texture_data.id] = texture_data;
    std::cout << "Loaded image: '" << texture_data.id << "' with width: " << texture_data.width << " and height: " << texture_data.height << ", from: " << texture_data.absolute_path << std::endl;
    return true;
}


// <tileset version="1.9" tiledversion="1.9.2" name="details" tilewidth="20" tileheight="20" tilecount="225" columns="15">
//  <image source="details.png" width="300" height="300"/>
//  <tile id="0">
//   <objectgroup draworder="index" id="2">
//    <object id="1" x="0" y="0" width="20" height="4"/>
//    <object id="2" x="0" y="0" width="4" height="20"/>
//   </objectgroup>
//  </tile>
// Tileset Loader::load_tileset(const Range_i& gids_range, const std::string& absolute_path_tls) {
//     std::cout << "  processing: " << absolute_path_tls << std::endl;
//     Tileset tileset;
//     tileset.ids_range = gids_range;
//     std::string dir_path = IO::get_containing_dir(absolute_path_tls);

//     pugi::xml_document doc;
//     pugi::xml_parse_result result_tls = doc.load_file(absolute_path_tls.c_str());
//     if (!result_tls) {
//         throw std::runtime_error("Error loading tileset data: " + absolute_path_tls);
//     }

//     //validate tilesize? seems not cuz tiles can have different size than map grid
//     //todo same this node and reuse
//     auto tileset_node = doc.child("tileset");
//     if(!tileset_node.hash_value()) {
//         throw std::runtime_error("Error parsing tileset node!");
//     }

//     std::string tileset_name = tileset_node.attribute("name").value();
//     int tileset_tilewidth = tileset_node.attribute("tilewidth").as_int();
//     int tileset_tileheight = tileset_node.attribute("tileheight").as_int();
//     int tileset_tilecount = tileset_node.attribute("tilecount").as_int();
//     int tileset_columns = tileset_node.attribute("columns").as_int();
    
//     auto image_node = tileset_node.child("image");
//     if(!image_node.hash_value()) {
//         throw std::runtime_error("Error parsing image node!");
//     }

//     std::string img_path = image_node.attribute("source").value();
//     int img_width = image_node.attribute("width").as_int();
//     int img_height = image_node.attribute("height").as_int();

//     // std::string img_path = /

//     return tileset;
// }


void Loader::__load_map(World& world, std::string map_name) {
    // std::string maps_dir{"res/data/maps/"};
    // auto path = IO::get_absolute_path(maps_dir + map_name + ".tmx");
    // pugi::xml_document doc;
    // pugi::xml_parse_result result = doc.load_file(path.c_str());
    // if (!result) {
    //     throw std::runtime_error("Error loading map: " + map_name);
    // }

    // MapData map_data{};
    // auto map_node = doc.child("map");
    // if(!map_node.hash_value()) {
    //     std::cout << "Fileformat error" << std::endl;
    // }
    // map_data.x = Settings::Map::ORIGIN_X;
    // map_data.y = Settings::Map::ORIGIN_Y;

    // map_data.width = map_node.attribute("width").as_int(Settings::Map::WIDTH);
    // map_data.height = map_node.attribute("height").as_int(Settings::Map::HEIGHT);

    // int tile_width = map_node.attribute("tilewidth").as_int(Settings::Map::TILE_SIZE);
    // int tile_height = map_node.attribute("tileheight").as_int(Settings::Map::TILE_SIZE);

    // if(tile_width != tile_height) {
    //     throw std::runtime_error("Tile width and height must be equal");
    // }
    // map_data.tile_size = tile_width;

    // std::cout << "Map width: " << map_data.width << ", height: " << map_data.height << ", tile size: " << map_data.tile_size << std::endl;

    // for(auto tileset_node = map_node.child("tileset"); tileset_node; tileset_node = tileset_node.next_sibling("tileset")) {
    //     //todo create some tileset data
    //     int tileset_first_id = tileset_node.attribute("firstgid").as_int();

    //     auto next_node = tileset_node.next_sibling("tileset");
    //     int tileset_last_id = next_node.hash_value() ? next_node.attribute("firstgid").as_int() - 1 : Settings::Map::MAX_GID;


    //     std::string relative_path = tileset_node.attribute("source").value();
    //     std::string absolute_path = IO::get_absolute_path(maps_dir + relative_path);

    //     std::cout << "[" << tileset_first_id  << "..." << tileset_last_id<< "],  tileset name: " << relative_path << "abs_path: " << absolute_path << std::endl;
    //     // std::cout << "  dir: " << IO::get_containing_dir(absolute_path) << std::endl;
    
    //     Range_i tileset_range{tileset_first_id, tileset_last_id};
    //     auto new_tileset = load_tileset(tileset_range, absolute_path);

    // }
}
    




        // std::string tileset_image_path = tileset_node.child("image").attribute("source").value();
        // std::string tileset_image_name = tileset_image_path.substr(0, tileset_image_path.find_last_of("."));
        // std::string tileset_image_extension = tileset_image_path.substr(tileset_image_path.find_last_of(".") + 1);
        // std::string tileset_image_relative_path = relative_path_to_resources + tileset_image_path;
        // std::string tileset_image_absolute_path = get_absolute_path(tileset_image_relative_path);
        // std::string tileset_image_key_name = tileset_image_name + "." + tileset_image_extension;

        // if(!has_image(tileset_image_key_name)) {
        //     load_image(tileset_image_key_name, tileset_image_relative_path);
        // }

        // ImageData& tileset_image_data = get_image(tileset_image_key_name);
        // int tileset_image_width = tileset_image_data.width;
        // int tileset_image_height = tileset_image_data.height;

        // int tileset_first_gid = tileset_node.attribute("firstgid").as_int();
        // int tileset_tile_width = tileset_node.attribute("tilewidth").as_int();
        // int tileset_tile_height = tileset_node.attribute("tileheight").as_int();
        // int tileset_tile_count = tileset_node.attribute("tilecount").as_int();
        // int tileset_columns = tileset_node.attribute("columns").as_int();

        // int tileset_rows = tileset_tile_count / tileset_columns;
        // int tileset_last_gid = tileset_first_gid + tileset_tile_count - 1;

        // for(int i = tileset_first_gid; i <= tileset_last_gid; i++) {
        //     int tileset_tile_x = (i - tileset_first_gid) % tileset_columns;
        //     int tileset_tile_y = (i - tileset_first_gid) / tileset_columns;

        //     float tileset_tile_u = (float)tileset_tile_x / (float)tileset_columns;
        //     float tileset_tile_v = (float)tileset_tile_y / (float)tileset_rows;
        //     float tileset_tile_u2 = (float)(tileset_tile_x + 1) / (float)tileset_columns;
        //     float tileset_tile_v2 = (float)(tileset_tile_y +


    

    // doc.child("map").attribute("width").as_int();



    // auto path = get_absolute_path("res/Exchange.xml");
    // pugi::xml_document doc;
    // pugi::xml_parse_result result = doc.load_file(path.c_str());
    // if (!result)
    //     return ;
    // //////////////////////////////////////////
        
    // // for (pugi::xml_node tool: doc.child("Profile").child("Tools").children("Tool"))
    // // {
    // //     int timeout = tool.attribute("Timeout").as_int();
        
    // //     if (timeout > 0)
    // //         std::cout << "Tool " << tool.attribute("Filename").value() << " has timeout " << timeout << "\n";
    // // }




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