#include "Gui.h"
#include <time.h>
#include <cmath>

#define PI 3.14159265359

using namespace std;

//Constructor. Seteo de variables.
Gui::Gui() {
    display = NULL;
    buffer = NULL;
    bitmap = NULL;
    font = NULL;
    state = MAIN_WINDOW;
    doAction = NOTHING;
    selectedFile = 0;
    selectedBlock = 0;
    currentBlock = NO_SELECTION;
    merkleRootCalculated = "";
    merkleRootBlock= "";
    merkleTree.clear();
    newMerkleRoot = false;
    newMerkleTree = false;
    selectedPath.clear();
    currentPath.clear();
    isNewNode = true;
    firstNode = NO_SELECTION;
    secondNode = NO_SELECTION;
    blockInfo.clear();
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

    //Variables para creacion de nodos
    static const char* nodeComboBox[] = { "", "SPV" , "FULL" };
    static const char* fullMessages[] = { "", "Block", "Transaction", "Merkleblock", "Get Blocks"};
    static const char* spvMessages[] = { "", "Transaction", "Filter", "Get block header" };
    static int nodeType = 0;
    static int messageType = 0;
    static const char* preview_text = "";
    static int cantNodes = 0;
    static bool connectionFinished = false;

    //Variable de control de ventana de ImGui
    static bool windowActive = true;

    //Widgets y acciones segun estado. 
    switch (this->state) {

    //MAIN_WINDOW
    case MAIN_WINDOW:
        ImGui::SetNextWindowSize(ImVec2(DISPLAY_SIZE_X, DISPLAY_SIZE_Y), ImGuiCond_Always);
        ImGui::Begin("Main", &windowActive, ImGuiWindowFlags_MenuBar);
        //Menu Bar
        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("Menu"))
            {
                state = MAIN_WINDOW;
                doAction = NOTHING;
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Nodes"))
            {
                state = NODE;
                doAction = NOTHING;
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("File"))
            {
                state = OPEN_FILE;
                doAction = NOTHING;
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Blocks"))
            {
                state = BLOCKS;
                doAction = NOTHING;
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }
        ImGui::Dummy(ImVec2(20.0f, 80.0f));
        if (ImGui::Button("Nodes")) {
            state = NODE;
        }
        ImGui::Dummy(ImVec2(20.0f, 15.0f));
        if (ImGui::Button("File")) {
            state = OPEN_FILE;
        }
        ImGui::Dummy(ImVec2(20.0f, 15.0f));
        if (ImGui::Button("Blocks")) {
            state = BLOCKS;
        }

        ImGui::End();
        break;

    //BLOCKS: widgets y acciones sobre los bloques cargados del archivo seleccionado en FILES.
    case BLOCKS:
        ImGui::SetNextWindowSize(ImVec2(DISPLAY_SIZE_X, DISPLAY_SIZE_Y), ImGuiCond_Always);
        ImGui::Begin("Blocks", &windowActive, ImGuiWindowFlags_MenuBar);

        //Menu Bar
        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("Menu"))
            {
                state = MAIN_WINDOW;
                doAction = NOTHING;
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Nodes"))
            {
                state = NODE;
                doAction = NOTHING;
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("File"))
            {
                state = OPEN_FILE;
                doAction = NOTHING;
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Blocks"))
            {
                state = BLOCKS;
                doAction = NOTHING;
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }

        //En caso de haber un archivo seleccionado, se disponen los bloques.
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
                        merkleRootCalculated = "";
                        merkleRootBlock = "";
                        merkleTree.clear();
                        newMerkleRoot = true;
                    }
                    if (doAction == CALC_MERKLE && merkleRootCalculated != "") {
                        ImGui::TextColored(ImVec4(1, 1, 0, 1), ("Block " + std::to_string(selectedBlock) + ":").c_str());
                        ImGui::Indent();
                        ImGui::Text(("Calulated: " + merkleRootCalculated).c_str());
                        ImGui::Text(("Block: " + merkleRootBlock).c_str());
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
        ImGui::End();
        break;

    case NODE:

        ImGui::SetNextWindowSize(ImVec2(DISPLAY_SIZE_X, DISPLAY_SIZE_Y), ImGuiCond_Always);
        ImGui::Begin("Node", &windowActive, ImGuiWindowFlags_MenuBar);
        //Menu Bar
        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("Menu"))
            {
                state = MAIN_WINDOW;
                doAction = NOTHING;
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Nodes"))
            {
                state = NODE;
                doAction = NOTHING;
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("File"))
            {
                state = OPEN_FILE;
                doAction = NOTHING;
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Blocks"))
            {
                state = BLOCKS;
                doAction = NOTHING;
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }

        if (ImGui::Button("Create Node")) {
            doAction = CREATE_NODE;
        }
        ImGui::SameLine();
        if (ImGui::Button("Connect Node")) {
            doAction = CONNECT_NODES;
        }
        ImGui::SameLine();
        if (ImGui::Button("Send Message")) {
            doAction = SEND_MESSAGE;
        }
        ImGui::SameLine();
        if (ImGui::Button("Show Connections")) {
            doAction = SHOW_CONNECTIONS;
        }
        ImGui::Separator();

        switch (this->doAction) {


        //Crear nodo: permite al usuario ingresar IP, puerto (debe ser par) y tipo de nodo.
        case CREATE_NODE:
            ImGui::TextColored(ImVec4(1, 1, 0, 1), "Create Node");
            ImGui::InputText("Node IP", nodeIPtext, 50);
            ImGui::InputText("Node Server Port", nodePortText, 50, ImGuiInputTextFlags_CharsDecimal);
            ImGui::Combo("Node Type", &nodeType, nodeComboBox, IM_ARRAYSIZE(nodeComboBox));

            ImGui::Dummy(ImVec2(0.0f, 20.0f));

            //Guarda información introducida por el usuario
            if (ImGui::Button("Create")) {
                int port = atoi(nodePortText);
                std::string ip = std::string(nodeIPtext);
                //En el caso de introducirse un puerto par y una IP no vacia, se crea el nodo.
                if (port % 2 == 0 && ip != "") {
                    this->userNodes.createnode(nodeType == 2, port, ip);
                    isNewNode = true;
                    //Se limpian los campos introducidos
                    memset(nodeIPtext, 0, sizeof(nodeIPtext));
                    memset(nodePortText, 0, sizeof(nodePortText));
                    nodeType = 0;
                }
            }
            ImGui::SameLine();
            if (ImGui::Button("Close")) {
                doAction = NOTHING;
                memset(nodeIPtext, 0, sizeof(nodeIPtext));
                memset(nodePortText, 0, sizeof(nodePortText));
            }
            break;

            //Conección de nodos: permite al usuario conectar un nodo con otros (segun el tipo) y generar así nodos vecinos.
        case CONNECT_NODES:
            ImGui::TextColored(ImVec4(1, 1, 0, 1), "Connect Nodes");
            //Generacion de texto antes de abrir la combobox
            cantNodes = this->userNodes.getcantnodos();
            preview_text = (userNodes.nodos.size() != 0) ? ((firstNode != NO_SELECTION) ? this->userNodes.nodos[firstNode].ip.c_str() : "") : "";
            //preview_text = (cantNodes != 0) ? ((firstNode != NO_SELECTION) ? (this->userNodes.getnodo(firstNode)).ip.c_str() : "") : "";

            //Primera combobox permite elegir el nodo desde el cual se va a conectar
            if (ImGui::BeginCombo("Selected Node", preview_text, 0))
            {
                for (int n = 0; n != cantNodes; n++)
                {
                    const bool is_selected = (firstNode == n);
                    if (ImGui::Selectable(this->userNodes.getnodo(n).ip.c_str(), is_selected)) {
                        firstNode = n;
                        secondNode = NO_SELECTION;
                    }

                    if (is_selected)
                        ImGui::SetItemDefaultFocus();
                }
                ImGui::EndCombo();
            }
            preview_text = (userNodes.nodos.size() != 0) ? ((secondNode != NO_SELECTION) ? this->userNodes.nodos[secondNode].ip.c_str() : "") : "";
            //preview_text = (cantNodes != 0) ? ((secondNode != NO_SELECTION) ? (this->userNodes.getnodo(secondNode)).ip.c_str() : "") : "";

            //Segunda combobox muestra opciones posibles de nodos para conectarse
            if (ImGui::BeginCombo("Neighbour Node", preview_text, 0))
            {
                if (firstNode != NO_SELECTION) {
                    for (int n = 0; n != cantNodes; n++)
                    {
                        //No se muestra la opcion del nodo ya seleccionado
                        if (n != firstNode) {
                            //En el caso que los dos sean nodos SPV, no se muestra esa opcion
                            if (!(!this->userNodes.getnodo(firstNode).nodofull && !this->userNodes.getnodo(n).nodofull)) {
                                const bool is_selected = (secondNode == n);
                                int neighbour = 0;
                                //Si ya tiene vecinos, no aparecen en la lista
                                if (this->userNodes.getnodo(firstNode).vecinos.size() != 0) {
                                    for (int i = 0; i != this->userNodes.getnodo(firstNode).vecinos.size() && neighbour == 0; i++) {
                                        if (this->userNodes.getnodo(firstNode).vecinos[i] == n) {
                                            neighbour++;
                                        }
                                    }
                                }
                                //Si no tiene vecinos, imprime opciones normalmente
                                if (neighbour == 0) {
                                    if (ImGui::Selectable(this->userNodes.getnodo(n).ip.c_str(), is_selected))
                                        secondNode = n;
                                }

                                if (is_selected)
                                    ImGui::SetItemDefaultFocus();
                            }
                        }
                    }
                }
                ImGui::EndCombo();
            }

            ImGui::Dummy(ImVec2(0.0f, 20.0f));
            //Si se apreta Connect y hay nodos seleccionados, se pasa a conectarlos y se limpian las combobox
            if (ImGui::Button("Connect")) {
                if (firstNode != NO_SELECTION && secondNode != NO_SELECTION) {
                    if (userNodes.conectnode(firstNode, secondNode)) {
                        firstNode = NO_SELECTION;
                        secondNode = NO_SELECTION;
                    }
                }
            }
            ImGui::SameLine();
            if (ImGui::Button("Close")) {
                doAction = NOTHING;
                firstNode = NO_SELECTION;
                secondNode = NO_SELECTION;
            }
            break;

        case SEND_MESSAGE:
            ImGui::TextColored(ImVec4(1, 1, 0, 1), "Send Message");
            cantNodes = this->userNodes.getcantnodos();
            preview_text = (userNodes.nodos.size() != 0) ? ((firstNode != NO_SELECTION) ? this->userNodes.nodos[firstNode].ip.c_str() : "") : "";
            //preview_text = (cantNodes != 0) ? ((firstNode != NO_SELECTION) ? (this->userNodes.getnodo(firstNode)).ip.c_str() : "") : "";
            //Primera combobox permite elegir el nodo desde el cual se va a enviar el mensaje
            if (ImGui::BeginCombo("Selected Node", preview_text, 0))
            {
                for (int n = 0; n != cantNodes; n++)
                {
                    const bool is_selected = (firstNode == n);
                    if (ImGui::Selectable(this->userNodes.getnodo(n).ip.c_str(), is_selected)) {
                        firstNode = n;
                        secondNode = NO_SELECTION;
                    }
                    if (is_selected)
                        ImGui::SetItemDefaultFocus();
                }
                ImGui::EndCombo();
            }
            preview_text = (userNodes.nodos.size() != 0) ? ((secondNode != NO_SELECTION) ? this->userNodes.nodos[secondNode].ip.c_str() : "") : "";
            //Segunda combobox muestra nodos vecinos para enviar mensaje 
            if (ImGui::BeginCombo("Neighbour Node", preview_text, 0))
            {
                int i = 0;
                if (firstNode != NO_SELECTION) {
                    for (int n = 0; n != userNodes.getnodo(firstNode).vecinos.size(); n++)
                    {
                        i = userNodes.getnodo(firstNode).vecinos[n];
                        const bool is_selected = (secondNode == i);
                        if (ImGui::Selectable(this->userNodes.getnodo(i).ip.c_str(), is_selected))
                            secondNode = i;

                        if (is_selected)
                            ImGui::SetItemDefaultFocus();
                    }
                }
                ImGui::EndCombo();
            }
            if (firstNode != NO_SELECTION) {
                if (this->userNodes.getnodo(firstNode).nodofull) {
                    ImGui::Combo("Message", &messageType, fullMessages, IM_ARRAYSIZE(fullMessages));
                    switch (messageType - 1) {
                    case 0: //"Block"
                        ImGui::TextColored(ImVec4(1, 1, 0, 1), "Message Parameters");
                        ImGui::InputText("Block", selection1, 50, ImGuiInputTextFlags_CharsDecimal);
                        break;
                    case 2: //"Merkleblock"
                        ImGui::TextColored(ImVec4(1, 1, 0, 1), "Message Parameters");
                        ImGui::InputText("Block", selection1, 50, ImGuiInputTextFlags_CharsDecimal);
                        ImGui::InputText("nTx", selection2, 50, ImGuiInputTextFlags_CharsDecimal);
                        break;
                    case 1: //"Transaction"
                    case 3: //"Get Blocks"
                        ImGui::TextColored(ImVec4(1, 1, 0, 1), "Message Parameters");
                        ImGui::InputText("Number", selection1, 50, ImGuiInputTextFlags_CharsDecimal);
                        ImGui::InputText("ID", selection2, 50);
                        break;
                    default:
                        break;
                    }
                }
                else {
                    ImGui::Combo("Node Type", &messageType, spvMessages, IM_ARRAYSIZE(spvMessages));
                    switch (messageType - 1) {
                    case 0: //"Transaction"
                    case 2: //"Get block header"
                        ImGui::TextColored(ImVec4(1, 1, 0, 1), "Message Parameters");
                        ImGui::InputText("Number", selection1, 50, ImGuiInputTextFlags_CharsDecimal);
                        ImGui::InputText("ID", selection2, 50);
                        break;
                    case 1: //"Filter"
                        break;
                    default:
                        break;
                    }
                }
            }

            ImGui::Dummy(ImVec2(0.0f, 20.0f));

            if (ImGui::Button("Send")) {
                //En caso de tener completos todos los campos, se realiza la conexion con el mensaje 
                if (firstNode != NO_SELECTION && secondNode != NO_SELECTION && messageType != 0) {
                    //clientconect(int node1, int node2, int option, int cant, std::string id, int bloque, int ntx, int& imgui) {
                    state = LOG;
                    connectionFinished = false;
                    logMessages.clear();
                }
                
                //MUESTRA ESTADO DE CONECCION. VOLVER A ESCUCHAR.
            }
            ImGui::SameLine();
            if (ImGui::Button("Close")) {
                doAction = NOTHING;
                firstNode = NO_SELECTION;
                secondNode = NO_SELECTION;
                memset(selection1, 0, sizeof(selection1));
                memset(selection2, 0, sizeof(selection2));
            }
            break;

            case SHOW_CONNECTIONS:
                ImGui::TextColored(ImVec4(1, 1, 0, 1), "Show Connections");
                ImGui::SameLine();
                if (ImGui::Button("Close")) {
                    doAction = NOTHING;
                }
                break;
        }

        ImGui::End();
        break;

    //OPEN_FILE: navegacion y seleccion sobre el path introducido por el usuario.
    case OPEN_FILE:

        ImGui::SetNextWindowSize(ImVec2(DISPLAY_SIZE_X, DISPLAY_SIZE_Y), ImGuiCond_Always);
        ImGui::Begin("File", &windowActive, ImGuiWindowFlags_MenuBar);

        //Menu Bar
        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("Menu"))
            {
                state = MAIN_WINDOW;
                doAction = NOTHING;
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Nodes"))
            {
                state = NODE;
                doAction = NOTHING;
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("File"))
            {
                state = OPEN_FILE;
                doAction = NOTHING;
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Blocks"))
            {
                state = BLOCKS;
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
            doAction = NOTHING;
            memset(pathName, 0, sizeof(pathName));
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
                    state = BLOCKS;
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

        //MAIN_WINDOW
    case LOG:
        ImGui::SetNextWindowSize(ImVec2(DISPLAY_SIZE_X, DISPLAY_SIZE_Y), ImGuiCond_Always);
        ImGui::Begin("Log", &windowActive, ImGuiWindowFlags_MenuBar);
        //Menu Bar
        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("Menu"))
            {
                state = MAIN_WINDOW;
                doAction = NOTHING;
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Nodes"))
            {
                state = NODE;
                doAction = NOTHING;
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("File"))
            {
                state = OPEN_FILE;
                doAction = NOTHING;
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Blocks"))
            {
                state = BLOCKS;
                doAction = NOTHING;
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }

        if (!connectionFinished) {
            nlohmann::json send = this->userNodes.clientconect(firstNode, secondNode, messageType - 1, atoi(selection1), std::string(selection2), atoi(selection1), atoi(selection2), checkConnectionState);
            if (send == nulljson) {
                checkConnectionState = BADPARAMETERS;
            }
        }
        ImGui::TextColored(ImVec4(1, 1, 0, 1), "Log");
        if (ImGui::Button("Close")) {
            state = MAIN_WINDOW;
            firstNode = NO_SELECTION;
            secondNode = NO_SELECTION;
            memset(selection1, 0, sizeof(selection1));
            memset(selection2, 0, sizeof(selection2));
        }
        ImGui::Separator();

        //Tiempo
        time_t date = time(0);
        struct tm* datetm = gmtime(&date);
        char* dateGMT = asctime(datetm);
        std::string str = "";
        if (dateGMT != NULL) {
            str.assign(dateGMT);
            str.pop_back();
        }

        ImGui::BeginChild("Scrolling");
        switch (checkConnectionState) {
        case CONNECTIONNOTHING:
            break;
        case CONECTIONOK:
            str += " Established connection between selected nodes.";
            logMessages.push_back(str);
            checkConnectionState = CONNECTIONNOTHING;
            break;
        case RESPONSEOK:
            str += " Response received.";
            logMessages.push_back(str);
            checkConnectionState = CONNECTIONNOTHING;
            break;
        case RESPONSEFAIL:
            str += " Response failed.";
            logMessages.push_back(str);
            checkConnectionState = CONNECTIONNOTHING;
            break;
        case CANTCONECT:
            str += " Cannot connect selected nodes.";
            logMessages.push_back(str);
            connectionFinished = true;
            checkConnectionState = CONNECTIONNOTHING;
            break;
        case CANCONECT:
            str += " Nodes are ready to connect.";
            logMessages.push_back(str);
            checkConnectionState = CONNECTIONNOTHING;
            break;
        case BADPARAMETERS:
            str += " Bad parameters introduced. Cannot send message.";
            logMessages.push_back(str);
            checkConnectionState = CONNECTIONNOTHING;
            break;
        case CONNECTIONFINISHED:
            str += " Connection finished.";
            logMessages.push_back(str);
            connectionFinished = true;
            checkConnectionState = CONNECTIONNOTHING;
            break;
        case ERRORCLIENT:
            str += " Error while connecting client.";
            logMessages.push_back(str);
            connectionFinished = true;
            checkConnectionState = CONNECTIONNOTHING;
            break;
        default:
            break;
        }
        for (int i = 0; i != logMessages.size(); i++) {
            ImGui::Dummy(ImVec2(0.0f, 10.0f));
            ImGui::Text((logMessages[i]).c_str());
        }
        ImGui::EndChild();
    
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
    static int sizeY = (int)(BUFFER_SIZE_Y / (treeHeight + 1));
    static int fontSize = 0;
    //Por optimizacion, se recalcula solamente al introducir un nuevo merkle tree.
    if (newMerkleTree) {
        treeHeight = merkleTree.size();
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

    int node = 0;
    int i = 1;
    //El primer for itera por los niveles del arbol.
    for (int height = 0; height != treeHeight; height++, i *= 2) {
        node = merkleTree[treeHeight - height - 1].size() - 1;
        //El segundo for grafica los nodos en cada nivel.
        int sizeX = (int)(BUFFER_SIZE_X / (i + 1));
        for (int j = i; node >= 0; j--, node--) {
            al_draw_text(font, WHITE, sizeX * (j), sizeY * (height), ALLEGRO_ALIGN_CENTRE, merkleTree[treeHeight - height - 1][node].c_str());
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
        }
    }

    al_set_target_backbuffer(display);
    al_draw_scaled_bitmap(buffer, 0, 0, BUFFER_SIZE_X, BUFFER_SIZE_Y, 5, DISPLAY_SIZE_Y - BUFFER_SIZE_Y, BUFFER_SIZE_X, DISPLAY_SIZE_Y, 0);
}

//Graficacion de conecciones
void Gui::drawNodesConnections(void) {
    //Grafica sobre el buffer
    al_set_target_bitmap(buffer);
    al_clear_to_color(BLACK);

    //Variables para calculo de dimensiones del buffer segun altura del arbol.
    static int nodes = 0;
    static int fontSize = 0;
    static float radius = 0;
    static float angle = 0;
    static float distance = 0;
    //Por optimizacion, se recalcula solamente al introducir un nuevo merkle tree.
    if (isNewNode) {
        nodes = userNodes.getcantnodos();
        angle = (2 * PI) / nodes;
        radius = (BUFFER_SIZE_Y - 10) / 2;
        distance = hypot(radius - radius* cos(angle), - radius * sin(angle));
        if (nodes == 1) {
            fontSize = (int)(radius / (nodes * 5));
        }
        else if (nodes < 4) {
            fontSize = (int)(distance / (nodes * 4));
        }
        else {
            fontSize = (int)(distance / (nodes));
        }
        al_destroy_font(font);
        font = al_load_ttf_font(TREE_FONT, fontSize, 0);
        if (font == NULL) {
            fprintf(stderr, "failed to create font!\n");
            return;
        }

        isNewNode = false;
    }

    //el primer for itera por los niveles del arbol
    for (int i = 0; i < nodes; i++) {
        if (nodes % 2 == 0) {
            al_draw_text(font, YELLOW, BUFFER_SIZE_X / 2 + radius * cos(angle * i + PI / 2), (radius - (radius * sin(angle * i + PI / 2) > radius/2 ? 5 : fontSize)) - radius * sin(angle * i + PI / 2), ALLEGRO_ALIGN_CENTRE, ("N" + std::to_string(i)).c_str());
        }
        else {
            al_draw_text(font, YELLOW, BUFFER_SIZE_X / 2 + radius * cos(angle * i + PI / 2), BUFFER_SIZE_Y / 2 - radius * sin(angle * i + PI / 2), ALLEGRO_ALIGN_CENTRE, ("N" + std::to_string(i)).c_str());
        }
    }
    al_set_target_backbuffer(display);
    al_draw_scaled_bitmap(buffer, 0, 0, BUFFER_SIZE_X, BUFFER_SIZE_Y, 5, DISPLAY_SIZE_Y - BUFFER_SIZE_Y, BUFFER_SIZE_X, BUFFER_SIZE_Y, 0);
}

//Graficacion de Imagen en Main Window
void Gui::drawMainWindow() {
    al_draw_bitmap(bitmap, 150,50,0);
}

int Gui::getDrawConnectionsState() {
    return (doAction == SHOW_CONNECTIONS);
}

//Devuelve Block deseado dentro del archivo json. 
std::string Gui::getSelectedFile(void) {
    if (doAction != CALC_MERKLE)
        doAction = NOTHING;
    return selectedPath.string();
}

//Devuelve cantidad de bloques exhibidas en la GUI. 
int Gui::getGuiCantBlocks(void) {
    return (blockInfo.size());
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

int Gui::getGuiState() {
    return state;
}

//Setter del Merkle Root calculado y real del bloque.
void Gui::setMerkleRoot(std::string merkleRootCalculated, std::string merkleRootBlock) {
    this->merkleRootCalculated = merkleRootCalculated;
    this->merkleRootBlock = merkleRootBlock;
}

//Setter del Merkle Tree.
void Gui::setMerkleTree(std::vector<std::vector<std::string>> merkle) {
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
    bitmap = al_load_bitmap(EDACOIN);
    if (buffer == NULL)
    {
        fprintf(stderr, "Failed to initialize bitmap!\n");
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