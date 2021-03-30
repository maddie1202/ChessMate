package com.example.chessiegame;

import androidx.appcompat.app.AppCompatActivity;
import androidx.constraintlayout.widget.ConstraintLayout;

import android.content.Context;
import android.content.Intent;
import android.content.res.Resources;
import android.graphics.Color;
import android.os.Bundle;
import android.os.Handler;
import android.util.Log;
import android.view.View;
import android.widget.ImageView;
import android.widget.TableLayout;
import android.widget.TableRow;

import com.example.chessiegame.components.Piece;
import com.example.chessiegame.components.Tile;
import com.example.chessiegame.components.BoardMap;

import java.util.ArrayList;
import java.util.concurrent.TimeUnit;
import java.util.Arrays;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class ReplayPastGamesScreen extends AppCompatActivity {

    ImageView backArrow;
    public TableLayout chessBoard;
    public Tile[][] tiles;
    public int gameID;
    private final int rows = 8;
    private final int cols = 8;
    private BoardMap imageMap;
    private Context context;

    private ArrayList<Integer[][]> boards;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_replay_past_games_screen);

        backArrow = findViewById(R.id.arrow_back);
        gameID = getIntent().getIntExtra("gameID", 0);
        backArrow.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent(ReplayPastGamesScreen.this, PastGamesScreen.class);
                startActivity(intent);
            }
        });

        context = this;
        imageMap = new BoardMap(this);
        tiles = new Tile[rows][cols];
        chessBoard = findViewById(R.id.chess);
        boards = new ArrayList<Integer[][]>();
        mockBoards();

        initChessboard();
        mockOneMove();
        //replayGame();
    }

    public void mockOneMove() {
        Handler h = new Handler();
        Runnable r = new Runnable() {
            public void run() {
                // move pawn 1 one space up
                Piece p = tiles[6][0].getPiece();
                tiles[6][0].removePiece(p);

                Piece pc = new Piece(context, 6, 0, "bpawn", (char) -1);
                tiles[5][0].setPiece(pc);
                pc.setImageResource(imageMap.get(-1));
            }
        };
        h.postDelayed(r,2000);
    }

    public void mockBoards() {
        boards.add(new Integer[][]{
                {-9, -19, -29, -48, -39, -30, -20, -10},
                {-1, -2, -3, -4, -5, -6, -7, -8},
                {0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0},
                {1, 2, 3, 4, 5, 6, 7, 8},
                {9, 19, 29, 48, 39, 30, 20, 10}
        });
        boards.add(new Integer[][]{
                {-9, -19, -29, -48, -39, -30, -20, -10},
                {-1, -2, -3, -4, -5, -6, -7, -8},
                {0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 5, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0},
                {1, 2, 3, 4, 0, 6, 7, 8},
                {9, 19, 29, 48, 39, 30, 20, 10}
        });
        boards.add(new Integer[][]{
                {-9, -19, -29, -48, -39, -30, -20, -10},
                {-1, -2, 0, -4, -5, -6, -7, -8},
                {0, 0, -3, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 5, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0},
                {1, 2, 3, 4, 0, 6, 7, 8},
                {9, 19, 29, 48, 39, 30, 20, 10}
        });
        boards.add(new Integer[][]{
                {-9, -19, -29, -48, -39, -30, -20, -10},
                {-1, -2, 0, -4, -5, -6, -7, -8},
                {0, 0, -3, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 5, 0, 0, 8},
                {0, 0, 0, 0, 0, 0, 0, 0},
                {1, 2, 3, 4, 0, 6, 7, 0},
                {9, 19, 29, 48, 39, 30, 20, 10}
        });
        boards.add(new Integer[][]{
                {-9, -19, -29, -48, -39, -30, -20, -10},
                {0, -2, 0, -4, -5, -6, -7, -8},
                {0, 0, -3, 0, 0, 0, 0, 0},
                {-1, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 5, 0, 0, 8},
                {0, 0, 0, 0, 0, 0, 0, 0},
                {1, 2, 3, 4, 0, 6, 7, 0},
                {9, 19, 29, 48, 39, 30, 20, 10}
        });
        boards.add(new Integer[][]{
                {-9, -19, -29, -48, -39, -30, -20, -10},
                {0, -2, 0, -4, -5, -6, -7, -8},
                {0, 0, -3, 0, 0, 0, 0, 0},
                {-1, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 5, 0, 0, 8},
                {0, 0, 0, 0, 0, 0, 0, 10},
                {1, 2, 3, 4, 0, 6, 7, 0},
                {9, 19, 29, 48, 39, 30, 20, 0}
        });
        boards.add(new Integer[][]{
                {-9, -19, -29, -48, -39, -30, -20, -10},
                {0, -2, 0, 0, -5, -6, -7, -8},
                {0, 0, -3, -4, 0, 0, 0, 0},
                {-1, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 5, 0, 0, 8},
                {0, 0, 0, 0, 0, 0, 0, 10},
                {1, 2, 3, 4, 0, 6, 7, 0},
                {9, 19, 29, 48, 39, 30, 20, 0}
        });
        boards.add(new Integer[][]{
                {0, -19, -29, -48, -39, -30, -20, -10},
                {0, -2, 0, -4, -5, -6, -7, -8},
                {-9, 0, -3, -4, 0, 0, 0, 0},
                {-1, 0, 0, 0, 5, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 8},
                {0, 0, 0, 0, 0, 0, 0, 10},
                {1, 2, 3, 4, 0, 6, 7, 0},
                {9, 19, 29, 48, 39, 30, 20, 0}
        });
        boards.add(new Integer[][]{
                {0, -19, -29, -48, -39, -30, -20, -10},
                {0, -2, 0, 0, -5, -6, -7, -8},
                {-9, 0, -3, 0, 0, 0, 0, 0},
                {-1, 0, 0, 0, -4, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 8},
                {0, 0, 0, 0, 0, 0, 0, 10},
                {1, 2, 3, 4, 0, 6, 7, 0},
                {9, 19, 29, 48, 39, 30, 20, 0}
        });
    }

    public void initChessboard() {
        int width = getScreenWidth();
        int tileSize = width / 8;

        for (int i = 0; i < rows; i++) {
            TableRow row = new TableRow(this);
            ConstraintLayout.LayoutParams tbl = (ConstraintLayout.LayoutParams) chessBoard.getLayoutParams();
            tbl.width = width;
            tbl.height = width;

            TableRow.LayoutParams rp = new TableRow.LayoutParams(TableRow.LayoutParams.MATCH_PARENT, TableRow.LayoutParams.MATCH_PARENT);
            rp.height = tileSize;
            rp.width = tileSize;
            row.setLayoutParams(tbl);
            chessBoard.addView(row, i);

            for (int j = 0; j < cols; j++) {
                tiles[i][j] = new Tile(this, i, j);
                if ((i + j) % 2 == 0) {
                    tiles[i][j].setBackgroundColor(Color.WHITE);
                } else {
                    tiles[i][j].setBackgroundColor(Color.argb(100, 151, 182, 181));
                }
                tiles[i][j].setLayoutParams(rp);

                Piece p = null;

                //Pawn placement
                if (i == 1) {
                    p = new Piece(this, i, j, "wpawn", (char) (j + 1));
                    p.setImageResource(R.drawable.wpawn);
                } else if (i == 6) {
                    p = new Piece(this, i, j, "bpawn", (char) (-1 * j - 1));
                    p.setImageResource(R.drawable.bpawn);
                }
                //Rook
                else if (j == 7 && i == 7) {
                    p = new Piece(this, i, j, "brook", (char) -10);
                    p.setImageResource(R.drawable.brook);
                } else if (j == 0 && i == 7) {
                    p = new Piece(this, i, j, "brook", (char) -9);
                    p.setImageResource(R.drawable.brook);
                } else if (j == 7 && i == 0) {
                    p = new Piece(this, i, j, "wrook", (char) 10);
                    p.setImageResource(R.drawable.wrook);
                } else if (j == 0 && i == 0) {
                    p = new Piece(this, i, j, "wrook", (char) 9);
                    p.setImageResource(R.drawable.wrook);
                }
                //Knights
                else if (j == 6 && i == 7) {
                    p = new Piece(this, i, j, "bknight", (char) -20);
                    p.setImageResource(R.drawable.bknight);
                } else if (j == 1 && i == 7) {
                    p = new Piece(this, i, j, "bknight", (char) -19);
                    p.setImageResource(R.drawable.bknight);
                } else if (j == 6 && i == 0) {
                    p = new Piece(this, i, j, "wknight", (char) 20);
                    p.setImageResource(R.drawable.wknight);
                } else if (j == 1 && i == 0) {
                    p = new Piece(this, i, j, "wknight", (char) 19);
                    p.setImageResource(R.drawable.wknight);
                }
                //Bishops
                else if (j == 5 && i == 7) {
                    p = new Piece(this, i, j, "bbishop", (char) -30);
                    p.setImageResource(R.drawable.bbishop);
                } else if (j == 2 && i == 7) {
                    p = new Piece(this, i, j, "bbishop", (char) -29);
                    p.setImageResource(R.drawable.bbishop);
                } else if (j == 5 && i == 0) {
                    p = new Piece(this, i, j, "wbishop", (char) 30);
                    p.setImageResource(R.drawable.wbishop);
                } else if (j == 2 && i == 0) {
                    p = new Piece(this, i, j, "wbishop", (char) 29);
                    p.setImageResource(R.drawable.wbishop);
                }
                //Queen
                else if (j == 4 && i == 7) {
                    p = new Piece(this, i, j, "bqueen", (char) -39);
                    p.setImageResource(R.drawable.bqueen);
                } else if (j == 4 && i == 0) {
                    p = new Piece(this, i, j, "wqueen", (char) 39);
                    p.setImageResource(R.drawable.wqueen);
                }
                //Queen
                else if (j == 3 && i == 7) {
                    p = new Piece(this, i, j, "bking", (char) -48);
                    p.setImageResource(R.drawable.bking);
                } else if (j == 3 && i == 0) {
                    p = new Piece(this, i, j, "wking", (char) 48);
                    p.setImageResource(R.drawable.wking);
                }

                if (p != null) {
                    tiles[i][j].setPiece(p);
                }

                row.addView(tiles[i][j], j);
            }
        }
    }

    public void replayGame() {
        int iteration = 0;
        int length = 4;

        while (iteration < length) {
            // get the current board
            Integer[][] b = boards.get(iteration);

            for (int index = 0; index < rows; index++) {
                for (int jndex = 0; jndex < cols; jndex++) {

                    int i = index;
                    int j = jndex;
                    Piece p = tiles[i][j].getPiece();
                    Tile t = tiles[i][j];
                    int newPieceID = b[i][j];
                    Context context = this;

                    Handler h = new Handler();
                    Runnable r = new Runnable() {
                        public void run() {
                            if (t.hasPiece() && p != null) { // there was a already piece on that square
                                if (newPieceID == 0) { // piece gets replaced with a blank
                                    t.removePiece(p);
                                    p.setImageResource(android.R.color.transparent);
                                }
                                else if (newPieceID != (int) p.id) { // b[i][j] is not 0
                                    // if the new board is not the same as the current board
                                    t.removePiece(p);
                                    // TODO: fix casting once char id is turned into an int id
                                    t.setPiece(new Piece(context, i, j, "Piece", (char) newPieceID));
                                    p.setImageResource(android.R.color.transparent);
                                }
                            } else { // no piece was on that square
                                if (newPieceID != 0) { // the square on new board has a piece
                                    Piece pc = new Piece(context, i, j, "Piece", (char) newPieceID);
                                    t.setPiece(pc);
                                    Log.d("Replay Games Screen", String.valueOf(newPieceID));
                                    pc.setImageResource(imageMap.get(newPieceID));
                                }
                            }
                        }
                    };
                    h.postDelayed(r, 3000);
                }
            }

            iteration++;
        }
    }

    public static int getScreenWidth() {
        return Resources.getSystem().getDisplayMetrics().widthPixels;
    }
}