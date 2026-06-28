# Hangman Word Guesser

A GUI Hangman game built in **C** using **[raylib 4.5](https://www.raylib.com/)**, rendered at 1920×1080. Guess letters (or the whole word) to save your hangman before you run out of tries, rack up points across rounds, and try to beat your high score.

[![Language](https://img.shields.io/badge/language-C-blue.svg)](https://en.wikipedia.org/wiki/C_(programming_language))
[![Library](https://img.shields.io/badge/raylib-4.5-orange.svg)](https://www.raylib.com/)
[![Platform](https://img.shields.io/badge/platform-Windows-lightgrey.svg)](https://www.microsoft.com/windows)

---

## Screenshots
![Start Screen](https://github.com/user-attachments/assets/64e81a88-409b-417f-9b8d-460aa49b5838)

![Gameplay](https://github.com/user-attachments/assets/ce177d56-46a4-40ac-9a02-20c634b1bc78)

![Game Over](https://github.com/user-attachments/assets/2bfd6331-6dd3-47d1-b9de-6c0644e5f63a)

---

## Features

- Full GUI flow - start screen, settings, gameplay, pause menu, and game over screen
- **Image-based logo** on the start screen, scaled with aspect-ratio preservation via `DrawTexturePro`
- Guess single letters **or** the entire word at once
- Free starter hint - 25% of the word's letters are revealed at the start of each round
- Lives tracked with heart icons, lost on incorrect guesses
- **Background music playlist** - up to 3 tracks loaded and played in sequence; advances automatically when a track ends
- **Sound effects** - distinct audio cues for correct guesses, wrong guesses, round wins, round losses, and UI clicks; a round-robin sound pool prevents audio cutoff on rapid inputs
- **Floating settings panel** - accessible from the start screen and the pause menu; includes separate sliders for main volume and music volume
- Pause menu with resume / restart / settings / main menu options
- Persistent high score **and** full game state, saved to disk between sessions (binary save file)
- Scoring system based on correct guesses, wrong guess penalties, and a speed/efficiency bonus
- Round-based replay loop - keep playing rounds until you lose, then start over

---

## Project Structure

```
hangman-word-guesser/
├── assets/
│   ├── sprite/
│   │   ├── heart.png            # Heart icon texture for the lives display
│   │   └── logo.png             # Title logo rendered on the start screen
│   └── audio/
│       └── (various .wav/.ogg)  # BGM tracks and UI sound effects
├── data/
│   ├── word-bank.txt            # Pool of possible secret words (one per line)
│   └── game-file.bin            # Binary save file (high score, game state, audio settings)
├── include/
│   └── raylib.h                 # raylib header
├── lib/
│   └── librarylib.a             # Pre-built static raylib library
├── main.c                       # Entry point, game loop, and screen state machine
├── game.c / game.h              # Core game logic — word loading, guess checking, scoring, save/load
├── drawing.c / drawing.h        # All rendering — screens, buttons, hangman, audio structs, UI elements
├── input.c / input.h            # Text input handling (typing, backspace, key repeat)
├── Makefile.win                 # Dev-C++ / TDM-GCC-64 Windows build file
└── hangman-word-guesser.dev     # Dev-C++ project file
```

---

## How to Play

1. Launch the game - you'll land on the **Start Screen** with the game logo.
2. Click **Play** to begin a round. A secret word is chosen at random from the word bank, with 25% of its letters already revealed as a free hint.
3. Type a single letter and press **Enter** to guess it, or type out the full word and press **Enter** to guess it outright.
4. Each wrong guess costs you a life (you have **6** tries total) and is added to your list of wrong letters.
5. **Win** by revealing the whole word before you run out of tries. **Lose** if you hit the wrong-guess limit.
6. Press **Esc** any time during play to pause - resume, restart, jump into settings, or return to the main menu.
7. After a round ends, choose to try again / play again or head back to the main menu. Win streaks increase your round count and score; a loss resets both.
8. Beat your high score - it's saved automatically and persists the next time you launch the game.

---

## Scoring

| Factor | Effect |
|---|---|
| Correct letters guessed | `+100` points each |
| Wrong guesses | `−30` points each |
| Total guesses taken | Fewer guesses = bigger bonus (`200 / number of guesses`) |

Final round score is never negative - it's floored at `0`.

---

## Audio System

### Background Music

The game supports a playlist of up to **3 BGM tracks** (`MAX_MUSIC_TRACKS`). Tracks are loaded at startup and played sequentially; when the current track finishes, the playlist automatically advances to the next one. Volume is controlled by the **Music Volume** slider in the settings panel.

### Sound Effects

Five distinct sound events are handled by the `UISounds` struct:

| Event | Trigger |
|---|---|
| `click` | Button hover / UI interaction |
| `correct` | Correct letter or word guess |
| `wrong` | Incorrect guess |
| `roundWin` | Completing a round successfully |
| `roundLoss` | Running out of lives |

To support rapid inputs without sounds cutting each other off, sound effects use a **round-robin pool** - multiple instances of each sound are kept in memory and cycled through so that overlapping playback works correctly.

### Settings Panel

The floating settings panel (`drawSettingsScreen`) is reachable from both the start screen and the pause menu. It exposes two independent sliders:

- **Main Volume** - controls the overall audio output level
- **Music Volume** - controls the BGM level independently of sound effects

Slider state is tracked in the `AudioSettings` struct. Drag detection correctly distinguishes between `IsMouseButtonPressed` (start of drag) and `IsMouseButtonDown` (held drag) via a `draggingSlider` field, preventing unwanted slider jumps on click.

Volume settings are saved to disk as part of the `SaveData` struct and restored on the next launch.

---

## Building from Source

This project was built with **Dev-C++ 6.3** and **TDM-GCC 64-bit**, and is set up for Windows.

### Requirements

- [Dev-C++](https://www.bloodshed.net/) (or any GCC-based toolchain on Windows)
- TDM-GCC / MinGW-w64 (64-bit)
- raylib **4.5** (already bundled - see `include/` and `lib/`)

### Option 1 - Dev-C++

1. Open `hangman-word-guesser.dev` in Dev-C++.
2. Build the project (**F9** or **Execute → Compile**).
3. Run the generated `hangman-word-guesser.exe`.

### Option 2 - Command Line (Makefile)

From the project root, using a MinGW/TDM-GCC environment:

```
mingw32-make -f Makefile.win
```

This compiles `main.c`, `game.c`, `drawing.c`, and `input.c` into object files and links them against raylib (`lib/librarylib.a`) along with `opengl32`, `gdi32`, `winmm`, and `msvcrt`, producing `hangman-word-guesser.exe`.

> **Note:** `Makefile.win` currently points to a hardcoded TDM-GCC install path for system includes and libraries. If you're building outside that exact toolchain location, update the `LIBS` and `INCS` paths in `Makefile.win` to match your local GCC installation.

### Running the Game

Make sure the executable is run from the **project root** (or that `assets/` and `data/` are alongside it) - the game loads assets and data using relative paths.

---

## Save System

Game state is persisted to `data/game-file.bin` using a binary `SaveData` struct. The following fields survive a session:

- High score
- Current round number and cumulative score
- Secret word and revealed/wrong letter state (so an in-progress game can be resumed)
- Main volume and music volume settings

The save file is read on startup via `loadSaveData()` and written after every significant game event via `writeSaveData()`.

---

## Customizing the Word Bank

Add or remove words by editing `data/word-bank.txt` - one word per line, plain text. Words are picked at random for each new round.

---

## Architecture Overview

The codebase is split into independent modules, each with a matching header:

- **`main.c`** - owns the screen state machine (`Screen` enum) and the main game loop; ties input, game logic, and drawing together each frame. Also manages the top-level `Texture2D` handles for `logo.png` and `heart.png`, and the `AudioSettings`, `UISounds`, and `BackgroundMusic` instances.
- **`game.c` / `game.h`** - all gameplay rules: loading a secret word, hint generation, guess validation, scoring, and save/restore of game state. Exposes a single shared `GameState state` used across the project.
- **`drawing.c` / `drawing.h`** - everything rendered to screen: buttons (via a reusable `Button` struct and factory functions), the hangman figure, and each screen layout (start, settings, playing, paused, game over). Also defines and implements the full audio subsystem - `UISounds`, `BackgroundMusic`, `AudioSettings`, and all related load/update/unload helpers.
- **`input.c` / `input.h`** - a self-contained text input module (`TextInput`) handling typed characters, backspace (including key-repeat-while-held), and input sanitization.

### Key Structs

| Struct | Defined in | Purpose |
|---|---|---|
| `GameState` | `game.h` | All live gameplay variables (word, guesses, score, round, etc.) |
| `SaveData` | `game.h` | Serialized subset of state written to / read from disk |
| `Button` | `drawing.h` | Reusable clickable button (bounds, label, hover state) |
| `AudioSettings` | `drawing.h` | Main and music volume floats + drag tracking |
| `UISounds` | `drawing.h` | Five `Sound` handles for UI and gameplay events |
| `BackgroundMusic` | `drawing.h` | `Music` track array, track count, current index, and volume |
| `TextInput` | `input.h` | Character buffer, cursor, and key-repeat timer |

---

## License

This project is open source - feel free to fork, modify, and learn from it.
