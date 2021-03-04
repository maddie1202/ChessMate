package com.example.chessiegame.components;

import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Rect;

public class Tile {
    // represents an tile on the chessboard
    public Paint tileColor; // black = 0, white = 1
    public int highlighted; // false = 0, highlighted = 1
    public char piece; // null if no piece currently on tile
    public final int col ;
    public final int row;
    public Rect square;

    public Tile(int x, int y, char piece) {
        this.piece = piece;
        this.col = y;
        this.row = x;
        this.tileColor = new Paint();
        //Determining the color of the tile
        if ((x + y) %2 == 0){
            tileColor.setColor(Color.BLACK);
        }
        else {
            tileColor.setColor(Color.WHITE);
        }

    }


    public void draw(Canvas canvas) {
        canvas.drawRect(square, tileColor);
    }

    //Will return true if x and y points are inside the tile
    public boolean isInTile(int x, int y){
        return square.contains(x, y);
    }

    public void setSquare(Rect square){
        this.square = square;
    }

    public int getRow(){
        return col;
    }

    public int getCol(){
        return row;
    }


        // TODO: write more class functions

}
