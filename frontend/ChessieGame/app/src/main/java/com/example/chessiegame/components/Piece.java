package com.example.chessiegame.components;

import android.annotation.SuppressLint;
import android.content.ClipData;
import android.content.Context;
import android.graphics.Canvas;
import android.graphics.drawable.Drawable;
import android.media.Image;
import android.view.MotionEvent;
import android.view.ViewGroup;
import android.widget.ImageView;
import android.view.View;
import android.view.DragEvent;
import android.widget.Toast;

//import com.example.chessiegame.components.Board;

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

    /*public void replaceID(int newID) {
        this.id = newID;
    }*/

    public void draw(Canvas canvas) {
        super.draw(canvas);
        this.getDrawable().draw(canvas);
    }

    public String getName(){ return name;}

}
