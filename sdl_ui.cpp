#include <SDL2/SDL.h>
#include <emscripten.h>
#include <cstdlib>
#include "game_of_life.h"

struct Context {
  SDL_Renderer* renderer;
  GameOfLife* game;
  int cell_size;
};

void mainloop(void* arg) {
  Context* ctx = static_cast<Context*>(arg);
  SDL_Renderer* renderer = ctx->renderer;
  GameOfLife* game = ctx->game;
  
  // Clear the screen with black background
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);
  
  // Draw the game grid
  const std::vector<std::vector<uint8_t>>& grid = game->GetGrid();
  int width = game->GetWidth();
  int height = game->GetHeight();
  int cell_size = ctx->cell_size;
  
  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      // Draw cell fill (smaller by 1 pixel to create gap)
      SDL_Rect fill_rect;
      fill_rect.x = x * cell_size + 1;
      fill_rect.y = y * cell_size + 1;
      fill_rect.w = cell_size - 2;
      fill_rect.h = cell_size - 2;
      
      if (grid[y][x] == 1) {
        // Live cell: black fill
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
      } else {
        // Dead cell: white fill
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
      }
      SDL_RenderFillRect(renderer, &fill_rect);
      
      // Draw black border for all cells
      SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
      SDL_Rect border_rect;
      border_rect.x = x * cell_size + 1;
      border_rect.y = y * cell_size + 1;
      border_rect.w = cell_size - 2;
      border_rect.h = cell_size - 2;
      SDL_RenderDrawRect(renderer, &border_rect);
    }
  }
  
  SDL_RenderPresent(renderer);
  
  // Update the game state for next iteration
  game->Iterate();
}

int main() {
  const int kGridWidth = 48;
  const int kGridHeight = 48;
  const int kCellSize = 15; // pixels per cell
  const int kWindowWidth = kGridWidth * kCellSize;
  const int kWindowHeight = kGridHeight * kCellSize;
  
  SDL_Init(SDL_INIT_VIDEO);
  SDL_Window* window;
  SDL_Renderer* renderer;
  SDL_CreateWindowAndRenderer(kWindowWidth, kWindowHeight, 0, &window, &renderer);
  
  // Create and initialize the Game of Life
  GameOfLife game(kGridWidth, kGridHeight);
  game.Init();
  
  Context ctx;
  ctx.renderer = renderer;
  ctx.game = &game;
  ctx.cell_size = kCellSize;
  
  const int simulate_infinite_loop = 1; // call the function repeatedly
  const int fps = 10; // Update 10 times per second for better visibility
  emscripten_set_main_loop_arg(mainloop, &ctx, fps, simulate_infinite_loop);
  
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  
  return EXIT_SUCCESS;
}