#include "Gui.h"

using namespace std;

//Constructor. Seteo de variables.
Gui::Gui() {
    display = NULL;
    buffer = NULL;
    font = NULL;
    state = OPEN_FILE;
    doAction = NOTHING;
    selectedFile = 0;
    selectedBlock = 0;
    currentBlock = NO_SELECTION;
    merkleRoot = "";
    merkleTree.clear();
    newMerkleRoot = false;
    newMerkleTree = false;
}

//Inicializacion de Allegro e ImGui.
bool Gui::initGUI()
{
    if (!initAllegro()) {
        return false;
    }

    al_set_new_display_flags(ALLEGRO_RESIZABLE);
    this->display = al_create_display(DISPLAY_SIZE_X, DISPLAY_SIZE_Y);
    if (!this->display) {
        fprintf(stderr, "Failed to create display!\n");
        return false;
    }

    al_set_window_title(this->display, "Blockchain");

    configImGui();

    return true;
}

//Configuracion de ImGui
void Gui::configImGui(void)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplAllegro5_Init(display);
    ImGui::StyleColorsDark();
}

//Metodo pricipal de la GUI. Control y graficacion de los estados y sus acciones.
void Gui::GUIwindow()
{
    //Flags de ImGui
    static bool NoTitlebar = false;
    static bool NoMenu = true;
    static bool NoCollapse = false;
    static bool NoResize = false;
    static bool NoMove = false;
    static bool NoClose = true;
    static bool NoBackground = false;
    static bool NoScrollbar = true;
    static bool no_bring_to_front = false;


    ImGuiWindowFlags window_flags = 0;
    if (NoTitlebar)        window_flags |= ImGuiWindowFlags_NoTitleBar;
    if (NoScrollbar)       window_flags |= ImGuiWindowFlags_NoScrollbar;
    if (!NoMenu)           window_flags |= ImGuiWindowFlags_MenuBar;
    if (NoMove)            window_flags |= ImGuiWindowFlags_NoMove;
    if (NoResize)          window_flags |= ImGuiWindowFlags_NoResize;
    if (NoCollapse)        window_flags |= ImGuiWindowFlags_NoCollapse;
    if (NoBackground)      window_flags |= ImGuiWindowFlags_NoBackground;
    if (no_bring_to_front)  window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;

    ImGuiSliderFlags reg_slider = ImGuiSliderFlags_AlwaysClamp;
    ImGuiSliderFlags log_slider = reg_slider | ImGuiSliderFlags_Logarithmic;
    const ImGuiViewport* main_viewport = ImGui::GetMainViewport();


    //Variable de control de ventana de ImGui
    static bool windowActive = true;

    //Widgets y acciones segun estado. 
    switch (this->state) {

        //MAIN_WINDOW: widgets y acciones sobre los bloques cargados del archivo seleccionado en FILES.
    case MAIN_WINDOW:
        ImGui::SetNextWindowSize(ImVec2(DISPLAY_SIZE_X, DISPLAY_SIZE_Y), ImGuiCond_Always);
        ImGui::Begin("Main", &windowActive, ImGuiWindowFlags_MenuBar);
        //Menu Bar
        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("Main"))
            {
                state = MAIN_WINDOW;
                doAction = NOTHING;
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("File"))
            {
                state = OPEN_FILE;
                doAction = NOTHING;
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }

        //En caso de haber un archivo seleccionado, se disponen los bloques.
        if (selectedPath.string() != "") {

            ImGui::TextColored(ImVec4(1, 1, 0, 1), "Selected file: "); ImGui::SameLine();
            ImGui::Text(selectedPath.string().c_str());

            ImGui::Separator();

            if (blockInfo.size() != 0) {

                if (ImGui::Button("Merkle Root")) {
                    doAction = CALC_MERKLE;
                }
                ImGui::SameLine();
                if (ImGui::Button("Merkle Tree")) {
                    doAction = SHOW_TREE;
                }
                if (doAction == CALC_MERKLE || doAction == SHOW_TREE) {
                    ImGui::SameLine();
                    if (ImGui::Button("Close")) {
                        doAction = NOTHING;
                        merkleRoot = "";
                        merkleTree.clear();
                        newMerkleRoot = true;
                    }
                    if (doAction == CALC_MERKLE && merkleRoot != "") {
                        ImGui::TextColored(ImVec4(1, 1, 0, 1), ("Block " + std::to_string(selectedBlock) + ":").c_str());
                        ImGui::Indent();
                        ImGui::Text(("Calulated: " + merkleRoot).c_str());
                        ImGui::Text(("Block's: " + merkleRoot).c_str());
                    }
                }
                ImGui::Separator();

                if (doAction != SHOW_TREE) {
                    ImGui::BeginChild("Scrolling");

                    //Bloques
                    for (int i = 0; i != blockInfo.size(); i++) {
                        ImGui::RadioButton(("Block " + std::to_string(i)).c_str(), &selectedBlock, i);
                        if (selectedBlock == i) {
                            if (currentBlock != selectedBlock) {
                                currentBlock = selectedBlock;
                                newMerkleRoot = true;
                            }
                            ImGui::Indent(); ImGui::Indent();
                            ImGui::Text(("Block ID: " + blockInfo[i].blockID).c_str());
                            ImGui::Text(("Previous block ID: " + blockInfo[i].previousBlockID).c_str());
                            ImGui::Text(("#Transactions: " + std::to_string(blockInfo[i].cantTransactions)).c_str());
                            ImGui::Text(("Block number: " + std::to_string(blockInfo[i].blockNumber)).c_str());
                            ImGui::Text(("Nonce: " + std::to_string(blockInfo[i].nonce)).c_str());
                            ImGui::Unindent(); ImGui::Unindent();
                        }
                    }
                    ImGui::EndChild();
                }
            }
            else {
                ImGui::Text("No blocks in selected file");
            }
        }
        ImGui::End();
        break;

        //OPEN_FILE: navegacion y seleccion sobre el path introducido por el usuario.
    case OPEN_FILE:

        ImGui::SetNextWindowSize(ImVec2(DISPLAY_SIZE_X, DISPLAY_SIZE_Y), ImGuiCond_Always);
        ImGui::Begin("File", &windowActive, ImGuiWindowFlags_MenuBar);

        //Mismas Menu Bar
        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("Main"))
            {
                state = MAIN_WINDOW;
                doAction = NOTHING;
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("File"))
            {
                state = OPEN_FILE;
                doAction = NOTHING;
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }

        ImGui::InputText("Path", pathName, 50);
        ImGui::SameLine();
        if (ImGui::Button("Open"))
        {
            selectedPath.clear();
            currentPath.clear();
            std::string currentPathStr = "";
            currentPathStr += std::string(pathName);
            currentPath = currentPathStr;
            path.setPath(currentPathStr);
            if (!path.openDirectory()) {
                std::cout << "Failed when opening the directory!" << std::endl;
            }
            else {
                doAction = SEARCH_FILES;
                files.clear();
                files = path.getFiles();
                dirs.clear();
                dirs = path.getDirs();
                selectedFile = 0;
            }
        }
        ImGui::SameLine();
        if (ImGui::Button("Close")) {
            state = MAIN_WINDOW;
            doAction = NOTHING;
        }
        ImGui::Separator();

        //Navegacion por el directorio del path introducido
        if (doAction == SEARCH_FILES) {

            //Seleccion del archivo JSON
            if (ImGui::Button("Select"))
            {
                //Se eligio archivo
                if (selectedFile >= (int)dirs.size()) {
                    currentPath = files[selectedFile - dirs.size()];
                    selectedPath = files[selectedFile - dirs.size()];
                    state = MAIN_WINDOW;
                    doAction = NOTIFY_NEW_PATH;
                    selectedBlock = 0;
                    currentBlock = NO_SELECTION;
                }
                //Se ingresa en directorio
                else {
                    currentPath = dirs[selectedFile];
                    path.setPath(currentPath.string());
                    if (!path.openDirectory()) {
                        std::cout << "Failed when opening the directory!" << std::endl;
                    }
                    else {
                        files.clear();
                        files = path.getFiles();
                        dirs.clear();
                        dirs = path.getDirs();
                        selectedFile = 0;
                    }
                }
            }
            //En el caso de estar dentro de un directorio, se agrega boton para volver atras
            if (currentPath.string() != std::string(pathName)) {
                ImGui::SameLine();
                if (ImGui::Button("..")) {
                    currentPath = currentPath.parent_path();
                    path.setPath(currentPath.string());
                    if (!path.openDirectory()) {
                        std::cout << "Failed when opening the directory!" << std::endl;
                    }
                    else {
                        files.clear();
                        files = path.getFiles();
                        dirs.clear();
                        dirs = path.getDirs();
                        selectedFile = 0;
                    }
                }
            }

            ImGui::Separator();

            ImGui::BeginChild("Scrolling");

            //Chequea si existen directorios internos y grafica sus widgets.
            if (dirs.size() != 0) {
                ImGui::TextColored(ImVec4(1, 1, 0, 1), "Subdirectories");
                for (int i = 0; i != dirs.size(); i++) {
                    ImGui::Indent();
                    ImGui::RadioButton(dirs[i].relative_path().string().c_str(), &selectedFile, i);
                    ImGui::Unindent();
                }
            }
            //Chequea si existen archivos internos y grafica sus widgets.
            if (files.size() != 0) {
                ImGui::TextColored(ImVec4(1, 1, 0, 1), "Files");
                for (int i = 0; i != files.size(); i++) {
                    ImGui::Indent();
                    ImGui::RadioButton(files[i].filename().string().c_str(), &selectedFile, i + dirs.size());
                    ImGui::Unindent();
                }
            }
            ImGui::EndChild();
        }
        ImGui::End();
        break;
    }
}


