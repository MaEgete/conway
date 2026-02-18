#include "raylib.h"
#include <cstddef>
#include <cstdio>
#include <fstream>
#include <vector>
#include <iostream>
#include <cstdint>
#include <string>


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
    std::cout << "size of x_vec: " << x_vec.size() << std::endl;
    std::cout << "size of y_vec: " << y_vec.size() << std::endl;

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


  /*
    0 | 0 | 0
    0 | 1 | 0
    0 | 0 | 0

    Oben:         -y        =>  val - block_count
    Unten:        +y        =>  val + block_count
    Links:        -x        =>  val - 1
    Rechts:       +x        =>  val + 1
    Links Oben:   -x && -y  =>  val - 1 - block_count
    Rechts Oben:  +x && -y  =>  val + 1 - block_count
    Links Unten:  -x && +y  =>  val - 1 + block_count
    Rechts Unten: +x && +y  =>  val + 1 + block_count

  */

int countNeighbours(int x, int y, const std::vector<uint8_t>& field, int block_count) {
    int n = 0;

    auto at = [&](int xx, int yy) -> int {
        if (xx < 0 || xx >= block_count || yy < 0 || yy >= block_count) return 0;
        return field[yy * block_count + xx] ? 1 : 0;
    };

    for (int dy = -1; dy <= 1; ++dy) {
        for (int dx = -1; dx <= 1; ++dx) {
            if (dx == 0 && dy == 0) continue;
            n += at(x + dx, y + dy);
        }
    }

    return n;
}



// Verändert das field, indem die Regeln angewandt werden
    /*
        Any live cell with fewer than two live neighbours dies, as if by underpopulation.
        Any live cell with two or three live neighbours lives on to the next generation.
        Any live cell with more than three live neighbours dies, as if by overpopulation.
        Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction.
    */
void rules(std::vector<uint8_t>& field, int block_count) {
    std::vector<uint8_t> next(field.size(), 0);

    for (int y = 0; y < block_count; ++y) {
        for (int x = 0; x < block_count; ++x) {
            int i = y * block_count + x;
            bool alive = field[i] != 0;
            int neighbours = countNeighbours(x, y, field, block_count);

            if (alive) {
                next[i] = (neighbours == 2 || neighbours == 3) ? 1 : 0;
            } else {
                next[i] = (neighbours == 3) ? 1 : 0;
            }
        }
    }

    field.swap(next);


}

std::string field_to_string(const std::vector<uint8_t>& field, int block_count){

  std::string values;

  int blocks = 1;
  for(const auto& val : field){
    values += std::to_string(val);

    if(blocks >= field.size()){
      break;
    }

    if(blocks % block_count == 0){
      values += "\n";
    }

    blocks++;
  }

  return values;
}

// returns -1 when failing, otherwise 0
int save(const std::string& filename, const std::vector<uint8_t>& field, int block_count){

  std::cout << filename << " wird gespeichert ..." << std::endl;

  std::ofstream file{filename};

  if(!file.is_open()){
    std::cout << "File could not be opened" << std::endl;
    return -1;
  }

  file << field_to_string(field, block_count);

  std::cout << filename << " wurde erfolgreich gespeichert!" << std::endl;

  file.close();

  return 0;

}


