#ifndef GUI_H
#define GUI_H

#include <stdio.h>
#include <string>

#include "files.h"

#include "./lib/imgui.h"
#include "./lib/imgui_impl_allegro5.h"

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>


#define     DISPLAY_SIZE_X      680
#define     DISPLAY_SIZE_Y      340
#define     FPS                 15
#define     NO_SELECTION        -1

enum states { MAIN_WINDOW, OPEN_FILE};

enum actions {NOTHING, NOTIFY_NEW_PATH, SEARCH_FILES, CALC_MERKLE, VALIDATE_MERKLE, SHOW_TREE};

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

    void setMerkleTree(std::string *);

    void setBlockInfo(std::string blockID, std::string previousBlockID, int cantTransactions, int blockNumber, int nonce);

    bool isNewPath();

    ALLEGRO_DISPLAY* display;

private:

    bool initAllegro();

    ALLEGRO_BITMAP* buffer;

    bool closeWindow;

    int state;

    int doAction;

    std::string merkleRoot;
    std::string* merkleTree;
    
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