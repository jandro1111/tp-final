#include "Gui.h"

using namespace std;

Gui::Gui() {
    closeWindow = false;
    display = NULL;
    buffer = NULL;
    state = OPEN_FILE;
    doAction = NOTHING;
    selectedFile = 0;
    selectedBlock = 0;
    merkleRoot = "";
    merkleTree = NULL;
}

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

    al_set_window_title(this->display, "Display Selector");

    configEvents();

    configImGui();

    return true;
}

void Gui::configImGui(void)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplAllegro5_Init(display);
    ImGui::StyleColorsDark();
}

void Gui::mainWindow()
{

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

    static bool windowActive = true;


    switch (this->state) {
    case MAIN_WINDOW:
        ImGui::SetNextWindowSize(ImVec2(DISPLAY_SIZE_X, DISPLAY_SIZE_Y), ImGuiCond_Always);
        ImGui::Begin("Main", &windowActive, ImGuiWindowFlags_MenuBar);
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
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }

        if (selectedPath.string() != "") {

            //blockchain myBlockchain(selectedPath.string());
            //nlohmann::json block;

            ImGui::TextColored(ImVec4(1, 1, 0, 1), "Selected file: "); ImGui::SameLine();
            ImGui::Text(selectedPath.string().c_str());

            ImGui::Separator();

            if (blockInfo.size() != 0) {

                ImGui::TextColored(ImVec4(1, 1, 0, 1), "Merkle root"); ImGui::SameLine();
                if (ImGui::Button("Calculate")) {
                    doAction = CALC_MERKLE;
                }
                ImGui::SameLine();
                if (ImGui::Button("Validate")) {
                    doAction = VALIDATE_MERKLE;
                }
                ImGui::SameLine();
                if (ImGui::Button("Merkle Tree")) {
                    doAction = SHOW_TREE;
                }
                if (doAction == CALC_MERKLE || doAction == VALIDATE_MERKLE || doAction == SHOW_TREE) {
                    ImGui::SameLine();
                    if (ImGui::Button("Close")) {
                        doAction = NOTHING;
                        merkleRoot = "";
                    }
                    if (doAction == CALC_MERKLE && merkleRoot != "") {
                        ImGui::TextColored(ImVec4(1, 1, 0, 1), ("Block " + std::to_string(selectedBlock) + " merkle root: ").c_str()); ImGui::SameLine();
                        ImGui::Text(merkleRoot.c_str());
                    }
                    if (doAction == VALIDATE_MERKLE && merkleRoot != "") {
                        //ImGui::TextColored(ImVec4(1, 1, 0, 1), ("Block " + std::to_string(selectedBlock) + " merkle root: ").c_str()); ImGui::SameLine();
                        //ImGui::Text(merkleRoot.c_str());
                    }
                }
                ImGui::Separator();
                //ImGui::Spacing();

                ImGui::BeginChild("Scrolling");

                //Bloques
                for (int i = 0; i != blockInfo.size(); i++) {
                    //if (ImGui::TreeNode(("Block " + std::to_string(i)).c_str())) {
                    //    ImGui::Indent(); 
                    //    ImGui::Text(("Block ID: " + blockInfo[i].blockID).c_str());
                    //    ImGui::Text(("Previous block ID: " + blockInfo[i].previousBlockID).c_str());
                    //    ImGui::Text(("#Transactions: " + std::to_string(blockInfo[i].cantTransactions)).c_str());
                    //    ImGui::Text(("Block number: " + std::to_string(blockInfo[i].blockNumber)).c_str());
                    //    ImGui::Text(("Nonce: " + std::to_string(blockInfo[i].nonce)).c_str());
                    //    ImGui::Unindent(); 
                    //    ImGui::TreePop();
                    //}

                    ImGui::RadioButton(("Block " + std::to_string(i)).c_str(), &selectedBlock, i);
                    if (selectedBlock == i) {
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
            else {
                ImGui::Text("No blocks in selected file");
            }
        }
        ImGui::End();
        break;

    case OPEN_FILE:

        ImGui::SetNextWindowSize(ImVec2(DISPLAY_SIZE_X, DISPLAY_SIZE_Y), ImGuiCond_Always);
        ImGui::Begin("File", &windowActive, ImGuiWindowFlags_MenuBar);
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

        if (doAction == SEARCH_FILES) {

            //Seleccion del archivo JSON
            if (ImGui::Button("Select"))
            {
                //Se eligio archivo
                if (selectedFile >= (int) dirs.size()) {
                    currentPath = files[selectedFile - dirs.size()];
                    selectedPath = files[selectedFile - dirs.size()];
                    state = MAIN_WINDOW;
                    doAction = NOTIFY_NEW_PATH;
                    selectedBlock = 0;
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

            //for (std::filesystem::recursive_directory_iterator it(parentDir);
            //    it != std::filesystem::recursive_directory_iterator();
            //    ++it) {
            //    index++;
            //    if (std::filesystem::is_directory(it->path())) {
            //        for (int i = 0; i < it.depth(); ++i) {
            //            ImGui::Indent();
            //        }
            //        ImGui::BulletText(it->path().filename().string().c_str());
            //        for (int i = 0; i < it.depth(); ++i) {
            //            ImGui::Unindent();
            //        }
            //    }
            //    else {
            //        files.push_back(*it);
            //        for (int i = 0; i < it.depth(); ++i) {
            //            ImGui::Indent();
            //        }
            //        ImGui::RadioButton(it->path().filename().string().c_str(), &selectedPath, index);
            //        for (int i = 0; i < it.depth(); ++i) {
            //            ImGui::Unindent();
            //        }
            //    }
            //}

            //for (std::filesystem::recursive_directory_iterator it(strPath);
            //    it != std::filesystem::recursive_directory_iterator();
            //    ++it) {
            //    if (std::filesystem::is_directory(it->path())) {
            //        for (int i = 0; i < it.depth(); ++i) {
            //            ImGui::Indent();
            //        }
            //        if (ImGui::TreeNode(it->path().filename().string().c_str())) {
            //        }
            //        for (int i = 0; i < it.depth(); ++i) {
            //            ImGui::Unindent();
            //        }
            //    }
            //    else {
            //        for (int i = 0; i < it.depth(); ++i) {
            //            ImGui::Indent();
            //        }
            //        ImGui::Text(it->path().filename().string().c_str());
            //        for (int i = 0; i < it.depth(); ++i) {
            //            ImGui::Unindent();
            //        }
            //    }
            //}
            ImGui::EndChild();
        }
        ImGui::End();
        break;
    }  
}

std::string Gui::getSelectedFile(void) {
    blockInfo.clear();  //Limpia vector con info de los bloques del json
    return selectedPath.string();
}

void Gui::setBlockInfo(std::string blockID, std::string previousBlockID, int cantTransactions, int blockNumber, int nonce) {
    BlockInfo block = {blockID, previousBlockID, cantTransactions, blockNumber, nonce};
    blockInfo.push_back(block);
}

bool Gui::isNewPath() {
    return (doAction == NOTIFY_NEW_PATH);
}

int Gui::getSelectedBlock() {
    if (doAction == CALC_MERKLE && merkleRoot == "") {
        return selectedBlock;
    }
    return NO_SELECTION;
}

void Gui::setMerkleRoot(std::string merkleRoot) {
    this->merkleRoot = merkleRoot;
}

void Gui::setMerkleTree(std::string * merkle) {
    merkleTree = merkle;
}

bool Gui::configEvents()
{

    return true;
}

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
    return true;
}


void Gui::destroyAllegro(void)
{
    al_shutdown_ttf_addon();
    al_shutdown_font_addon();
    al_shutdown_primitives_addon();
    al_shutdown_image_addon();
}

void Gui::update(void)
{

    //al_set_target_bitmap(buffer);

    //do stuff

    //al_set_target_backbuffer(display);
    //al_draw_scaled_bitmap(buffer, 0, 0, DISPLAY_SIZE_X, DISPLAY_SIZE_Y, 0, 0, DISPLAY_SIZE_X, DISPLAY_SIZE_Y, 0);
    al_flip_display();
}