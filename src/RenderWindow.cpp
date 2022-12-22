#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <iostream>
#include <chrono>
#include <thread>

#include "RenderWindow.hpp"

RenderWindow::RenderWindow(const char* title, int width, int height) 
	:window(NULL), renderer(NULL)
{
	window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, 0);

	if (window == NULL) {
		std::cout << "Failed to init window. Error:" << SDL_GetError() << std::endl;
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
}

void RenderWindow::SetTitle(const char* title) {
	SDL_SetWindowTitle(window, title);
}

char RenderWindow::GetTitle() {
	return *SDL_GetWindowTitle(window);
}

CTexture RenderWindow::loadTexture(const char* fp) {
	SDL_Texture* texture = NULL;
	texture = IMG_LoadTexture(renderer, fp);

	if (texture == NULL) {
		std::cout << "Failed to load texture. Error: " << SDL_GetError() << std::endl;
	}

	return CTexture(texture);
}

void RenderWindow::blitTexture(CTexture texture, int x, int y) {
	SDL_RenderCopy(renderer, texture.texture, &texture.src, texture.GetDst(x, y));
}

void RenderWindow::DrawPixel(int x, int y, Colour colour) {

	uint8_t r, g, b, a;
	SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a);
	SDL_SetRenderDrawColor(renderer, colour.r, colour.g, colour.b, colour.a);

	SDL_RenderDrawPoint(renderer, x, y);

	SDL_SetRenderDrawColor(renderer, r, g, b, a);
}

SDL_Rect RenderWindow::Rect(const int x, const int y, const int w, const int h) {
	SDL_Rect rect = SDL_Rect();
	rect.x = x;
	rect.y = y;
	rect.w = w;
	rect.h = h;

	return rect;
}

void RenderWindow::BlitRect(SDL_Rect p_rect, Colour colour, bool outlines) {
	SDL_Rect* rect = &p_rect;

	uint8_t r, g, b, a;
	SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a);
	SDL_SetRenderDrawColor(renderer, colour.r, colour.g, colour.b, colour.a);

	if (outlines) {
		SDL_RenderDrawRect(renderer, rect);
	}
	else if (!outlines) {
		SDL_RenderFillRect(renderer, rect);
	}
	SDL_SetRenderDrawColor(renderer, r, g, b, a);
}

void RenderWindow::cleanUp(){
	SDL_DestroyWindow(window);
}

void RenderWindow::clear() {
	SDL_RenderClear(renderer);
}

void RenderWindow::update(){
	SDL_RenderPresent(renderer);
}

void RenderWindow::fill(const int r, const int g, const int b) {
	SDL_SetRenderDrawColor(renderer, r, g, b, 255);
}

void RenderWindow::GetImageDimensions(CTexture img, int* w, int* h) {
	SDL_QueryTexture(img.texture, NULL, NULL, w, h);
}

void RenderWindow::GetImageDimensionsFromPath(const char* path, int* w, int* h) {
	SDL_Texture* img = IMG_LoadTexture(renderer, path);
	GetImageDimensions(img, w, h);
}

void RenderWindow::BlitRotate(CTexture img, const double angle, int x, int y, bool horizontal) {
	SDL_Rect dst = *img.GetDst(x, y);

	if (horizontal) SDL_RenderCopyEx(renderer, img.texture, NULL, &dst, angle, NULL, SDL_FLIP_HORIZONTAL);
	else if (!horizontal) SDL_RenderCopyEx(renderer, img.texture, NULL, &dst, angle, NULL, SDL_FLIP_VERTICAL);
}

Colour::Colour(uint8_t p_r, uint8_t p_g, uint8_t p_b, uint8_t p_a)
{
	r = p_r;
	g = p_g;
	b = p_b;
	a = p_a;
}

CTexture::CTexture(SDL_Texture* p_texture) {
	texture = p_texture;

	scaleFactor = 1.f;

	src = SDL_Rect();
	SDL_QueryTexture(texture, NULL, NULL, &src.w, &src.h);

	dst = SDL_Rect();
	dst.w = (int)src.w * scaleFactor;
	dst.h = (int)src.h * scaleFactor;
}

void CTexture::Scale(const float amount) {
	if (amount == scaleFactor) return;

	scaleFactor = amount;
	dst.w = (int)src.w*amount;
	dst.h = (int)src.h*amount;
}

SDL_Rect* CTexture::GetDst(int x, int y) {
	dst.x = x;
	dst.y = y;
	return &dst;
}
SDL_Rect* CTexture::GetSrc() {
	return &src;
}
SDL_Texture* CTexture::GetTexture() {
	return texture;
}
void CTexture::BlitTexture(SDL_Renderer* renderer, const int x, const int y) {
	SDL_RenderCopy(renderer, texture, &src, GetDst(x, y));
}

void CTexture::SetDimensions(const int width, const int height) {
	if (width == dst.w && height == dst.h) return;

	dst.w = width;
	dst.h = height;
}

Gui::Gui(SDL_Renderer* p_renderer, SDL_Window* p_window, int width, int height, int x, int y) {
	renderer = p_renderer;
	window = p_window;

	rect = SDL_Rect();
	rect.w = width;
	rect.h = height;
	rect.x = x;
	rect.y = y;
}

bool Gui::IsOn()
{
	return activated;
}

void Gui::SwitchState()
{
	activated = !activated;
}

void Gui::RenderGui()
{
	if (!activated) return;

	uint8_t r, g, b, a;
	SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a);
	SDL_SetRenderDrawColor(renderer, colour.r, colour.g, colour.b, colour.a);

	SDL_RenderFillRect(renderer, &rect);

	SDL_SetRenderDrawColor(renderer, r, g, b, a);
}

void Gui::SetColour(Colour p_colour)
{
	colour = p_colour;
}

Clock::Clock(int StupidClassFixSetNull)
{
	lastSet = SDL_GetTicks();
	lastGet = SDL_GetPerformanceCounter();
}

void Clock::TickFPS(int p_fps) {
	if (FpsCalc != p_fps) FpsCalc = 1.000000f / p_fps * 1000;
	FpsRaw = p_fps;
	

	float takenms = (SDL_GetTicks() - lastGet)/(float)SDL_GetPerformanceFrequency() * 1000.0f;

	if (takenms < FpsCalc) {
		SDL_Delay(floor(FpsCalc -takenms));
	}

	lastGet = SDL_GetTicks();
}


float Clock::GetFPS() {
	float takensec = 1.f/((SDL_GetPerformanceCounter() - lastSet) / (float)SDL_GetPerformanceFrequency());
	lastSet = SDL_GetPerformanceCounter();

	return takensec;
}

float Clock::GetFPSRaw() {
	float takensec = ((SDL_GetPerformanceCounter() - lastSet) / (float)SDL_GetPerformanceFrequency());
	lastSet = SDL_GetPerformanceCounter();

	return takensec;
}