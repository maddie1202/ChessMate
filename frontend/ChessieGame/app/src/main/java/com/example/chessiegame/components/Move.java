package com.example.chessiegame.components;

public class Move {
    Piece piece;
    int dest_col;
    int dest_row;
    int init_col;
    int init_row;


    public Move (Piece piece, int icol, int irow, int dcol, int drow){
        this.piece = piece;
        this.dest_col = dcol;
        this.dest_row = drow;
        this.init_col = icol;
        this.init_row = irow;
    }



    public int getDest_col(){
        return dest_col;
    }


    public int getDest_row() {
        return dest_row;
    }

    public int getInit_col() {
        return init_col;
    }

    public int getInit_row() {
        return init_row;
    }

    public Piece getPiece() {
        return piece;
    }
}

