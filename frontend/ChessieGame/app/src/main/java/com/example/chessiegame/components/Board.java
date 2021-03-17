package com.example.chessiegame.components;

import android.annotation.SuppressLint;
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
import android.view.ViewGroup;
import android.widget.ImageView;
import android.widget.LinearLayout;
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

    public boolean dragging;
    public View dragView;

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
        drawBoard(canvas);
        drawPiece(canvas);
    }


    //Drawing the board
    public void drawBoard (Canvas canvas) {
        int width = getWidth();
        int height = getHeight();
        size = width/8;
        this.y0 = (height - size*8)/2;

        for (int i = 0; i < cols ; i ++) {
            for (int j = 0; j < rows; j++) {
                int xcoord = size * (7 - i);
                int ycoord = y0 + size * (7 - j);

                Piece p = new Piece(getContext(), i, j, "empty");

                board[i][j] = new Tile(getContext(), i, j);
                board[i][j].setPiece(p);
                board[i][j].setSquare(rect);
                board[i][j].setDrop(getContext());
                //board[i][j].drop.setOnDragListener(dragListener);

                rect.left = xcoord;
                rect.top = ycoord;
                rect.right = xcoord + size;
                rect.bottom = ycoord + size;
                board[i][j].draw(canvas);

            }

        }
    }

    public void drawPiece (Canvas canvas){

        int width = getWidth();
        int height = getHeight();
        size = width/8;
        this.y0 = (height - size*8)/2;

        for (int i = 0; i < cols ; i ++) {
            for (int j = 0; j < rows; j++) {
                int xcoord = size * (7 - i);
                int ycoord = y0 + size * (7 - j);
                //Setting the pieces
                piece_board[i][j] = new ImageView(getContext());
                //Pawn placement
                if(j==1){
                    Drawable wpawn = getResources().getDrawable(R.drawable.wpawn);
                    wpawn.setBounds(xcoord, ycoord, xcoord+size, ycoord+size);
                    piece_board[i][j].setImageDrawable(wpawn);
                    piece_board[i][j].getDrawable().draw(canvas);
                    Piece p = new Piece (getContext(), i, j, "wpawn");
                    board[i][j].setPiece(p);
                }
                else if(j==6){
                    Drawable bpawn = getResources().getDrawable(R.drawable.bpawn);
                    bpawn.setBounds(xcoord, ycoord, xcoord+size, ycoord+size);
                    piece_board[i][j].setImageDrawable(bpawn);
                    piece_board[i][j].getDrawable().draw(canvas);
                    Piece p = new Piece (getContext(), i, j, "bpawn");
                    board[i][j].setPiece(p);
                }
                //Rook
                else if(i == 7 && j == 7 || i == 0 && j == 7){
                    Drawable brook =  getResources().getDrawable(R.drawable.brook);
                    brook.setBounds(xcoord, ycoord, xcoord+size, ycoord+size);
                    piece_board[i][j].setImageDrawable(brook);
                    piece_board[i][j].getDrawable().draw(canvas);
                    Piece p = new Piece (getContext(), i, j, "brook");
                    board[i][j].setPiece(p);
                }
                else if(i == 7 && j == 0 || i == 0 && j == 0){
                    Drawable wrook =  getResources().getDrawable(R.drawable.wrook);
                    wrook.setBounds(xcoord, ycoord, xcoord+size, ycoord+size);
                    piece_board[i][j].setImageDrawable(wrook);
                    piece_board[i][j].getDrawable().draw(canvas);
                    Piece p = new Piece (getContext(), i, j, "wrook");
                    board[i][j].setPiece(p);
                }
                //Knights
                else if(i == 6 && j == 7 || i == 1 && j == 7){
                    Drawable bknight =  getResources().getDrawable(R.drawable.bknight);
                    bknight.setBounds(xcoord, ycoord, xcoord+size, ycoord+size);
                    piece_board[i][j].setImageDrawable(bknight);
                    piece_board[i][j].getDrawable().draw(canvas);
                    Piece p = new Piece (getContext(), i, j, "bknight");
                    board[i][j].setPiece(p);
                }
                else if(i == 6 && j == 0 || i == 1 && j == 0){
                    Drawable wknight =  getResources().getDrawable(R.drawable.wknight);
                    wknight.setBounds(xcoord, ycoord, xcoord+size, ycoord+size);
                    piece_board[i][j].setImageDrawable(wknight);
                    piece_board[i][j].getDrawable().draw(canvas);
                    Piece p = new Piece (getContext(), i, j, "wknight");
                    board[i][j].setPiece(p);
                }
                //Bishops
                else if(i == 5 && j == 7 || i == 2 && j == 7){
                    Drawable bbishop =  getResources().getDrawable(R.drawable.bbishop);
                    bbishop.setBounds(xcoord, ycoord, xcoord+size, ycoord+size);
                    piece_board[i][j].setImageDrawable(bbishop);
                    piece_board[i][j].getDrawable().draw(canvas);
                    Piece p = new Piece (getContext(), i, j, "bbishop");
                    board[i][j].setPiece(p);
                }
                else if(i == 5 && j == 0 || i == 2 && j == 0){
                    Drawable wbishop =  getResources().getDrawable(R.drawable.wbishop);
                    wbishop.setBounds(xcoord, ycoord, xcoord+size, ycoord+size);
                    piece_board[i][j].setImageDrawable(wbishop);
                    piece_board[i][j].getDrawable().draw(canvas);
                    Piece p = new Piece (getContext(), i, j, "wbishop");
                    board[i][j].setPiece(p);
                }
                //Queen
                else if(i == 4 && j == 7){
                    Drawable bqueen =  getResources().getDrawable(R.drawable.bqueen);
                    bqueen.setBounds(xcoord, ycoord, xcoord+size, ycoord+size);
                    piece_board[i][j].setImageDrawable(bqueen);
                    piece_board[i][j].getDrawable().draw(canvas);
                    Piece p = new Piece (getContext(), i, j, "bqueen");
                    board[i][j].setPiece(p);
                }
                else if(i == 4 && j == 0 ){
                    Drawable wqueen =  getResources().getDrawable(R.drawable.wqueen);
                    wqueen.setBounds(xcoord, ycoord, xcoord+size, ycoord+size);
                    piece_board[i][j].setImageDrawable(wqueen);
                    piece_board[i][j].getDrawable().draw(canvas);
                    Piece p = new Piece (getContext(), i, j, "wqueen");
                    board[i][j].setPiece(p);
                }
                //Queen
                else if(i == 3 && j == 7){
                    Drawable bking =  getResources().getDrawable(R.drawable.bking);
                    bking.setBounds(xcoord, ycoord, xcoord+size, ycoord+size);
                    piece_board[i][j].setImageDrawable(bking);
                    piece_board[i][j].getDrawable().draw(canvas);
                    Piece p = new Piece (getContext(), i, j, "bking");
                    board[i][j].setPiece(p);
                }
                else if(i == 3 && j == 0 ){
                    Drawable wking =  getResources().getDrawable(R.drawable.wking);
                    wking.setBounds(xcoord, ycoord, xcoord+size, ycoord+size);
                    piece_board[i][j].setImageDrawable(wking);
                    piece_board[i][j].getDrawable().draw(canvas);
                    Piece p = new Piece (getContext(), i, j, "wking");
                    board[i][j].setPiece(p);
                }

                piece_board[i][j].setOnTouchListener(new View.OnTouchListener() {
                    @SuppressLint("ClickableViewAccessibility")
                    @Override
                    public boolean onTouch(View v, MotionEvent event) {
                        return onTouchPiece(event, (ImageView) v);
                    }
                });

                piece_board[i][j].setOnDragListener(new View.OnDragListener() {
                    @Override
                    public boolean onDrag(View v, DragEvent event) {
                        onDragPiece(v, event, (ImageView) v);
                        return true;
                    }
                });

            }
        }

    }


    //Takes a list with all possible moves that a player can make, the move that player wants to make
    //Returns if the move is valid or not
    boolean isMoveValid(List<String> move_list, Move pmove){

        Piece p = mockMove(pmove);
        String presentBoard = boardToString();
        if (move_list.contains(presentBoard)){
            return true;
        }

        undoMove(pmove, p);
        return false;
    }

    //Get our present board and make it as a string
    String boardToString(){
        String boardMoves = "";

        for (int i = 0; i < cols ; i ++) {
            for (int j = 0; j < rows; j++) {
                boardMoves += board[i][j].getPiece().getName();
            }
        }
        return boardMoves;
    }

    //Apply a move to our board
    void applyMoveToBoard(Move move){
        Piece empty = new Piece (getContext(), "empty");
        board[move.getInit_col()][move.getInit_row()].setPiece(empty);
        board[move.getDest_col()][move.getDest_row()].setPiece(move.getPiece());
    }

    //Mock how the board would be if we applied the move
    //Returns the piece that was on the tile that we put our piece
    Piece mockMove(Move move){
        Piece empty = new Piece (getContext(), "empty");
        board[move.getInit_col()][move.getInit_row()].setPiece(empty);
        Piece ret = board[move.getDest_col()][move.getDest_row()].getPiece();
        board[move.getDest_col()][move.getDest_row()].setPiece(move.getPiece());
        return ret;
    }

    void undoMove(Move move, Piece p){
        //Set the intial piece back to its intial positions
        board[move.getInit_col()][move.getInit_row()].setPiece(move.getPiece());
        //Set the piece that used to be in the board back to its positions
        board[move.getDest_col()][move.getDest_row()].setPiece(p);
    }




    /*View.OnLongClickListener longClickListener = new View.OnLongClickListener(){
        @Override
        public boolean onLongClick(View view) {
            ClipData data = ClipData.newPlainText("", "");
            View.DragShadowBuilder myshadow = new View.DragShadowBuilder(view);
            view.startDrag(data, myshadow, view, 0);
            return true;
        }

    };


    View.OnDragListener dragListener = new View.OnDragListener(){
        @Override
        public boolean onDrag(View v, DragEvent event) {

            View view = (View) event.getLocalState();
            int dragEvent = event.getAction();
            switch (dragEvent){
                case DragEvent.ACTION_DRAG_ENTERED:
                    break;
                case DragEvent.ACTION_DRAG_EXITED:
                    break;
                case DragEvent.ACTION_DROP:

                    //Create Move

                    //Move pmove = new Move()

                    //Validate Player Move
                    //isMoveValid()
                    //Move Piece
                    view.animate()
                            .x(event.getX())
                            .y(event.getY())
                            .setDuration(700)
                            .start();

                    break;
                default:
                    break;
            }
            return true;
        }
    };*/

    public boolean onTouchPiece(MotionEvent event, ImageView img) {
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

    public void onDragPiece(View v, DragEvent event, ImageView img) {
        switch (event.getAction()) {
            case DragEvent.ACTION_DRAG_STARTED:
                break;
            case DragEvent.ACTION_DRAG_ENTERED:
                if (!dragging) {
                    dragging = true;
                    dragView = img;
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

                //int topBoard = (height - width) / 2;
                //int bottomBoard = height;
                //LinearLayout container = (LinearLayout) v;
                //container.addView(view);
                view.setVisibility(View.VISIBLE);
                break;
            default:
                break;
        }
    }


}
