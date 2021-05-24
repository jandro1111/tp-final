#ifndef GUI_H
#define GUI_H

#include <stdio.h>
#include <string>

#include "files.h"

#include "./lib/imgui.h"
#include "./lib/imgui_impl_allegro5.h"
#include <allegro5/allegro_color.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>


#define     DISPLAY_SIZE_X      720
#define     DISPLAY_SIZE_Y      380
#define     BUFFER_SIZE_X       710
#define     BUFFER_SIZE_Y       280

#define     TREE_NODE_SIZE_X    

#define     FPS                 10

#define     TREE_FONT           "./Fonts/Minecraft.ttf"


#define     NO_SELECTION        -1


// --------------------------     colores      -------------------------- //
#define BLACK           al_map_rgb(14,14,14)
#define WHITE           al_map_rgb(255,255,255)
#define RED             al_map_rgb(255,0,0)
#define GREEN           al_map_rgb(0,155,0)
#define GREY            al_map_rgb(128, 128, 128)
#define BACK            al_map_rgb(150, 175, 0)


enum states { MAIN_WINDOW, OPEN_FILE };

enum actions { NOTHING, NOTIFY_NEW_PATH, SEARCH_FILES, CALC_MERKLE, SHOW_TREE };

typedef struct {
    std::string blockID;
    std::string previousBlockID;
    int cantTransactions;
    int blockNumber;
    int nonce;
} BlockInfo;


class Gui {
public:

    Gui();

    void mainWindow();

    bool initGUI();

    bool configEvents();

    void configImGui();

    void destroyAllegro();

    void update();

    std::string getSelectedFile(void);

    int getSelectedBlock(void);

    void setMerkleRoot(std::string merkleRoot);

    void setMerkleTree(std::vector <std::string>);



    void setBlockInfo(std::string blockID, std::string previousBlockID, int cantTransactions, int blockNumber, int nonce);

    bool isNewPath();

    bool isMerkleTree();

    ALLEGRO_DISPLAY* display;

    void drawMerkleTree();

    

private:

    bool initAllegro();

    int doAction;

    ALLEGRO_BITMAP* buffer;
    ALLEGRO_FONT* font;

    bool closeWindow;

    int state;

    bool newMerkleRoot;

    std::string merkleRoot;
    std::vector <std::string> merkleTree;

    char pathName[50] = { 0 };

    std::vector<std::filesystem::path> files;
    std::vector<std::filesystem::path> dirs;
    std::filesystem::path currentPath;
    std::filesystem::path selectedPath;
    int selectedFile;
    int selectedBlock;

    Files path;

    std::vector<BlockInfo> blockInfo;
};


#endif