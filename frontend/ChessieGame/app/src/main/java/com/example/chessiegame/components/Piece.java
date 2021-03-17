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

import com.example.chessiegame.components.Board;

public class Piece extends androidx.appcompat.widget.AppCompatImageView {

    public int col;
    public int row;
    public String name;
    public String color;
    public ImageView img;
    public boolean dragging;

    public Piece (Context context, int col, int row, String name) {
        super(context);
        this.col = col;
        this.row = row;
        this.name = name;
        this.dragging = false;

        this.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View v, MotionEvent event) {
                return onTouchPiece(v, event);
            }
        });

        this.setOnDragListener(new View.OnDragListener() {
            @Override
            public boolean onDrag(View v, DragEvent event) {
                onDragPiece(v, event);
                return true;
            }
        });
    }

    public Piece (Context context, int col, int row, String name, ImageView img) {
        super(context);
        this.col = col;
        this.row = row;
        this.name = name;
        this.img = img;
        this.dragging = false;

        this.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View v, MotionEvent event) {
                return onTouchPiece(v, event);
            }
        });

        this.setOnDragListener(new View.OnDragListener() {
            @Override
            public boolean onDrag(View v, DragEvent event) {
                onDragPiece(v, event);
                return true;
            }
        });

    }

    public void draw(Canvas canvas) {
        super.draw(canvas);
        this.img.getDrawable().draw(canvas);
    }

    public Piece(Context context, String name) {
        super(context);
        this.name = name;
    }

    public boolean onTouchPiece(View v, MotionEvent event) {
        if (event.getAction() == MotionEvent.ACTION_DOWN) {
            ClipData data = ClipData.newPlainText("", "");
            DragShadowBuilder shadowBuilder = new View.DragShadowBuilder(
                    img);
            img.startDrag(data, shadowBuilder, img, 0);
            img.setVisibility(View.INVISIBLE);
            return true;
        } else {
            return false;
        }
    }

    public boolean onDragPiece(View v, DragEvent event) {
        switch (event.getAction()) {
            case DragEvent.ACTION_DRAG_STARTED:
                break;
            case DragEvent.ACTION_DRAG_ENTERED:
                if (!dragging) {
                    dragging = true;
                    img.setVisibility(View.INVISIBLE);
                }

                int x_cord = (int) event.getX();
                int y_cord = (int) event.getY();
                break;
            case DragEvent.ACTION_DRAG_EXITED:
                x_cord = (int) event.getX();
                y_cord = (int) event.getY();
                break;
            case DragEvent.ACTION_DRAG_LOCATION:
                break;
            case DragEvent.ACTION_DRAG_ENDED:
                dragging = false;
                x_cord = (int) event.getX();
                y_cord = (int) event.getY();
                img.setVisibility(View.VISIBLE);
                break;
            case DragEvent.ACTION_DROP:
                x_cord = (int) event.getX();
                y_cord = (int) event.getY();
                View view = (View) event.getLocalState();
                ViewGroup owner = (ViewGroup) view.getParent();
                owner.removeView(view);

                //LinearLayout container = (LinearLayout) v;
                //container.addView(view);
                view.setVisibility(View.VISIBLE);
                break;
            default:
                break;
        }
        return true;
    }

    /*public Piece(String name){
        this.name = name;
    }*/

    public String getName(){ return name;}

}
