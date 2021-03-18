package com.example.chessiegame.components;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Rect;
import android.widget.LinearLayout;
import android.widget.TextView;

import androidx.cardview.widget.CardView;

public class Tile extends CardView {
    // represents an tile on the chessboard
    public Paint tileColor; // black = 0, white = 1
    public int highlighted; // false = 0, highlighted = 1
    public Piece piece; // null if no piece currently on tile
    public final int col ;
    public final int row;
    public Rect square;
    public TextView drop;

    public Tile(Context context, int x, int y) {
        super(context);
        this.row = x;
        this.col = y;
        this.tileColor = new Paint();
        //Determining the color of the tile
        if ((x + y) %2 == 0){
            tileColor.setColor(Color.WHITE);
        }
        else {
            tileColor.setColor(Color.argb(100, 151, 182, 181 ));
        }

    }

    @Override
    public void draw(Canvas canvas) {
        super.draw(canvas);
        //canvas.drawRect(square, tileColor);
    }

    //Will return true if x and y points are inside the tile
    public boolean isInTile(int x, int y){
        return square.contains(x, y);
    }

    public void setSquare(Rect square){
        this.square = square;
    }

    public void setDrop(Context context){this.drop = new TextView(context);}

    public int getRow(){
        return row;
    }

    public int getCol(){
        return col;
    }

    public Piece getPiece() {return piece;}

    public void setPiece(Piece p) {
        this.piece = p;
        this.addView(p);
    }

    public void removePiece(Piece p) {
        this.piece = null;
        this.removeView(p);
    }


    //public int setImage() {

        //square.
   // }


        // TODO: write more class functions

}