//Graficacion de Merkle Tree
void Gui::drawMerkleTree(void) {
    //Grafica sobre el buffer
    al_set_target_bitmap(buffer);
    al_clear_to_color(BLACK);

    //Variables para calculo de dimensiones del buffer segun altura del arbol.
    static int treeHeight = 0;
    static int treeNodes = 1;
    static int sizeY = (int)(BUFFER_SIZE_Y / (treeHeight + 1));;
    static int fontSize = 0;
    //Por optimizacion, se recalcula solamente al introducir un nuevo merkle tree.
    if (newMerkleTree) {
        while (treeNodes < (int)merkleTree.size()) {
            treeNodes *= 2;
            treeHeight++;
        }
        sizeY = (int)(BUFFER_SIZE_Y / (treeHeight + 2));
        fontSize = (int)(BUFFER_SIZE_Y / (treeHeight * 6));
        al_destroy_font(font);
        font = al_load_ttf_font(TREE_FONT, fontSize, 0);
        if (font == NULL) {
            fprintf(stderr, "failed to create font!\n");
            return;
        }
        newMerkleTree = false;
    }

    int node = merkleTree.size();
    int i = 1;
    //El primer for itera por los niveles del arbol.
    for (int height = 0; height != treeHeight; height++, i *= 2) {
        //El segundo for grafica los nodos en cada nivel.
        int sizeX = (int)(BUFFER_SIZE_X / (i + 1));
        for (int j = i; j > 0; j--) {
            if (node != 0) {
                al_draw_text(font, WHITE, sizeX * (j), sizeY * (height), ALLEGRO_ALIGN_CENTRE, merkleTree[node - 1].c_str());
                if (height == (treeHeight - 1)) {
                    al_draw_text(font, YELLOW, sizeX * (j), sizeY * (height + 1), ALLEGRO_ALIGN_CENTRE, ("T" + std::to_string(j - 1)).c_str());
                    al_draw_line(sizeX * (j), sizeY * (height + 1) - fontSize, sizeX * (j), sizeY * (height + 1) - fontSize * 2, WHITE, (float)(8.0 / treeHeight));
                }
                if (i > 1) {
                    //En el caso de ser par
                    if ((j % 2) == 0) {
                        al_draw_line(sizeX * (j), sizeY * (height)-fontSize, sizeX * (j - 1), sizeY * (height)-fontSize, WHITE, (float)(8.0 / treeHeight));
                        al_draw_line(sizeX * (j)-(int)(sizeX / 2), sizeY * (height)-fontSize, sizeX * (j)-(int)(sizeX / 2), sizeY * (height)-fontSize * 2, WHITE, (float)(10.0 / treeHeight));
                    }
                }
                node--;
            }
        }
    }

    al_set_target_backbuffer(display);
    al_draw_scaled_bitmap(buffer, 0, 0, BUFFER_SIZE_X, BUFFER_SIZE_Y, 5, DISPLAY_SIZE_Y - BUFFER_SIZE_Y, BUFFER_SIZE_X, DISPLAY_SIZE_Y, 0);
}

