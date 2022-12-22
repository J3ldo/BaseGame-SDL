#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <string>
#include <thread>
#include <chrono>

#include "RenderWindow.hpp"

void show_fps(RenderWindow window, float* fps, const char* baseTitle) {
	while (1) {
		std::string title = baseTitle + std::to_string(*fps);
		window.SetTitle(title.c_str());

		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
}

// Comments are in dutct. But why would you read them?
int main(int argc, char* args[]) {
	std::cout << "Started\n";

	// Init classes
	RenderWindow window("Game", 1000, 500);
	Clock clk(NULL);
	SDL_Event event;

	// Init textures
	CTexture img = window.loadTexture("gfx/grass.png");
	img.Scale(1.5);

	// Init Threads
	float fps = 0.f;
	std::thread t1(show_fps, window, &fps, " - FPS: ");

	float angle = 0.f;
	float TimeDelta = 0;

	bool running = true;

	bool pressingW = false;
	bool pressingS = false;
	// Renderering hier
	while (running) {
		// Standaard constante events
		fps = clk.GetFPS();
		TimeDelta = 60 / fps;

		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				running = false;
			}
		}
		window.fill(255, 255, 0);

		angle += 4*TimeDelta;
		if (angle >= 360) angle = 0;
		window.BlitRotate(img, (int)angle, 450, 200); // Waarschijnlijk niet in het midden.


		window.update();
		window.clear();
	}

	window.cleanUp();

	// Kill threads
	t1.detach();

	SDL_Quit();

	return 0;
}