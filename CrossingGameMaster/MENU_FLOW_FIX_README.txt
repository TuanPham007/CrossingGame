CROSSING GAME - MENU FLOW FIX (SIMPLE PROTOTYPE)
================================================

This build fixes the New Game / Load Game routing problem by replacing the
old overlapping menu booleans with one explicit Screen state.

NEW FLOW
--------
MAIN MENU
  1. NEW GAME
  2. LOAD GAME
  3. EXIT

NEW GAME
  Step 1: Choose mode
    - CLASSIC MODE  = the 10-stage game based on the Excel design
    - INFINITE MODE = endless mode, difficulty increases every 200 points

  Step 2: Enter Name
  Step 3: Enter ID
  START GAME creates a save automatically in the first empty slot (1..10).

LOAD GAME
  - Shows exactly 10 slots.
  - Occupied slot shows Name, ID, Score, and mode.
  - Classic slot also shows current Stage/10.
  - Empty slot cannot be selected.
  - Clicking an occupied slot continues that saved game.

IMPORTANT MENU DESIGN CHANGE
----------------------------
Menu.cpp no longer loads or draws menu.png for the main menu.
The main menu is a deliberately simple prototype made from SFML RectangleShape
and Text. Therefore the visible button rectangle IS the click hitbox; there is
no image/hitbox mismatch.

SFML itself is still required because the entire game window/input/render loop
uses SFML. Only the decorative menu image/texture dependency was removed.

FILES CHANGED IN THIS FIX
-------------------------
1. Menu.h
   - Replaced overlapping booleans with enum class Screen:
     Main / NewChooseMode / NewEnterName / NewEnterId / Load.

2. Menu.cpp
   - Rebuilt main menu as simple rectangles/text.
   - New Game now chooses Classic/Infinite BEFORE Name/ID.
   - New game automatically creates first available slot.
   - Load Game exclusively shows 10 save slots.
   - Load slot displays Name + ID + Score + Mode + Stage when Classic.
   - menu.png is no longer required by Menu::load().

3. main.cpp
   - HUD display names changed:
       ENDLESS -> INFINITE
       LEVEL   -> CLASSIC
   - Existing internal mechanics are retained.

4. SaveData.h
   - Comment updated to clarify save mapping:
       0 = Infinite
       1 = Classic
   - Mapping remains compatible with the previous build.

TEST PERFORMED
--------------
CSAVEGAME V4 save/load was compiled and tested separately with C++20.
The test successfully preserved Name, ID, game mode, stage, score and lives.

On your Windows machine, open the existing Visual Studio project and use:
Build -> Rebuild Solution
with the same SFML 3.1.0 setup used by the project.
