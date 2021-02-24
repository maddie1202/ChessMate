package com.example.chessiegame.components;

public class Tile {
    // represents an tile on the chessboard
    public int tileColor; // black = 0, white = 1
    public int highlighted; // false = 0, highlighted = 1
    public char piece; // null if no piece currently on tile

    public Tile(int x, int y, char piece) {
        this.piece = piece;
        // rest of contructor depends on starting state
    }

    // TODO: write more class functions

}
