package com.example.chessiegame.components;

import android.content.ClipData;
import android.content.Context;
import android.graphics.Rect;
import android.graphics.drawable.Drawable;
import android.media.Image;
import android.os.Build;
import android.view.DragEvent;
import android.view.MotionEvent;
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

import androidx.annotation.RequiresApi;


public class Board extends View {
    public Tile[][] board;
    public ImageView[][] piece_board;
    public Rect rect;
    public static int cols = 8;
    public static int rows= 8;
    public int size = 0;
    private int x0 = 0;
    private int y0 = 0;

    public Board(Context context) {
        super(context); // initialize a new chessboard
        this.board = new Tile[cols][rows];
        this.piece_board = new ImageView[cols][rows];
        rect = new Rect();
        //setPieces();

    }

    @RequiresApi(api = Build.VERSION_CODES.LOLLIPOP)
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

                //Setting the pieces
                piece_board[i][j] = new ImageView(getContext());
                //Pawn placement
                if(j==1){
                    Drawable wpawn = getResources().getDrawable(R.drawable.wpawn);
                    wpawn.setBounds(xcoord, ycoord, xcoord+size, ycoord+size);
                    piece_board[i][j].setImageDrawable(wpawn);
                    piece_board[i][j].getDrawable().draw(canvas);
                }
                else if(j==6){
                    Drawable bpawn = getResources().getDrawable(R.drawable.bpawn);
                    bpawn.setBounds(xcoord, ycoord, xcoord+size, ycoord+size);
                    piece_board[i][j].setImageDrawable(bpawn);
                    piece_board[i][j].getDrawable().draw(canvas);
                }
                //Rook
                else if(i == 7 && j == 7 || i == 0 && j == 7){
                    Drawable brook =  getResources().getDrawable(R.drawable.brook);
                    brook.setBounds(xcoord, ycoord, xcoord+size, ycoord+size);
                    piece_board[i][j].setImageDrawable(brook);
                    piece_board[i][j].getDrawable().draw(canvas);
                }
                else if(i == 7 && j == 0 || i == 0 && j == 0){
                    Drawable wrook =  getResources().getDrawable(R.drawable.wrook);
                    wrook.setBounds(xcoord, ycoord, xcoord+size, ycoord+size);
                    piece_board[i][j].setImageDrawable(wrook);
                    piece_board[i][j].getDrawable().draw(canvas);
                }
                //Knights
                else if(i == 6 && j == 7 || i == 1 && j == 7){
                    Drawable bknight =  getResources().getDrawable(R.drawable.bknight);
                    bknight.setBounds(xcoord, ycoord, xcoord+size, ycoord+size);
                    piece_board[i][j].setImageDrawable(bknight);
                    piece_board[i][j].getDrawable().draw(canvas);
                }
                else if(i == 6 && j == 0 || i == 1 && j == 0){
                    Drawable wknight =  getResources().getDrawable(R.drawable.wknight);
                    wknight.setBounds(xcoord, ycoord, xcoord+size, ycoord+size);
                    piece_board[i][j].setImageDrawable(wknight);
                    piece_board[i][j].getDrawable().draw(canvas);
                }
                //Bishops
                else if(i == 5 && j == 7 || i == 2 && j == 7){
                    Drawable bbishop =  getResources().getDrawable(R.drawable.bbishop);
                    bbishop.setBounds(xcoord, ycoord, xcoord+size, ycoord+size);
                    piece_board[i][j].setImageDrawable(bbishop);
                    piece_board[i][j].getDrawable().draw(canvas);
                }
                else if(i == 5 && j == 0 || i == 2 && j == 0){
                    Drawable wbishop =  getResources().getDrawable(R.drawable.wbishop);
                    wbishop.setBounds(xcoord, ycoord, xcoord+size, ycoord+size);
                    piece_board[i][j].setImageDrawable(wbishop);
                    piece_board[i][j].getDrawable().draw(canvas);
                }
                //Queen
                else if(i == 4 && j == 7){
                    Drawable bqueen =  getResources().getDrawable(R.drawable.bqueen);
                    bqueen.setBounds(xcoord, ycoord, xcoord+size, ycoord+size);
                    piece_board[i][j].setImageDrawable(bqueen);
                    piece_board[i][j].getDrawable().draw(canvas);
                }
                else if(i == 4 && j == 0 ){
                    Drawable wqueen =  getResources().getDrawable(R.drawable.wqueen);
                    wqueen.setBounds(xcoord, ycoord, xcoord+size, ycoord+size);
                    piece_board[i][j].setImageDrawable(wqueen);
                    piece_board[i][j].getDrawable().draw(canvas);
                }
                //Queen
                else if(i == 3 && j == 7){
                    Drawable bking =  getResources().getDrawable(R.drawable.bking);
                    bking.setBounds(xcoord, ycoord, xcoord+size, ycoord+size);
                    piece_board[i][j].setImageDrawable(bking);
                    piece_board[i][j].getDrawable().draw(canvas);
                }
                else if(i == 3 && j == 0 ){
                    Drawable wking =  getResources().getDrawable(R.drawable.wking);
                    wking.setBounds(xcoord, ycoord, xcoord+size, ycoord+size);
                    piece_board[i][j].setImageDrawable(wking);
                    piece_board[i][j].getDrawable().draw(canvas);
                }

                /*
                if(i > 3){
                    piece_board[i][j].setOnTouchListener(new MyTouch());
                } else{
                    piece_board[i][j].setOnDragListener(new MyDragListener());
                }

                 */

            }
        }


    }


    public class MyTouch implements View.OnTouchListener{
        @Override
        public boolean onTouch(View view, MotionEvent motionEvent) {
            if (motionEvent.getAction() == MotionEvent.ACTION_DOWN) {
                ClipData data = ClipData.newPlainText("", "");
                View.DragShadowBuilder shadowBuilder = new View.DragShadowBuilder(
                        view);
                view.startDrag(data, shadowBuilder, view, 0);
                view.setVisibility(View.INVISIBLE);
                return true;
            } else {
                return false;
            }
        }
    }

    public class MyDragListener implements View.OnDragListener {
        @Override
        public boolean onDrag(View v, DragEvent event) {
            int action = event.getAction();
            switch (action) {
                case DragEvent.ACTION_DRAG_STARTED:
                    // do nothing
                    break;
                case DragEvent.ACTION_DRAG_ENTERED:
                    break;
                case DragEvent.ACTION_DRAG_EXITED:
                    break;
                case DragEvent.ACTION_DROP:
                    // Dropped, reassign View to ViewGroup
                    View dragview = (View) event.getLocalState();
                    dragview.setVisibility(View.VISIBLE);
                    break;
                default:
                    break;
            }
            return true;
        }
    }


}