std::vector<uint8_t> load_field_from_save(std::string filename, int block_count){

  // Field erstellen
  std::vector<uint8_t> field(block_count * block_count, 0);

  std::ifstream file(filename);

  if(!file.is_open()){
    std::cout << filename << " could not be opended!" << std::endl;
    return field;
  }

  std::string line;

  // Field aus dem Save in den vector field laden
  int idx = 0;
  while(std::getline(file, line)){
    //std::cout << line << std::endl;

    for(char c : line){
      if(c == '1'){
        field[idx] = 1;
      }
      idx++;
    }
  }

  file.close();

  return field;

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


  std::string filename = "field_save.txt";

  std::vector<uint8_t> field;

  // Koordinaten fuer Initialisierungsbloecke
  std::vector<int> x_vec{0, 1, 0, 1};
  std::vector<int> y_vec{0, 0, 1, 1};

  // Tub
  x_vec = {2, 1, 3, 2};
  y_vec = {1, 2, 2, 3};

  // Boat
  x_vec = {2, 1, 3, 2, 3};
  y_vec = {1, 2, 2, 3, 3};

  // Snake
  x_vec = {1, 3, 4, 1, 2, 4};
  y_vec = {3, 3, 3, 4, 4, 4};

  // Ship
  x_vec = {2, 3, 2, 4, 3, 4};
  y_vec = {2, 2, 3, 3, 4, 4};

  // Aircraft carrier
  x_vec = {1, 2, 1, 4, 3, 4};
  y_vec = {2, 2, 3, 3, 4, 4};

  // Beehive
  x_vec = {2, 3, 1, 4, 2, 3};
  y_vec = {2, 2, 3, 3, 4, 4};

  // Barge
  x_vec = {2, 1, 3, 2, 4, 3};
  y_vec = {1, 2, 2, 3, 3, 4};

  // Python
  x_vec = {4, 5, 1, 3, 5, 1, 2};
  y_vec = {2, 2, 3, 3, 3, 4, 4};

  // Long Boat
  x_vec = {3, 2, 4, 3, 5, 4, 5};
  y_vec = {1, 2, 2, 3, 3, 4 ,4};

  // Eater, Fishhook
  x_vec = {2, 3, 2, 4, 4, 4, 5};
  y_vec = {1, 1, 2, 2, 3, 4, 4};

  // Loaf
  x_vec = {3, 2, 4, 2, 5, 3, 4};
  y_vec = {1, 2, 2, 3, 3, 4, 4};


  // Oszillatoren
  x_vec = {
    10, 11, 12, 13, 
    10, 14, 
    10, 
    1, 2, 11, 14, 
    0, 1, 3, 4, 
    1, 2, 3, 4, 8, 
    2, 3, 7, 9, 10, 19, 24, 25, 26, 
    6, 10, 19, 24, 26, 
    2, 3, 7, 9, 10, 19, 24, 25, 26, 
    1, 2, 3, 4, 8, 
    0, 1, 3, 4, 
    1, 2, 11, 14, 
    10, 
    10, 14, 
    10, 11, 12, 13
  };

  y_vec = {
    0, 0, 0, 0, 
    1, 1, 
    2, 
    3, 3, 3, 3, 
    4, 4, 4, 4, 
    5, 5, 5, 5, 5, 
    6, 6, 6, 6, 6, 6, 6, 6, 6, 
    7, 7, 7, 7, 7, 
    8, 8, 8, 8, 8, 8, 8, 8, 8,
    9, 9, 9, 9, 9, 
    10, 10, 10, 10, 
    11, 11, 11, 11, 
    12, 
    13, 13, 
    14, 14, 14, 14
  };


  // Clock 2, period 4
  /*
  x_vec = {
    6, 7, 
    6, 7, 
    4, 5, 6, 7, 
    0, 1, 3, 8, 
    0, 1, 3, 5, 6, 8, 
    3, 7, 8, 10, 11, 
    3, 8, 10, 11, 
    4, 5, 6, 7, 
    4, 5, 4, 5
  };

  y_vec = {
    0, 0, 
    1, 1, 
    3, 3, 3, 3, 
    4, 4, 4, 4, 
    5, 5, 5, 5, 5, 5, 
    6, 6, 6, 6, 6, 
    7, 7, 7, 7, 
    8, 8, 8, 8, 
    10, 10, 
    11, 11
  };
  */

  bool load = false;

  std::ifstream file(filename);
  if(file.is_open()){

  std::cout << "test" << std::endl;
    file.close();

    do{
      std::cout << "Do you want to load from save? [y][n] ";

      char c;
      std::cin >> c;

     
      if(c == 'y'){
        load = true;
        field = load_field_from_save(filename, block_count);
        break;
      }

      if(c == 'n'){
        load = false;
        break;
      }

      std::cout << "\n Please insert [y]es or [n]o \n\n";


    }while(true);

  }
  
  if(!load){
    // Lineares Feld
    // Beinhaltet nur 8 Bit Integer Werte:
    //   0 = block ist schwarz
    //   1 = block ist weiss
    field.assign(block_count * block_count, 0);

    initialize_field(field, x_vec, y_vec, block_count);

  }


  //int step = 0;


  SetTargetFPS(10);

  while(!WindowShouldClose()){

    BeginDrawing();

    ClearBackground(BLACK);

      // animation_field(field, step++);

      // Erst printen, dann verändern
      printField(field, block_size, block_count);

      rules(field, block_count);

    EndDrawing();

  }

  CloseWindow();

  std::cout << std::endl;

  if(save(filename, field, block_count) == -1){
    return -1;
  }

  return 0;
  
}

