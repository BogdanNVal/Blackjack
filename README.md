# Blackjack — Console Card Game (C++)

A console-based Blackjack game written in C++, with custom classes for each
game concept (card, deck, linked list for a hand, player) and simple player
persistence between sessions.

## Screenshots

![Main menu](docs/screenshots/meniu.png)
![A round in progress](docs/screenshots/runda.png)
![Round result](docs/screenshots/rezultat.png)

## Features

- **Main menu**: new player or continue with an already saved player.
- **Standard Blackjack rules**: Hit, Stand, Double (double the bet + exactly
  one additional card); aces count as 1 or 11, calculated automatically for
  the optimal score; the dealer automatically draws while their score is
  below 17.
- **Correct outcome determination**: bust, natural blackjack (push), score
  higher/equal/lower than the dealer's.
- **Continue playing**: after each round you can keep playing with the same
  player, keeping your accumulated money, without restarting the application.
- **Save/load player**: the name and money are saved automatically on exit
  to a text file (`jucatori.txt`), and can be reloaded in the next session.
- **Score displayed right-aligned on screen**, for each hand; while the
  dealer still has a hidden card, their score is shown as "value+?".
- **Card-dealing "animation"**: cards appear one at a time, with a short
  pause between them, instead of all appearing at once.
- **Console colors**: Hearts/Diamonds cards are shown in red, and result
  messages are colored (green for a win, red for a loss/bust, yellow for a
  tie).
- **Graphical card symbols**: Clubs/Hearts/Spades/Diamonds are displayed as
  Unicode symbols (♣ ♥ ♠ ♦), not as letters (C/H/S/D).

## Tech stack

- C++17, no external dependencies — standard library only
  (`<iostream>`, `<fstream>`, `<random>`, `<thread>`, `<chrono>`, `<map>`)
- Visual Studio 2022 project (`.sln` / `.vcxproj`), x64/Win32 platform
- Custom classes for each concept: `Carte` (Card), `Lista` (List — a hand-
  written linked list used for each player's hand), `Pachet` (Deck),
  `Jucator` (Player)
- Deck shuffling: `std::mt19937` + `std::random_device`
- Player persistence: plain text file, read/written with `std::fstream` and
  `std::map` for lookup by name
- Pauses between cards: `std::this_thread::sleep_for`
- Console colors: Windows Console API (`SetConsoleTextAttribute`)

## Running locally

1. Open `Blackjack.sln` in Visual Studio 2022.
2. If a build error related to the **v143** toolset appears, install the
   missing component from the Visual Studio Installer:
   Modify → Individual components → **MSVC v143 - VS 2022 C++ x64/x86
   build tools**.
3. Build & Run (F5). The game starts directly in the console.

The save file `jucatori.txt` is created automatically, next to the
executable, the first time a player is saved — it doesn't need to be
created manually.

## Technical decisions worth noting

- The `Jucator` class correctly implements the Rule of Three (copy
  constructor + `operator=`), because it dynamically allocates memory for
  the name (`new char[]`) and contains a `Lista` object with its own
  allocation — copying a `Jucator` (for example, when loading a saved
  player) is safe, with no double-free.
- Deck shuffling uses an `std::mt19937` generator seeded once from
  `std::random_device`, not `rand()`/`srand(time(0))` — a time-based seed
  with 1-second resolution would produce the same shuffle if the game were
  started multiple times within the same second.
- Player persistence is an unencrypted text file, sufficient for a project
  of this kind, but not suitable for sensitive data in a real-world
  context.
- **Split is not implemented**: it would require `Jucator` to be able to
  hold multiple hands at once (instead of a single `Lista carti` + a single
  `suma_pariata`), plus adapting the game loop in `Joc.cpp` to iterate over
  the active hands — a targeted restructuring, not just filling in an
  empty function.
- **Card symbols (♣ ♥ ♠ ♦) are Unicode, written as raw UTF-8 bytes**
  directly in the code (`\xE2\x99\xA3` etc.), not as OEM codepage letters —
  they display correctly in any console (classic cmd, Windows Terminal),
  regardless of the font used. The console's output codepage is explicitly
  set to UTF-8 (`SetConsoleOutputCP(CP_UTF8)`) in `Source.cpp`. Because a
  UTF-8 symbol takes up 3 bytes but displays as a single character, the
  column alignment of hands in `Joc.cpp` counts Unicode characters, not raw
  bytes (the `lungimeUtf8` function), otherwise the padding would come out
  wrong.

## Project structure

```
Blackjack/
  Blackjack.sln                 -> Visual Studio solution
  Blackjack/
    Source.cpp                  -> main() -> meniuPrincipal()
    Joc.h / Joc.cpp              -> menu, game loop, round rules
    Jucator.h / Jucator.cpp      -> Jucator class (money, hand, score, actions)
    Carte.h / Carte.cpp          -> Carte class (value + symbol)
    Lista.h / Lista.cpp          -> linked list, used for the hand
    Pachet.h / Pachet.cpp        -> the 52-card deck + shuffling
    Salvare.h / Salvare.cpp      -> player save/load (jucatori.txt)
    Blackjack.vcxproj(.filters) -> Visual Studio project configuration
```
