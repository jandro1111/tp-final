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
	//blockchain algo("ejemplo.json");
	//nlohmann::json bloque = algo.getblock(0);
	//std::string aux = str(boost::format("\"tx\":[%1%,%2%,%3%,%4%,%5%]") % algo.gettx(bloque, 0) % algo.gettx(bloque, 0) % algo.gettx(bloque, 1) % algo.gettx(bloque, 2) % algo.gettx(bloque, 3));
	//
	//for (int i = 0; i < 800; ++i) {//sacar el for y meter dentro del sistema de polling
	//	newbloque = algo.mine(aux, 5, newbloque);//tener en cuenta que block id es el hash de minado, osea que a la hora de minar block id es ""
	//	if (newbloque == true)
	//		break;
	//}
	//bloque = algo.getblock(5);
	//cout << endl << endl << endl << bloque << endl;
	//
	//blockchain algo2("ejemplo.json");



	//
	//int option = 0;//el tipo de request que voy a hacer
	//cliente algo;
	//std::string request[6] = { "send_block/","send_tx/","send_merkle_block/","send_filter/","get_blocks","get_block_header" };
	//std::string id = "534F219B";
	//std::string publicid = "8745926946298734";
	//int amount = 2;
	//std::string data = "";
	//int cant = 2;
	//int bloque = 0;
	//int tx = 0;
	//std::string ip = "127.0.0.1/";
	//int imgui = 0;
	//	int puerto = 80;
	//	int puertoc = 80;
	//	boost::asio::io_context io_context;//
	//	server sv(io_context,puertoc);//
	//int nodeuse;
	//std::cin >> nodeuse;
	//algo.configClient();
	//try{
	//	switch (nodeuse) {
	//	case 1://cliente
	//		for (int i = 0; i < 6; ++i) {
	//			switch (i) {//despues sacar afuera y cambiar i x option
	//			case 0:
	//				data = algo2.getblock(bloque).dump();
	//				break;
	//			case 1:
	//				data = algo2.sendtx(amount, publicid);
	//				break;
	//			case 2:
	//				data = algo2.makemerkleblock(bloque, tx);
	//				break;
	//			case 3:
	//				data = "{\"Key\" : \"pubkey1\"}";
	//				break;
	//			default:
	//				break;
	//			}
	//			ip += request[i];//cambiar i x option
	//			while (!imgui) {
	//				algo.client(ip, puerto, i, cant, id, data.c_str(), imgui);//despues cambiar i por option	
	//			}
	//			std::cout << "put any number to continue test" << std::endl;
	//			cin >> nodeuse;
	//			ip = "127.0.0.1/";
	//			data = "";
	//		}
	//		break;
	//case 2://servidor comentar si se esta probando con el servidor de hercules
	//	try
	//	{
	//		sv.start();
	//		io_context.run();
	//	}
	//	catch (std::exception& e)
	//	{
	//		std::cerr << e.what() << std::endl;
	//	}
	//	break;
	//default:
	//	break;

	//}
	//}
	//catch (std::exception& e) {
	//	std::cerr << e.what() << std::endl << "no se pudo conectar a ese nodo" << std::endl;
	//	imgui = 1;
	//	//imgui = RESPONSEFAIL;
	//}

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

			/* Manda el evento a Dear ImGui para que lo procese
			ImGui_ImplAllegro5_ProcessEvent(&ev);


			Servidores y clientes
*/

			switch (ev.type) {
			case ALLEGRO_EVENT_TIMER:
				if (ev.timer.source == timer) {

					 //---------- Inicializacion frame ---------- //
					ImGui_ImplAllegro5_NewFrame();
					ImGui::NewFrame();
					ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);

					 //---------- Widgets y ventanas  ---------- //
					myGui.GUIwindow();

					//De haberse seleccionado algun archivo, se lo accede y se pasa la información de los bloques
					//if (myGui.isNewPath()) {
					//	blockchain myBlockchain(myGui.getSelectedFile());
					//	nlohmann::json block;
					//	for (int i = 0; i < myBlockchain.getcantblock(); i++) {
					//		block = myBlockchain.getblock(i);
					//		if (block != nulljson) {
					//			myGui.setBlockInfo(myBlockchain.getblockid(block), myBlockchain.getpreviousblockid(block),
					//				myBlockchain.getnTx(block), myBlockchain.getheight(block), myBlockchain.getnonce(block));
					//		}
					//	}
					//}

					//MINADO
					newblock = myBlockchain.mine(aux, 0, newblock);//tener en cuenta que block id es el hash de minado, osea que a la hora de minar block id es ""


					//Al minarse un nuevo bloque, debe actualizarse la información contenida en la GUI. 
					if (newblock) {
						//Se cambia archivo fuente de blockchain.
						if (myGui.isNewPath()) {
							myBlockchain.setnewfile(myGui.getSelectedFile());
						}
					
						std::cout << std::endl;
						std::cout << std::endl;
						std::cout << std::endl;
						std::cout << "NUEVO BLOQUE" << std::endl;
						std::cout << std::endl;
						std::cout << std::endl;
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

					int selectedBlock = myGui.getSelectedBlock();
					if (selectedBlock != NO_SELECTION) {
						blockchain myBlockchain(myGui.getSelectedFile());
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
