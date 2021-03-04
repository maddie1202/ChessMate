package com.example.chessiegame.components;

import android.graphics.drawable.Drawable;
import android.media.Image;

public class Piece {

    public int col;
    public int row;
    public String name;

    public Piece (int x, int y, String name){
        this.col = y;
        this.row = x;
        this.name = name;

    }

}
