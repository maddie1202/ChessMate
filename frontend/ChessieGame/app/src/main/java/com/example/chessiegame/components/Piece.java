package com.example.chessiegame.components;

import android.content.Context;
import android.graphics.Canvas;

public class Piece extends androidx.appcompat.widget.AppCompatImageView {

    public int col;
    public int row;
    public String name;
    public String color;
    public int id;

    public Piece (Context context, int row, int col, String name, int id) {
        super(context);
        this.col = col;
        this.row = row;
        this.name = name;
        this.id = id;
    }

    public void draw(Canvas canvas) {
        super.draw(canvas);
        this.getDrawable().draw(canvas);
    }

    public String getName(){ return name;}

    public void updateCoordinates(int row, int col) {
        this.col = col;
        this.row = row;
    }

}
