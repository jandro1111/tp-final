#include "Gui.h"
#include "files.h"

int main() {

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
	
	bool redraw = true;

	al_start_timer(timer);
	while (running) {
		// ---------- Obtencion de eventos ---------- //
		ALLEGRO_EVENT ev;
		while (al_get_next_event(queue, &ev)) {

			// Mandar el evento a Dear ImGui para que lo procese
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

					// ---------- Rendering  ---------- //
					ImGui::Render();

					al_clear_to_color(al_map_rgba_f(1, 1, 0.8, 1));

					//Todo lo que dibuje aca va a quedar por detrás de las ventanas de DearImGui

					ImGui_ImplAllegro5_RenderDrawData(ImGui::GetDrawData());

					//Todo lo que dibuje aca va a quedar por encima de las ventanas de DearImGui

					myGui.update();
					//redraw = false;
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

	ImGui_ImplAllegro5_Shutdown();
	ImGui::DestroyContext();

	myGui.destroyAllegro();
	al_destroy_event_queue(queue);
	al_destroy_display(myGui.display);

	return 0;
}