//Devuelve Block deseado dentro del archivo json. 
std::string Gui::getSelectedFile(void) {
    if (doAction != CALC_MERKLE)
        doAction = NOTHING;
    return selectedPath.string();
}

//Setter de informacion primordial del bloque
void Gui::setBlockInfo(std::string blockID, std::string previousBlockID, int cantTransactions, int blockNumber, int nonce) {
    BlockInfo block = { blockID, previousBlockID, cantTransactions, blockNumber, nonce };
    blockInfo.push_back(block);
}

//Notificacion de nuevo path introducido por el usuario.
bool Gui::isNewPath() {
    if (doAction == NOTIFY_NEW_PATH) {
        blockInfo.clear();  //Limpia vector con info de los bloques del json
        return true;
    }
    return false;
}

//Notificacion de necesidad de actualizar Merkle Tree. 
bool Gui::isMerkleTree() {
    return (doAction == SHOW_TREE);
}

//Devuelve Block deseado dentro del archivo json.
int Gui::getSelectedBlock() {
    if (newMerkleRoot) {
        newMerkleRoot = false;
        return selectedBlock;
    }
    return NO_SELECTION;
}

//Setter del Merkle Root.
void Gui::setMerkleRoot(std::string merkleRoot) {
    this->merkleRoot = merkleRoot;
}

