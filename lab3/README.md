# Lab 3: Adventure Game

## Getting Started

Make sure you have C++ compiler, `make` and `cmake` installed, and run the following command in your terminal:

```bash
./run.sh
```

## How to Play

When you start the game, you should set the shape of the castle, including the width, height and level.

```
Welcome to the game!
Enter castle size (format: width height level): 
```

Then you will see the castle map and the prompt like this:

```
┌───┬───┬───┬───┬───┬───┐
│           │           │
├   ┼   ┼   ┼   ┼───┼───┤
│   │   │       │K E    │
├───┼───┼   ┼   ┼───┼   ┤
│ S     │ M │ P     │   │
├───┼   ┼   ┼   ┼   ┼   ┤
│       │   │   │       │
├───┼   ┼───┼   ┼   ┼   ┤
│     S         │   │   │
└───┴───┴───┴───┴───┴───┘
Steps: 0
Welcome to the lobby. There are 1 exits: east.
Enter your command:
```

### Characters

The characters are represented by single uppercase letters as follows:

- E: entrance
- K: knight
- P: princess
- M: monster
- S: staircase

### Commands

You can enter one of the following commands to control the game:

- `go {{direction}}`: move the knight to the adjacent room in the given direction
- `exit` or `quit`: quit the game
- `restart`: restart the game from the beginning

### Game rules

- You need to find the princess and escape the castle
- When you find the princess, a message will be displayed and the princess will follow you in the following steps.
- If you lead the princess to the exit, you win the game.
- If you run into the monster, you lose the game.
