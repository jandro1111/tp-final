// tp final.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include"Header.h"
#include"blockchain.h"
#include "Gui.h"
#include "files.h"

using namespace std;

int main()
{

	int num, numoutofrange;
	num = 0;
	numoutofrange = 10;
	string aux;
	nlohmann::json bloque, tx, vin, vout;
	std::string file = "ejemplo.json";
	blockchain prueba(file);
	bloque = prueba.getblock(num);//pasar num como si fuese un indice
	tx = prueba.gettx(bloque, 0);
	tx = prueba.gettx(bloque, num);//pasar num como indice
	vin = prueba.getvin(tx, num);//pasar num como indice
	vout = prueba.getvout(tx, num);//pasar num como indice

	cout << "cantidad de bloques: " << prueba.getcantblock() << endl;
	cout << "id del bloque: " << prueba.getblockid(bloque) << endl;
	cout << "id del bloque anterior: " << prueba.getpreviousblockid(bloque) << endl;
	cout << "height: " << prueba.getheight(bloque) << endl;
	cout << "merkleroot: " << prueba.getmerkleroot(bloque) << endl;
	cout << "nonce: " << prueba.getnonce(bloque) << endl;
	cout << "numero de transacciones: " << prueba.getnTx(bloque) << endl;
	cout << "id de la  transaccion 1: " << prueba.gettxid(tx) << endl;
	cout << "cant de vin en transaccion 1: " << prueba.getnTxin(tx) << endl;
	cout << "cant de vout en transaccion 1: " << prueba.getnTxout(tx) << endl;
	cout << "en vin 1: " << endl;
	cout << "block id: " << prueba.getblockid(vin) << endl;
	cout << "tx id: " << prueba.gettxid(vin) << endl;
	cout << "outputindex: " << prueba.getoutputIndex(vin) << endl;
	cout << "signature: " << prueba.getsignature(vin) << endl;
	cout << "en vout 1: " << endl;
	cout << "amount: " << prueba.getamount(vout) << endl;
	cout << "publicid: " << prueba.getpublicid(vout) << endl << endl;
	prueba.calculatemerkleroot(num);


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
	ALLEGRO_FONT* font;
	font = al_load_ttf_font(TREE_FONT, 40, 0);
	if (font == NULL) {
		fprintf(stderr, "failed to create font!\n");
		return -1;
	}
	bool redraw = true;

	//PRUEBA
	vector <std::string> tree;
	for (int i = 0; i < 15; i++) {
		tree.push_back("1234");
	}


	al_start_timer(timer);
	while (running) {

		// ---------- Obtencion de eventos ---------- //
		ALLEGRO_EVENT ev;
		while (al_get_next_event(queue, &ev)) {

			// Manda el evento a Dear ImGui para que lo procese
			ImGui_ImplAllegro5_ProcessEvent(&ev);

			switch (ev.type) {
			case ALLEGRO_EVENT_TIMER:
				if (ev.timer.source == timer) {

					// ---------- Inicializacion frame ---------- //
					ImGui_ImplAllegro5_NewFrame();
					ImGui::NewFrame();
					ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);

					// ---------- Widgets y ventanas  ---------- //
					myGui.mainWindow();

					//De haberse seleccionado algun archivo, se lo accede y se pasa la información de los bloques
					if (myGui.isNewPath()) {
						blockchain myBlockchain(myGui.getSelectedFile());
						nlohmann::json block;
						for (int i = 0; i < myBlockchain.getcantblock(); i++) {
							block = myBlockchain.getblock(i);
							if (block != nulljson) {
								myGui.setBlockInfo(myBlockchain.getblockid(block), myBlockchain.getpreviousblockid(block),
									myBlockchain.getnTx(block), myBlockchain.getheight(block), myBlockchain.getnonce(block));
							}
						}
					}
					if (myGui.getSelectedBlock() != NO_SELECTION) {
						//myGui.setMerkleRoot();
						myGui.setMerkleTree(tree);
					}

					// ---------- Rendering  ---------- //
					ImGui::Render();

					//al_clear_to_color(al_map_rgba_f(1, 1, 0.8, 1));
					al_clear_to_color(BLACK);
					//Todo lo que dibuje aca va a quedar por detrás de las ventanas de DearImGui


					ImGui_ImplAllegro5_RenderDrawData(ImGui::GetDrawData());

					//Todo lo que dibuje aca va a quedar por encima de las ventanas de DearImGui

					if (myGui.isMerkleTree()) {
						myGui.drawMerkleTree();
					}
					myGui.update();
				}
				break;

			case ALLEGRO_EVENT_DISPLAY_CLOSE:
				running = false;
				break;

			case ALLEGRO_EVENT_DISPLAY_RESIZE:
				//ImGui_ImplAllegro5_InvalidateDeviceObjects();
				//al_acknowledge_resize(myGui.display);
				//ImGui_ImplAllegro5_CreateDeviceObjects();
				break;
			}
		}
	}

	//Destructores ImGui
	ImGui_ImplAllegro5_Shutdown();
	ImGui::DestroyContext();

	//Destructores Allegro
	myGui.destroyAllegro();
	al_destroy_timer(timer);
	al_destroy_event_queue(queue);

	return 0;
}
