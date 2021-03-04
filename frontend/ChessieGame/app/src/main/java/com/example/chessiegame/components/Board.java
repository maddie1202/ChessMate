package com.example.chessiegame.components;

import android.content.Context;
import android.graphics.Rect;
import android.media.Image;
import android.view.View;

import com.example.chessiegame.R;
import com.example.chessiegame.components.Tile;
import java.util.List;
import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.widget.ImageView;
import android.widget.TextView;



public class Board extends View {
    public Tile[][] board;
    public TextView[][] piece_board;

    public Rect rect;
    public static int cols = 8;
    public static int rows= 8;
    public int size = 0;
    private int x0 = 0;
    private int y0 = 0;

    public Board(Context context) {
        super(context); // initialize a new chessboard
        this.board = new Tile[cols][rows];
        this.piece_board = new TextView[cols][rows];
        rect = new Rect();
        //setPieces();

    }

    @Override
    public void onDraw(Canvas canvas) {

        canvas.drawColor(Color.argb(100, 151, 182, 167));
        int width = getWidth();
        int height = getHeight();
        size = width/8;
        this.y0 = (height - size*8)/2;

        for (int i = 0; i < cols ; i ++){
            for (int j = 0; j < rows; j++){
                int xcoord =  size * (7 - i);
                int ycoord = y0 + size * (7 - j) ;

                board[i][j] = new Tile(i, j, 'p');

                board[i][j].setSquare(rect);

                rect.left = xcoord;
                rect.top = ycoord;
                rect.right = xcoord + size;
                rect.bottom = ycoord + size;
                board[i][j].draw(canvas);


            }
        }
       // board[0][0].
        //piece_board[0][0].setBackgroundResource(R.drawable.bbishop);



    }

    public void setPieces(){

       // piece_board[0][1] = new ImageView(getContext());
        //piece_board[0][1].setImageResource(R.drawable.bbishop);
        //piece_board[1][0] = new ImageView(getContext());
       // piece_board[1][0].setImageResource(R.drawable.bbishop);


    }

    public static Image getImage(String name){
        String piece = name + ".png";
        Image i = getImage(piece);
        return i;
    }



}
