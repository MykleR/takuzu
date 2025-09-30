<div align="center">

# Takuzu

<img src="https://github.com/MykleR/takuzu/blob/main/images/screenshot.png" width="500" />

</div>


> [!NOTE]
> This project is an implementation of the [Takuzu](https://en.wikipedia.org/wiki/Takuzu) puzzle game in C. The objective is to fill a grid with tiles while adhering to specific rules.

## ✨ Features

> - 🎮 **Multiple Implementations**:
>    - Terminal: Classic text-based interface.
>    - WebGL: Compiled for the web, play in a browser.
>    - SDL2: Classic graphical version.
> - 🧩 **Solver**: Integrated solver to assist you.
> - 🔄 **Undo/Redo**: To facilitate and improve playability
> - ⚔️ **Difficulties**: Easy, medium, and hard. Increases the size of the grid.

## 📚 Game Rules

> The rules of the game are simple. You must fill the entire grid with either blue or black according to the following constraints:
> 1.  **No More Than Two Alike**: There cannot be more than two identical numbers next to each other in a row or column.
> 2.  **Equal Numbers**: Each row and each column must have an equal number of 0s and 1s.
> 3.  **No Identical Rows or Columns**: No two rows or two columns can be identical.

## 🚀 Getting Started

### Prerequisites

> - C Compiler (like `gcc` or `Clang`)
> - `CMake`
> - `GNU Make`
> - `SDL2` library (for the graphical version)

### Installation

```bash
git clone https://github.com/MykleR/takuzu.git
cd takuzu

mkdir build && cd build
cmake ..
make

./game_text
./game_sdl
open web/index.html
```

> [!TIP]
> Hosted web version that you can try out [here](https://michael-rouves.emi.u-bordeaux.fr/takuzu/web/)

## ✍️ Authors

- **Michael ROUVES**
- **Hugo BERNARD**
- **Corentin TINNES**
