#ifndef GAME_OF_LIFE_H
#define GAME_OF_LIFE_H

#include <vector>
#include <random>
#include <cstdint>

class GameOfLife {
 public:
  GameOfLife(int width, int height) 
      : width_(width), height_(height) {
    std::random_device rd;
    rng_.seed(rd());
    
    // Initialize 2D grids
    grid_.resize(height_, std::vector<uint8_t>(width_, 0));
    next_grid_.resize(height_, std::vector<uint8_t>(width_, 0));
  }
  
  void Init() {
    std::uniform_int_distribution<int> dist(0, 1);
    for (int y = 0; y < height_; ++y) {
      for (int x = 0; x < width_; ++x) {
        grid_[y][x] = dist(rng_) ? 1 : 0;
      }
    }
  }
  
  void Iterate() {
    for (int y = 0; y < height_; ++y) {
      for (int x = 0; x < width_; ++x) {
        int neighbors = CountNeighbors(x, y);
        
        // Conway's Game of Life rules
        if (grid_[y][x] == 1) {
          // Cell is alive
          if (neighbors < 2 || neighbors > 3) {
            next_grid_[y][x] = 0; // Dies from underpopulation or overpopulation
          } else {
            next_grid_[y][x] = 1; // Survives
          }
        } else {
          // Cell is dead
          if (neighbors == 3) {
            next_grid_[y][x] = 1; // Becomes alive from reproduction
          } else {
            next_grid_[y][x] = 0; // Stays dead
          }
        }
      }
    }
    SwapGrids();
  }
  
  const std::vector<std::vector<uint8_t>>& GetGrid() const { return grid_; }
  int GetWidth() const { return width_; }
  int GetHeight() const { return height_; }

 private:
  int width_;
  int height_;
  std::vector<std::vector<uint8_t>> grid_;
  std::vector<std::vector<uint8_t>> next_grid_;
  std::mt19937 rng_;
  
  int CountNeighbors(int x, int y) const {
    int count = 0;
    for (int dy = -1; dy <= 1; ++dy) {
      for (int dx = -1; dx <= 1; ++dx) {
        if (dx == 0 && dy == 0) continue; // Skip the cell itself
        
        int nx = (x + dx + width_) % width_; // Wrap around edges
        int ny = (y + dy + height_) % height_;
        
        if (grid_[ny][nx] == 1) {
          count++;
        }
      }
    }
    return count;
  }
  
  void SwapGrids() {
    grid_.swap(next_grid_);
  }
};

#endif // GAME_OF_LIFE_H