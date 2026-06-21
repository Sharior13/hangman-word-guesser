# Hangman Word Guesser

A GUI Hangman game built in **C** using **[raylib 4.5](https://www.raylib.com/)**, rendered at 1920x1080. Guess letters (or the whole word) to save your hangman before you run out of tries, rack up points across rounds, and try to beat your high score.

![Language](https://img.shields.io/badge/language-C-blue.svg)
![Library](https://img.shields.io/badge/raylib-4.5-orange.svg)
![Platform](https://img.shields.io/badge/platform-Windows-lightgrey.svg)

---

## Screenshots
![Start Screen](https://github.com/user-attachments/assets/23d76697-b675-4f33-936f-a89626e8cfc8)
![Gameplay](https://github.com/user-attachments/assets/ce177d56-46a4-40ac-9a02-20c634b1bc78)
![Game Over](https://github.com/user-attachments/assets/2bfd6331-6dd3-47d1-b9de-6c0644e5f63a)

---

## Features

- Full GUI flow - start screen, settings, gameplay, pause menu, and game over screen
- Guess single letters **or** the entire word at once
- Free starter hint - 25% of the word's letters are revealed at the start of each round
- Lives tracked with heart icons, lost on incorrect guesses
- Persistent high score, saved to disk between sessions
- Scoring system based on correct guesses, wrong guess penalties, and a speed/efficiency bonus
- Pause menu with resume / settings / main menu options
- Round-based replay loop - keep playing rounds until you lose, then start over

---

## Project Structure

```
hangman-word-guesser/
├── assets/
│   └── heart.png            # Heart icon texture used for the lives display
├── data/
│   ├── word-bank.txt        # Pool of possible secret words (one per line)
│   └── game-file.dat        # Stores the saved high score
├── include/
│   └── raylib.h              # raylib header
├── lib/
│   └── librarylib.a          # Pre-built static raylib library
├── main.c                   # Entry point, game loop, and screen state machine
├── game.c / game.h          # Core game logic - word loading, guess checking, scoring
├── drawing.c / drawing.h    # All rendering - screens, buttons, hangman, UI elements
├── input.c / input.h        # Text input handling (typing, backspace, key repeat)
├── Makefile.win             # Dev-C++ / TDM-GCC Windows build file
└── hangman-word-guesser.dev # Dev-C++ project file
```

---

## How to Play

1. Launch the game - you'll land on the **Start Screen**.
2. Click **Play** to begin a round. A secret word is chosen at random from the word bank, with 25% of its letters already revealed as a free hint.
3. Type a single letter and press **Enter** to guess it, or type out the full word and press **Enter** to guess it outright.
4. Each wrong guess costs you a life (you have **6** tries total) and is added to your list of wrong letters.
5. **Win** by revealing the whole word before you run out of tries. **Lose** if you hit the wrong-guess limit.
6. Press **Esc** any time during play to pause - resume, jump into settings, or return to the main menu.
7. After a round ends, choose to try again / play again or head back to the main menu. Win streaks increase your round count and score; a loss resets both.
8. Beat your high score - it's saved automatically and persists the next time you launch the game.

---

## Scoring

| Factor | Effect |
|---|---|
| Correct letters guessed | `+100` points each |
| Wrong guesses | `-30` points each |
| Total guesses taken | Fewer guesses = bigger bonus (`200 / number of guesses`) |

Final round score is never negative - it's floored at `0`.

---

## Building from Source

This project was built with **Dev-C++ 6.3** and **TDM-GCC 64-bit**, and is set up for Windows.

### Requirements
- [Dev-C++](https://www.bloodshed.net/) (or any GCC-based toolchain on Windows)
- TDM-GCC / MinGW-w64
- raylib **4.5** (already bundled - see `include/` and `lib/`)

### Option 1 - Dev-C++
1. Open `hangman-word-guesser.dev` in Dev-C++.
2. Build the project (**F9** or **Execute → Compile**).
3. Run the generated `hangman-word-guesser.exe`.

### Option 2 - Command Line (Makefile)
From the project root, using a MinGW/TDM-GCC environment:

```bash
mingw32-make -f Makefile.win
```

This compiles `main.c`, `game.c`, `drawing.c`, and `input.c` into object files and links them against raylib (`lib/librarylib.a`) along with `opengl32`, `gdi32`, `winmm`, and `msvcrt`, producing `hangman-word-guesser.exe`.

> **Note:** `Makefile.win` currently points to a hardcoded Embarcadero/TDM-GCC install path for system includes and libraries. If you're building outside that exact toolchain location, update the `LIBS` and `INCS` paths in `Makefile.win` to match your local GCC installation.

### Running the Game
Make sure the executable is run from the project root (or that `assets/` and `data/` are alongside it) - the game loads `assets/heart.png` and `data/word-bank.txt` using relative paths.

---

## Customizing the Word Bank

Add or remove words by editing `data/word-bank.txt` - one word per line, plain text. Words are picked at random for each new round.

---

## Architecture Overview

The codebase is split into independent modules, each with a matching header:

- **`main.c`** - owns the screen state machine (`Screen` enum) and the main game loop; ties input, game logic, and drawing together each frame.
- **`game.c` / `game.h`** - all gameplay rules: loading a secret word, hint generation, guess validation, scoring, and high score persistence. Exposes a single shared `GameState state` used across the project.
- **`drawing.c` / `drawing.h`** - everything rendered to screen: buttons (via a reusable `Button` struct/factory), the hangman figure, and each screen layout (start, settings, playing, paused, game over).
- **`input.c` / `input.h`** - a self-contained text input module (`TextInput`) handling typed characters, backspace (including key-repeat-while-held), and input sanitization.

---

## License

This project is open source - feel free to fork, modify, and learn from it.
