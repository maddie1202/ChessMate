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
import java.util.Collections;
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

    private HashMap<Integer, Integer[][]> boards;
    private HashMap<Integer, int[][]> gameBoards;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_replay_past_games_screen);

        backArrow = findViewById(R.id.arrow_back);
        gameID = getIntent().getIntExtra("gameID", 0);
        gameBoards = (HashMap<Integer, int[][]>) getIntent().getSerializableExtra("boards");
        backArrow.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent(ReplayPastGamesScreen.this, PastGamesScreen.class);
                startActivity(intent);
            }
        });

        Log.d("ReplayPastGames", "GameID is: " + gameID);
        context = this;
        imageMap = new BoardMap(this);
        tiles = new Tile[rows][cols];
        chessBoard = findViewById(R.id.chess);
        boards = new HashMap<>();
        mockBoards();

        initChessboard();
        replayGame();
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
        boards.put(0, new Integer[][]{
                {-9, -19, -29, -48, -39, -30, -20, -10},
                {-1, -2, -3, -4, -5, -6, -7, -8},
                {0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0},
                {1, 2, 3, 4, 5, 6, 7, 8},
                {9, 19, 29, 48, 39, 30, 20, 10}
        });
        boards.put(1, new Integer[][]{
                {-9, -19, -29, -48, -39, -30, -20, -10},
                {-1, -2, -3, -4, -5, -6, -7, -8},
                {0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 5, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0},
                {1, 2, 3, 4, 0, 6, 7, 8},
                {9, 19, 29, 48, 39, 30, 20, 10}
        });
        boards.put(2, new Integer[][]{
                {-9, -19, -29, -48, -39, -30, -20, -10},
                {-1, -2, 0, -4, -5, -6, -7, -8},
                {0, 0, -3, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 5, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0},
                {1, 2, 3, 4, 0, 6, 7, 8},
                {9, 19, 29, 48, 39, 30, 20, 10}
        });
        boards.put(3, new Integer[][]{
                {-9, -19, -29, -48, -39, -30, -20, -10},
                {-1, -2, 0, -4, -5, -6, -7, -8},
                {0, 0, -3, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 5, 0, 0, 8},
                {0, 0, 0, 0, 0, 0, 0, 0},
                {1, 2, 3, 4, 0, 6, 7, 0},
                {9, 19, 29, 48, 39, 30, 20, 10}
        });
        boards.put(4, new Integer[][]{
                {-9, -19, -29, -48, -39, -30, -20, -10},
                {0, -2, 0, -4, -5, -6, -7, -8},
                {0, 0, -3, 0, 0, 0, 0, 0},
                {-1, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 5, 0, 0, 8},
                {0, 0, 0, 0, 0, 0, 0, 0},
                {1, 2, 3, 4, 0, 6, 7, 0},
                {9, 19, 29, 48, 39, 30, 20, 10}
        });
        boards.put(5, new Integer[][]{
                {-9, -19, -29, -48, -39, -30, -20, -10},
                {0, -2, 0, -4, -5, -6, -7, -8},
                {0, 0, -3, 0, 0, 0, 0, 0},
                {-1, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 5, 0, 0, 8},
                {0, 0, 0, 0, 0, 0, 0, 10},
                {1, 2, 3, 4, 0, 6, 7, 0},
                {9, 19, 29, 48, 39, 30, 20, 0}
        });
        boards.put(6, new Integer[][]{
                {-9, -19, -29, -48, -39, -30, -20, -10},
                {0, -2, 0, 0, -5, -6, -7, -8},
                {0, 0, -3, -4, 0, 0, 0, 0},
                {-1, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 5, 0, 0, 8},
                {0, 0, 0, 0, 0, 0, 0, 10},
                {1, 2, 3, 4, 0, 6, 7, 0},
                {9, 19, 29, 48, 39, 30, 20, 0}
        });
        boards.put(7, new Integer[][]{
                {-9, -19, -29, -48, -39, -30, -20, -10},
                {0, -2, 0, 0, -5, -6, -7, -8},
                {0, 0, -3, -4, 0, 0, 0, 0},
                {-1, 0, 0, 0, 5, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 8},
                {0, 0, 0, 0, 0, 0, 0, 10},
                {1, 2, 3, 4, 0, 6, 7, 0},
                {9, 19, 29, 48, 39, 30, 20, 0}
        });
        boards.put(8, new Integer[][]{
                {-9, -19, -29, -48, -39, -30, -20, -10},
                {0, -2, 0, 0, -5, -6, -7, -8},
                {0, 0, -3, 0, 0, 0, 0, 0},
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
                    p = new Piece(this, i, j, "bpawn", (-1 * j - 1));
                    p.setImageResource(R.drawable.bpawn);
                } else if (i == 6) {
                    p = new Piece(this, i, j, "wpawn", (j + 1));
                    p.setImageResource(R.drawable.wpawn);
                }
                //Rook
                else if (j == 7 && i == 7) {
                    p = new Piece(this, i, j, "wrook", 10);
                    p.setImageResource(R.drawable.wrook);
                } else if (j == 0 && i == 7) {
                    p = new Piece(this, i, j, "wrook", 9);
                    p.setImageResource(R.drawable.wrook);
                } else if (j == 7 && i == 0) {
                    p = new Piece(this, i, j, "brook", -10);
                    p.setImageResource(R.drawable.brook);
                } else if (j == 0 && i == 0) {
                    p = new Piece(this, i, j, "brook", -9);
                    p.setImageResource(R.drawable.brook);

                }
                //Knights
                else if (j == 6 && i == 7) {
                    p = new Piece(this, i, j, "wknight", 20);
                    p.setImageResource(R.drawable.wknight);
                } else if (j == 1 && i == 7) {
                    p = new Piece(this, i, j, "wknight", 19);
                    p.setImageResource(R.drawable.wknight);
                } else if (j == 6 && i == 0) {
                    p = new Piece(this, i, j, "bknight", -20);
                    p.setImageResource(R.drawable.bknight);
                } else if (j == 1 && i == 0) {
                    p = new Piece(this, i, j, "bknight", -19);
                    p.setImageResource(R.drawable.bknight);
                }
                //Bishops
                else if (j == 5 && i == 7) {
                    p = new Piece(this, i, j, "wbishop", 30);
                    p.setImageResource(R.drawable.wbishop);
                } else if (j == 2 && i == 7) {
                    p = new Piece(this, i, j, "wbishop", 29);
                    p.setImageResource(R.drawable.wbishop);
                } else if (j == 5 && i == 0) {
                    p = new Piece(this, i, j, "bbishop", -30);
                    p.setImageResource(R.drawable.bbishop);
                } else if (j == 2 && i == 0) {
                    p = new Piece(this, i, j, "bbishop", -29);
                    p.setImageResource(R.drawable.bbishop);
                }
                //Queen
                else if (j == 4 && i == 7) {
                    p = new Piece(this, i, j, "wqueen", 39);
                    p.setImageResource(R.drawable.wqueen);
                } else if (j == 4 && i == 0) {
                    p = new Piece(this, i, j, "bqueen", -39);
                    p.setImageResource(R.drawable.bqueen);
                }
                //King
                else if (j == 3 && i == 7) {
                    p = new Piece(this, i, j, "wking", 48);
                    p.setImageResource(R.drawable.wking);
                } else if (j == 3 && i == 0) {
                    p = new Piece(this, i, j, "bking", -48);
                    p.setImageResource(R.drawable.bking);
                }

                if (p != null) {
                    tiles[i][j].setPiece(p);
                }

                row.addView(tiles[i][j], j);
            }
        }
    }

    public void replayGame() {
        int length = gameBoards.size();
        ArrayList<Integer> keys = new ArrayList<>(gameBoards.keySet());
        Collections.sort(keys);
        for (int i = 0; i < keys.size(); i++) {
            Log.d("ReplayPastGames", "Key index: " + keys.get(i));
        }

        Handler h = new Handler();
        for(int index = 0; index < length; index++) {
            int[][] b = gameBoards.get(keys.get(index));
            h.postDelayed(new Runnable() {
                public void run() {
                    for (int i = 0; i < rows; i++) {
                        for (int j = 0; j < cols; j++) {
                            Piece p = tiles[i][j].getPiece();
                            Tile t = tiles[i][j];
                            int newPieceID = b[i][j];

                            if (t.hasPiece() && p != null) { // there was a already piece on that square
                                if (newPieceID == 0) { // piece gets replaced with a blank
                                    t.removePiece(p);
                                } else if (newPieceID != p.id) { // b[i][j] is not 0
                                    // if the new board is not the same as the current board
                                    t.removePiece(p);
                                    Piece pc = new Piece(context, i, j, "Piece", newPieceID);
                                    t.setPiece(pc);
                                    Log.d("Replay Games Screen", "NewPieceID is " + String.valueOf(newPieceID));
                                    pc.setImageResource(imageMap.get(newPieceID));
                                }
                            } else { // no piece was on that square
                                if (newPieceID != 0) { // the square on new board has a piece
                                    Piece pc = new Piece(context, i, j, "Piece", newPieceID);
                                    t.setPiece(pc);
                                    Log.d("Replay Games Screen", "NewPieceID is " + String.valueOf(newPieceID));
                                    pc.setImageResource(imageMap.get(newPieceID));
                                }
                            }
                        }
                    }
                }

            }, 1000 * index);
        }
    }

    public static int getScreenWidth() {
        return Resources.getSystem().getDisplayMetrics().widthPixels;
    }
}