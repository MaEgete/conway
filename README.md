# Conway's Game of Life (C++ / Raylib)

A simple implementation of **Conway's Game of Life** written in modern C++ using the [raylib](https://www.raylib.com/) graphics library.

The simulation renders a square grid where each cell evolves according to Conway's classic rules.

---

## Preview

- Window size: 1200x1200
- Block size: 10px
- Adjustable FPS (currently 10)

Each cell:
- `0` → black (dead)
- `1` → white (alive)

---

## Rules of the Game

For every generation:

1. Any live cell with fewer than two live neighbours dies (underpopulation).
2. Any live cell with two or three live neighbours lives on.
3. Any live cell with more than three live neighbours dies (overpopulation).
4. Any dead cell with exactly three live neighbours becomes alive (reproduction).

---

## Internal Representation

The grid is stored as a **1D `std::vector<uint8_t>`**, even though it represents a 2D field.

Index mapping:

```cpp
index = y * block_count + x;
```

This ensures:

- contiguous memory layout
- good cache locality
- simple memory management



## Save & Load System

The program automatically saves the current state to:
```
field_save.txt
```

## On startup:

- If the file exists → you are asked whether to load it.

- Otherwise → a predefined pattern is initialized.

The save format is plain text:
```
0010100
0001110
```



## Built-in Patterns

The code contains several predefined patterns, including:

- Tub

- Boat

- Snake

- Ship

- Aircraft Carrier

- Beehive

- Barge

- Python

- Long Boat

- Loaf

- Oscillators

You can switch between patterns by modifying the x_vec and y_vec vectors in main().


## Requirements

- C++17 or newer

- raylib installed

- CMake (recommended)



## Build (CMake Example)
```
mkdir build
cd build
cmake ..
cmake --build .
```

Then run:
```
./your_executable_name
```

## Controls

- Close window → exit simulation

- On exit → field automatically saved


## Possible Improvements

- Mouse interaction (draw cells manually)

- Pause / Resume

- Adjustable simulation speed

- Camera2D support

- Infinite grid / wrapping edges

- Pattern loader (.rle format)

- Multithreading

- GPU acceleration


## About

This project is a learning project for:

- 2D graphics with raylib

- memory-efficient grid handling

- simulation design

- file I/O in C++

- rule-based systems

## License

- Free to use and modify.
