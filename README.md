# Conway's Game of Life (C++ / raylib)

A simple implementation of **Conway's Game of Life** written in modern C++ using the raylib graphics library.

This version includes:

- A state-based UI (Start screen, Draw mode, Running simulation, Settings)
- Mouse-based pattern drawing
- Save & Load system
- Rainbow rendering mode
- Basic settings menu

---

## Preview / Configuration

- Window size: `1200 x 1200`
- Block size: `10px`
- Grid size: `block_count = width / block_size`
- Target FPS: `30`

Each cell:
- `0` → black (dead)
- `1` → white or colored (alive)

---

## Game States

The program uses a finite state machine:

```cpp
enum class GameState {
  StartScreen,
  DrawScreen,
  Running,
  End,
  Settings
};
```

### StartScreen
- Displays:
  - Title
  - "Load Game? yes / no"
- Yes → tries to load `field_save.txt`
- No → switches to DrawScreen with empty field
- If file cannot be opened → error message shown on screen

### DrawScreen
- You can manually draw cells with the mouse.
- Internally stores positions in:
  - `std::vector<int> x_vec`
  - `std::vector<int> y_vec`
- Each frame:
  - Field is cleared
  - All stored coordinates are applied again
  - Field is rendered

### Running
- Field is rendered (rainbow mode enabled)
- Conway rules are applied each frame
- SPACE → opens Settings

### Settings
- Field remains visible in background
- Buttons:
  - Draw new → clears drawing vectors and returns to DrawScreen
  - Exit → exits program
- SPACE → return to Running

### End
- Exits main loop
- Field is saved automatically

---

## Controls

### StartScreen
- Left click on Yes → load save
- Left click on No → go to DrawScreen

### DrawScreen
- Left mouse button (hold) → add cells
- Right mouse button (hold) → remove cells
- Middle mouse button (press) → start simulation

### Running
- SPACE → open Settings

### Settings
- Left click:
  - Draw new → return to DrawScreen
  - Exit → quit
- SPACE → return to Running

---

## Rules of the Game

For every generation:

1. Live cell with < 2 neighbours → dies
2. Live cell with 2 or 3 neighbours → survives
3. Live cell with > 3 neighbours → dies
4. Dead cell with exactly 3 neighbours → becomes alive

Implementation details:

- `countNeighbours(x, y, field, block_count)`
- No wrapping (edges treated as dead)

---

## Internal Representation

The grid is stored as a 1D vector:

```cpp
std::vector<uint8_t> field;
```

Index mapping:

```cpp
index = y * block_count + x;
```

Advantages:

- Contiguous memory
- Cache-friendly
- Simple memory management

---

## Drawing System

In DrawScreen:

- Clicking does not permanently modify the field
- Coordinates are stored in `x_vec` and `y_vec`
- Every frame:
  1. Field is cleared
  2. All stored coordinates are applied
  3. Field is rendered

Right-click removes matching coordinates from both vectors.

When returning from Settings with "Draw new":
- `redraw = true`
- Both vectors are cleared

---

## Rendering

### Normal Mode
- Dead → BLACK
- Alive → WHITE

### Rainbow Mode
While Running and in Settings:
- Alive cells cycle through:
  - PURPLE
  - BLUE
  - GREEN
  - YELLOW
  - ORANGE
  - RED

Color index updates automatically every frame.

---

## Save & Load System

File used:

```
field_save.txt
```

### Save Format

Plain text grid:

```
0010100
0001110
```

Each row represents one grid line.

### On Startup

If "Yes" is clicked:

- Program attempts to open file
- If successful → loads grid
- If not → error shown

### On Exit

After window closes:

```cpp
save(filename, field, block_count);
```

Current field is written to disk.

---

## Build (CMake Example)

```bash
mkdir build
cd build
cmake ..
cmake --build .
```

Run:

```bash
./your_executable_name
```

---

## Requirements

- C++17 or newer
- raylib installed
- CMake recommended

---

## Possible Improvements

- Prevent duplicate coordinates in Draw mode
- Pause / Resume in Running state
- Adjustable simulation speed
- Wrap-around edges (toroidal grid)
- Pattern loader (.rle format)
- Camera2D zoom & pan
- Stable state detection
- UI improvements
- Multiple save slots

---

## Learning Goals

This project demonstrates:

- 2D rendering with raylib
- Grid-based simulations
- Finite state machines
- Mouse and keyboard input handling
- File I/O in C++
- Memory-efficient data layout

---

## License

Free to use and modify.
