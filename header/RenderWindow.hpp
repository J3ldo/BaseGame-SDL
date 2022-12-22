#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_Image.h>
#include <chrono>

class Colour {
public:
	Colour(uint8_t p_r, uint8_t p_g, uint8_t p_b, uint8_t p_a = 0);
	uint8_t r;
	uint8_t g;
	uint8_t b;
	uint8_t a;
};

class CTexture { // Custom Texture
public:
	CTexture(SDL_Texture* p_texture);
	void Scale(const float amount);
	void SetDimensions(const int width, const int height);
	SDL_Rect* GetDst(int x, int y);
	SDL_Rect* GetSrc();
	SDL_Texture* GetTexture();
	void BlitTexture(SDL_Renderer* renderer, const int x, const int y);

	SDL_Rect src;
	SDL_Texture* texture;
	SDL_Rect dst;
private:
	float scaleFactor;
};

class Clock {
public:
	Clock(int StupidClassFixSetNull);
	void TickFPS(int p_fps);
	float GetFPS();
	float GetFPSRaw();
private:
	Uint64 lastSet = SDL_GetTicks();
	Uint64 lastGet = SDL_GetPerformanceCounter();
	int FpsRaw = 0;
	float FpsCalc = 0.f;
};

class RenderWindow {
public:
	RenderWindow(const char* p_title, int p_w, int p_h);
	void cleanUp();
	void clear();
	void update();

	CTexture loadTexture(const char* fp);
	void blitTexture(CTexture texture, int x, int y);

	SDL_Rect Rect(const int x, const int y, const int w, const int h);
	void BlitRect(SDL_Rect p_rect, Colour colour = Colour(0, 0, 0, 0), bool outlines = false);
	void DrawPixel(int x, int y, Colour colour);

	void fill(const int r, const int g, const int b);

	void GetImageDimensions(CTexture img, int* w, int* h);
	void GetImageDimensionsFromPath(const char* path, int* w, int* h);
	void SetTitle(const char* title);
	char GetTitle();
	void BlitRotate(CTexture img, const double angle, int x, int y, bool horizontal = true);

	SDL_Window* window;
	SDL_Renderer* renderer;
private: 
};




// GUI SHIT

class Button {
public:
	Button(SDL_Renderer* p_renderer, SDL_Window* p_window, int x, int y);
	bool isClicked();
	void Render();
private:
	SDL_Renderer* renderer;
	SDL_Window* window;
};

class Gui {
public:
	Gui(SDL_Renderer* p_renderer, SDL_Window* p_window, int width, int height, int x, int y);
	bool IsOn();
	void SwitchState();
	void SetColour(Colour p_colour);

	void RenderGui();
	Button AddButton();
private:
	SDL_Renderer* renderer;
	SDL_Window* window;
	SDL_Rect rect;
	Colour colour = Colour(211, 211, 211);

	bool activated = false;  // If the gui is on
};
