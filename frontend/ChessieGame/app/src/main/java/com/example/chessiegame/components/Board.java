package com.example.chessiegame.components;

import android.content.Context;
import android.graphics.Rect;
import android.view.View;

import com.example.chessiegame.components.Tile;
import java.util.List;
import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;


public class Board extends View {
    public Tile[][] board;

    public Rect rect;
    public static int cols = 8;
    public static int rows= 8;
    public int size = 0;

    Tile tile;
    private int x0 = 0;
    private int y0 = 0;

    public Board(Context context) {
        super(context); // initialize a new chessboard
        this.board = new Tile[cols][rows];

        rect = new Rect();

    }

    @Override
    public void onDraw(Canvas canvas) {

        canvas.drawColor(Color.GREEN);
        int width = getWidth()-1;
        int height = getHeight()-1;
        size = Math.min(width/8, height/8);

        this.x0 = (width - size *8)/2;
        this.y0 = (height - size*8)/2;

        for (int i = 0; i < cols ; i ++){
            for (int j = 0; j < rows; j++){
                int xcoord = x0 + size * 7 - i;
                int ycoord = y0 + size * 7 - j ;

                board[i][j].setSquare(rect);

                rect.left = xcoord;
                rect.top = ycoord;
                rect.right = xcoord + size;
                rect.bottom = ycoord + size;
                board[i][j].draw(canvas);


            }
        }



    }

}
