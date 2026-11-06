# chess_engine

# Chess Game in C

A chess game I'm building in C with Raylib on Windows. with clean and simple readble code.

## What I did so far

It started with just a window and an empty board. Now it's a game you can actually play:

- Raylib window with an 8x8 board 
- All the pieces start in the right place
- I click a piece to select it and click a square to move it
- White and black play one after the other, and you can only pick your own pieces
- Every piece moves by its own rules (pawn, knight, bishop, rook, queen, king)
- Green dots show where the selected piece can go
- Check detection, and you can't make a move that leaves your own king in check
- Checkmate and stalemate end the game with a message on the screen
- Pawn promotion (a pawn that reaches the last row becomes a queen)
- En passant
- Castling

### How the movement logic works

Every move is just two numbers: how many rows the piece moved and how many columns. Each piece is a rule about those numbers. A knight moves 2 and 1, a bishop moves the same number of rows and columns. Rooks, bishops and queens also check that nothing is standing in the way.

To know if a move is really allowed, the game makes the move, checks if my king is attacked, and then puts everything back. If the king is attacked, the move is not allowed.

## Project structure

```
chess_engine/
├── src/
│   ├── main.c
│   ├── board.c / board.h     drawing the board, mouse input, game state
│   ├── pieces.c / pieces.h   setting up and drawing the pieces
│   └── moves.c / moves.h     movement rules, check, checkmate, special moves
├── assets/pieces/            piece images
├── stockfish/                stockfish.exe goes here (not in the repo)
├── bin/                      compiled game (not in the repo)
└── README.md
```

## How to build and run

need Raylib and gcc (MinGW). From the project folder:

```
gcc src/*.c -o bin/chess.exe -lraylib -lopengl32 -lgdi32 -lwinmm
./bin/chess.exe
```

## What I'm doing next

1. **Piece textures.** Replace the letters on the pieces with real images.
2. **Stockfish.** Let me play against Stockfish. The game starts `stockfish.exe` in the background and talks to it with text commands (the UCI protocol). I send the moves played so far and it answers with its best move. The idea is that the game keeps drawing while Stockfish thinks, so the window never freezes. Before that I need to clean up my code so that the human and the engine both move pieces with the same function.

## Future goal: my own engine

After Stockfish works, I want to write my own chess engine. I know it will be much weaker than Stockfish, but I want to understand how an engine works instead of only using one. My plan:

1. A move generator that makes a list of all valid moves
2. A simple evaluation: count the material (pawn 100, knight 320, bishop 330, rook 500, queen 900)
3. Minimax search, depth 3 or 4
4. Alpha-beta pruning to make it faster
5. Try captures first, so the search skips more moves
6. Small bonuses for good squares (like knights in the center)
7. Better checkmate scoring

