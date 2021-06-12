#ifndef GUI_H
#define GUI_H

#include <stdio.h>
#include <string>

#include "files.h"
#include "nodenet.h"

#include "./lib/imgui.h"
#include "./lib/imgui_impl_allegro5.h"
#include "./lib/imgui_internal.h"
#include <allegro5/allegro_color.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>



// --------------------------     ALLEGRO      -------------------------- //

#define     DISPLAY_SIZE_X      720
#define     DISPLAY_SIZE_Y      380
#define     BUFFER_SIZE_X       710
#define     BUFFER_SIZE_Y       280 

#define     FPS                 10

#define     TREE_FONT           "./Fonts/Roboto-Light.ttf"
#define     EDACOIN             "edacoinLogo.png"

// --------------------------     colores      -------------------------- //
#define BLACK           al_map_rgb(14,14,14)
#define WHITE           al_map_rgb(255,255,255)
#define RED             al_map_rgb(255,0,0)
#define GREEN           al_map_rgb(0,155,0)
#define GREY            al_map_rgb(128, 128, 128)
#define BACK            al_map_rgb(150, 175, 0)
#define YELLOW          al_map_rgb(255, 255, 0)


// --------------------------     GUI      -------------------------- //

#define     NO_SELECTION        -1

enum states { MAIN_WINDOW, BLOCKS, OPEN_FILE, NODE, LOG};

enum actions { NOTHING, NOTIFY_NEW_PATH, SEARCH_FILES, CALC_MERKLE, SHOW_TREE, CREATE_NODE, CONNECT_NODES, SEND_MESSAGE};

//Estructura que guarda la informacion a mostrar al usuario del bloque deseado
typedef struct {
    std::string blockID;
    std::string previousBlockID;
    int cantTransactions;
    int blockNumber;
    int nonce;
} BlockInfo;

/*La clase Gui contiene los metodos que hacen a la GUI del usuario y la graficacion con allegro. */
class Gui {
public:
    //Constructor
    Gui();

    //Metodo pricipal de la GUI. Control y graficacion de los estados y sus acciones.
    void GUIwindow();

    //Inicializacion de ImGui y Allegro. 
    bool initGUI();

    //Llamado a los metodos de destroy y shutdown de Allegro. 
    void destroyAllegro();

    //Contiene al flip display de Allegro.
    void update();

    //Devuelve el path ya verificado al archivo seleccionado
    std::string getSelectedFile(void);

    //Devuelve Block deseado dentro del archivo json. 
    int getSelectedBlock(void);

    //Setter del Merkle Root.
    void setMerkleRoot(std::string merkleRootCalculated, std::string merkleRootBlock);

    //Setter del Merkle Tree.
    void setMerkleTree(std::vector<std::vector<std::string>>);

    //Setter de informacion primordial del bloque
    void setBlockInfo(std::string blockID, std::string previousBlockID, int cantTransactions, int blockNumber, int nonce);

    //Notificacion de nuevo path introducido por el usuario.
    bool isNewPath();

    //Notificacion de necesidad de actualizar Merkle Tree. 
    bool isMerkleTree();

    //Graficacion de Merkle Tree
    void drawMerkleTree();

    //Devuelve ventana actual de la GUI
    int getGuiState();

    //Graficacion de Imagen en Main Window
    void drawMainWindow();

    //Graficacion de conecciones entre nodos
    void drawNodesConnections();

    //Display de Allegro
    ALLEGRO_DISPLAY* display;

private:

    //Inicializacion de addons y variables de Allegro.
    bool initAllegro();

    //Configuracion de ImGui.
    void configImGui();

    //Variables de Allegro.
    ALLEGRO_BITMAP* buffer;
    ALLEGRO_BITMAP* bitmap;
    ALLEGRO_FONT* font;

    //Variable de control de acciones. 
    int doAction;

    //Variable de control de estados. 
    int state;

    //Flags de notificacion de nuevos merkle root y merkle tree.
    bool newMerkleTree;
    bool newMerkleRoot;

    //Merkle Root calculado del bloque seleccionado
    std::string merkleRootCalculated;

    //Merkle Root real del bloque seleccionado
    std::string merkleRootBlock;

    //Merkle Tree del bloque seleccionado
    std::vector<std::vector<std::string>> merkleTree;

    //Variable para guardar el path introducido por el usuario.
    char pathName[50] = { 0 };

    //Variable para introducir la IP de un nodo
    char nodeIPtext[50] = { 0 };

    //Variable para introducir el puerto de un nodo
    char nodePortText[50] = { 0 };

    //Variable para introducir el puerto de un nodo
    char selection1[50] = { 0 };

    //Variable para introducir el puerto de un nodo
    char selection2[50] = { 0 };

    //Archivos json del path seleccionado.
    std::vector<std::filesystem::path> files;

    //Path a los directorios del path seleccionado.
    std::vector<std::filesystem::path> dirs;

    //Variable para moverse por los directorios.
    std::filesystem::path currentPath;

    //Guarda el path seleccionado para que se pueda recuperar con el getter
    std::filesystem::path selectedPath;

    //Variable para seleccion de los files con RadiusButton de ImGui
    int selectedFile;

    //Variable para seleccion de los bloques con RadiusButton de ImGui
    int selectedBlock;

    //Variable de seleccion de primer nodo
    int firstNode;

    //Variable de seleccion de segundo nodo
    int secondNode;

    //Variable para controlar cambio de bloque seleccionado y optimizar el programa
    int currentBlock;

    //Instancia de la clase File para navegar un directorio
    Files path;

    //Guarda informacion de todos los bloques del archivo seleccionado
    std::vector<BlockInfo> blockInfo;

    //Variable para mostrar estados de conexion entre nodos
    int checkConnectionState;

    //CONECCIONES
    bool isNewConnection;

    //Mensajes de Log
    std::vector<std::string> logMessages;

    nodenet userNodes;
};


#endif