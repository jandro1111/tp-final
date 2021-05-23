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
#define     FPS                 60
#define     NO_SELECTION        -1

enum states { MAIN_WINDOW, OPEN_FILE, SELECT_FILE};


class Gui {
public:
    
    Gui();

    void mainWindow();

    bool initGUI();

    bool configEvents();

    void configImGui();

    void destroyAllegro();

    void update();

    ALLEGRO_DISPLAY* display;

private:

    bool initAllegro();

    ALLEGRO_BITMAP* buffer;

    bool closeWindow;

    int state;
    
    char pathName[50] = { 0 };
    
    std::vector<std::filesystem::path> files;
    std::vector<std::filesystem::path> dirs;
    std::filesystem::path currentPath;
    int selectedFile;

    Files path;
};


#endif