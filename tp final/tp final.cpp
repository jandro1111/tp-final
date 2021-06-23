// tp final.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include"Header.h"
#include"blockchain.h"
#include "Gui.h"
#include "files.h"
#include "cliente.h"
#include"server.h"
using namespace std;

int main()
{
	bool newblock = true;
	srand(time(NULL));

	/////////////////////////////
	///Blockchain por default///
	////////////////////////////
	blockchain myBlockchain("ejemplo.json");
	std::string aux = str(boost::format("\"tx\":[]") );

	bool running = true;
	Gui myGui;
	myGui.initGUI();
	ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
	if (!queue)
	{
		fprintf(stderr, "Failed to create event queue!\n");
		al_destroy_display(myGui.display);
		return -1;
	}

	ALLEGRO_TIMER* timer = al_create_timer(1.0 / FPS);

	al_register_event_source(queue, al_get_display_event_source(myGui.display));
	al_register_event_source(queue, al_get_display_event_source(myGui.display));
	al_register_event_source(queue, al_get_keyboard_event_source());
	al_register_event_source(queue, al_get_mouse_event_source());
	al_register_event_source(queue, al_get_timer_event_source(timer));


	al_start_timer(timer);
	while (running) {


		 //---------- Obtencion de eventos ---------- //
		ALLEGRO_EVENT ev;
		while (al_get_next_event(queue, &ev)) {

			//Manda el evento a Dear ImGui para que lo procese
			ImGui_ImplAllegro5_ProcessEvent(&ev);


			//Servidores y clientes

			switch (ev.type) {
			case ALLEGRO_EVENT_TIMER:
				if (ev.timer.source == timer) {

					 //---------- Inicializacion frame ---------- //
					ImGui_ImplAllegro5_NewFrame();
					ImGui::NewFrame();
					ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);

					 //---------- Widgets y ventanas  ---------- //
					myGui.GUIwindow();

					//Al minarse un nuevo bloque, debe actualizarse la información contenida en la GUI. 
					if (newblock) {
						//Se cambia archivo fuente de blockchain.
						if (myGui.isNewPath()) {
							myBlockchain.setnewfile(myGui.getSelectedFile());
						}
						//Carga de informacion de bloques en Gui.
						int blockchainBlocks = myBlockchain.getcantblock();
						int guiBlocks = myGui.getGuiCantBlocks();
						nlohmann::json block;

						if(blockchainBlocks > guiBlocks) {
							for (int i = guiBlocks; i < blockchainBlocks; i++) {
								block = myBlockchain.getblock(i);
								if (block != nulljson) {
									myGui.setBlockInfo(myBlockchain.getblockid(block), myBlockchain.getpreviousblockid(block),
										myBlockchain.getnTx(block), myBlockchain.getheight(block), myBlockchain.getnonce(block));
								}
							}
						}
					}

					//MINADO
					newblock = myBlockchain.mine(aux, 0, newblock);//tener en cuenta que block id es el hash de minado, osea que a la hora de minar block id es ""

					int selectedBlock = myGui.getSelectedBlock();
					if (selectedBlock != NO_SELECTION) {
						nlohmann::json block = myBlockchain.getblock(selectedBlock);
						myGui.setMerkleRoot(myBlockchain.calculatemerkleroot(selectedBlock), myBlockchain.getmerkleroot(block));
						myGui.setMerkleTree(myBlockchain.calculatemerkletree(selectedBlock));
					}

					 //---------- Rendering  ---------- //
					ImGui::Render();

					al_clear_to_color(BLACK);
					//Todo lo que dibuje aca va a quedar por detrás de las ventanas de DearImGui


					ImGui_ImplAllegro5_RenderDrawData(ImGui::GetDrawData());

					//Todo lo que dibuje aca va a quedar por encima de las ventanas de DearImGui

					if (myGui.isMerkleTree()) {
						myGui.drawMerkleTree();
					}
					if (myGui.getDrawConnectionsState()) {
						myGui.drawNodesConnections();
					}
					if (myGui.getGuiState() == MAIN_WINDOW) {
						myGui.drawMainWindow();
					}

					myGui.update();
				}
				break;

			case ALLEGRO_EVENT_DISPLAY_CLOSE:
				running = false;
				break;

			case ALLEGRO_EVENT_DISPLAY_RESIZE:
				ImGui_ImplAllegro5_InvalidateDeviceObjects();
				al_acknowledge_resize(myGui.display);
				ImGui_ImplAllegro5_CreateDeviceObjects();
				break;
			}
		}
	}

	//Destructores ImGui
	ImGui_ImplAllegro5_Shutdown();
	ImGui::DestroyContext();

	//Destructores Allegro
	al_destroy_timer(timer);
	al_destroy_event_queue(queue);
	myGui.destroyAllegro();
	
	return 0;
}
