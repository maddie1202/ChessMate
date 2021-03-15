package com.example.chessiegame.components;

import android.graphics.drawable.Drawable;
import android.media.Image;


public class Piece {

    public int col;
    public int row;
    public String name;
    public String color;

    public Piece (int col, int row, String name){
        this.col = col;
        this.row = row;
        this.name = name;

    }

    public Piece(String name){
        this.name = name;
    }

    public String getName(){ return name;}

}
