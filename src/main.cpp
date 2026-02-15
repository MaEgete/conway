#include "raylib.h"
#include <cstddef>
#include <cstdio>
#include <filesystem>
#include <vector>
#include <iostream>
#include <cstdint>



void animation_field(std::vector<uint8_t>& field, int step){

  if(field.empty()) return;

  std::fill(field.begin(), field.end(), 0);

  // Fun animation
  step = step % field.size() + 1;

  for(size_t i = 0; i < field.size(); ++i){
    if(i % step == 0){
      field[i] = 1;
    }
    
  }

}


void set_block(std::vector<uint8_t>& field, int x, int y, int block_count){

  // Hier nicht zeichnen, sondern die Werte im Field setzen!!!
  
  field[y * block_count + x] = 1;

}

void initialize_field(std::vector<uint8_t>& field, const std::vector<int>& x_vec, const std::vector<int>& y_vec, int block_count){

  if(field.empty()){
    std::cout << "field is empty" << std::endl;
    return;
  }

  if(x_vec.empty() || y_vec.empty()){
    std::cout << "x_vec or y_vec is empty" << std::endl;
    return;
  }

  if(x_vec.size() != y_vec.size()){
    std::cout << "x_vec and y_vec have not the same size" << std::endl;
    return;
  }

  // Alle Bloecke setzen
  for(int i = 0; i < x_vec.size(); i++){

    if(x_vec[i] < 0 || x_vec[i] >= block_count){
      std::cout << "x_vec[" << i << "] is out of range!" << std::endl;
      continue;
    }

    if(y_vec[i] < 0 || y_vec[i] >= block_count){
      std::cout << "y_vec[" << i << "] is out of range!" << std::endl;
      continue;
    }

    set_block(field, x_vec[i], y_vec[i], block_count);
  }

}


void printBlock(int x, int y, int block_size, uint8_t val){

  // Wenn val == 1, dann WHITE, ansonsten BLACK
  DrawRectangle(x, y, block_size, block_size, (val ? WHITE : BLACK));

}

void printField(std::vector<uint8_t>& field, const int block_size, const size_t block_count){

  auto at = [&](size_t x, size_t y) -> uint8_t& {
    return field[y * block_count + x];
  };


  // Konsolen Test Ausgabe:
  
  for(int i = 0; i < field.size(); i++){
    int y = i / block_count;
    int x = i % block_count;

    // if(x == 0){
    //   std::cout << std::endl;
    // }

    // DEBUG: std::cout << "x: " << x << ", y: " << y << std::endl;

    // std::cout << static_cast<int>(at(x, y)) << " ";

    // Block an der Richtigen Stelle ausgeben
    printBlock(x * block_size, y * block_size, block_size, at(x,y));
    
  }

}



int main(void){



  // Camera2D noch hinzufuegen

  const int width = 1200;
  const int height = width;
  
  InitWindow(width, height, "Conway's Game of Life");


  // Ein Feld soll deafult 10px gross/breit sein
  const int block_size = 10;

  // Anzahl an Blocken pro Zeile bzw Spalte. Das Feld ist quadratisch!!!, aber dennoch Eindimensional
  const size_t block_count = GetScreenWidth() / block_size;

  // Quadratisches Feld -> [y][x]
  // Beinhaltet nur 8 Bit Integer Werte:
  //   0 = block ist schwarz
  //   1 = block ist weiss
  std::vector<uint8_t> field(block_count * block_count, 0);


  //int step = 0;

  int test = block_count-1;
  std::vector<int> x_vec{0, 1, test};
  std::vector<int> y_vec{0, 1, test};

  initialize_field(field, x_vec, y_vec, block_count);
  

  while(!WindowShouldClose()){

    BeginDrawing();

    ClearBackground(BLACK);

      // animation_field(field, step++);

      printField(field, block_size, block_count);

    EndDrawing();

  }

  CloseWindow();

  std::cout << std::endl;

  return 0;
  
}