//Setter del Merkle Tree.
void Gui::setMerkleTree(std::vector <std::string> merkle) {
    merkleTree.clear();
    merkleTree = merkle;
    newMerkleTree = true;
}

//Inicializacion de addons y variables de Allegro.
bool Gui::initAllegro(void)
{
    if (!al_init()) {
        fprintf(stderr, "Failed to initialize allegro!\n");
        return false;
    }

    // Dear ImGui necesita Allegro para poder graficar. Para esto, usa el add-on de primitivas.
    if (!al_init_primitives_addon()) {
        fprintf(stderr, "Failed to initialize primitives addon!\n");
        return false;
    }

    if (!al_init_image_addon())
    {
        fprintf(stderr, "Failed to initialize image addon!\n");
        return false;
    }

    if (!al_install_keyboard()) {
        fprintf(stderr, "Failed to initialize keyboard!\n");
        return false;
    }

    if (!al_install_mouse()) {
        fprintf(stderr, "Failed to initialize mouse!\n");
        return false;
    }

    if (!al_init_font_addon())
    {
        fprintf(stderr, "Failed to initialize font addon!\n");
        return false;
    }
    if (!al_init_ttf_addon())
    {
        fprintf(stderr, "Failed to initialize ttf addon!\n");
        return false;
    }
    buffer = al_create_bitmap(BUFFER_SIZE_X, BUFFER_SIZE_Y);
    if (buffer == NULL)
    {
        fprintf(stderr, "Failed to initialize bitmap buffer!\n");
        return false;
    }
    return true;
}

//Llamado a los metodos de destroy y shutdown de Allegro. 
void Gui::destroyAllegro(void)
{
    al_destroy_font(font);
    al_destroy_bitmap(buffer);

    al_shutdown_font_addon();
    al_shutdown_primitives_addon();
    al_shutdown_image_addon();
    al_destroy_display(display);
}

void Gui::update(void)
{
    al_flip_display();
}