# MINES-TUI
![GitHub](https://img.shields.io/github/license/Judro/mines)
![GitHub release (latest SemVer)](https://img.shields.io/github/v/release/Judro/mines)
[![build](https://github.com/Judro/mines/actions/workflows/c-cpp.yml/badge.svg)](https://github.com/Judro/mines/actions/workflows/c-cpp.yml)
<div>
<img src="https://github.com/Judro/mines/blob/main/img/game.png" alt="game" width="250px">
<img src="https://github.com/Judro/mines/blob/main/img/game_over.png" alt="game over" width="250px">
<img src="https://github.com/Judro/mines/blob/main/img/highscores.png" alt="highscores" width="250px">
</div>

## Installation
### Ubuntu- / Debian-based Systems
install the deb package: [x86-64/amd64](https://github.com/Judro/mines/releases/download/v1.0.0/mines_1.0.0_amd64.deb)
[arm64](https://github.com/Judro/mines/releases/download/v1.0.0/mines_1.0.0_arm64.deb)


### Arch-based Systems
Install the package from the AUR using an AUR helper like yay:

`yay -S mines-tui`

## Run
`mines-tui`

### Get help
`man mines-tui`

## Controls
- Movement: `hjkl or ←↓↑→`
- Place flag: `f`
- Open tile: `s`
- Display highscores: `b`
- Emphasize neighbouring cells `e`
- Open the game manual: `m`
- Quit: `q`

(please read `man mines-tui`)

## Build from source
1. install dependencies
2. `make`
3. `make install`

(If linking fails, consider replacing `-lncursesw` with `-lncurses` in the `Makefile`)

### Dependencies
- libncurses-devel

### Contributing
This project was my playground to learn C, so don't expect the cleanest code or the most optimized solutions. I might have done things differently if I had to redo it! If you're keen to chip in, feel free to send over your ideas or pull requests
