package com.example.chessiegame;

import androidx.annotation.RequiresApi;
import androidx.appcompat.app.AppCompatActivity;
import androidx.constraintlayout.widget.ConstraintLayout;

import android.annotation.SuppressLint;
import android.bluetooth.BluetoothDevice;
import android.content.ClipData;
import android.content.ClipDescription;
import android.content.Intent;
import android.content.res.Resources;
import android.graphics.Color;
import android.graphics.PorterDuff;
import android.os.Build;
import android.os.Bundle;
import android.os.CountDownTimer;
import android.os.Handler;
import android.os.ResultReceiver;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.DragEvent;
import android.view.Gravity;
import android.view.LayoutInflater;
import android.view.MotionEvent;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.ImageButton;
import android.widget.LinearLayout;
import android.widget.PopupWindow;
import android.widget.TableLayout;
import android.widget.TableRow;
import android.widget.TextView;
import android.bluetooth.BluetoothAdapter;
import android.widget.Toast;

import com.android.volley.Request;
import com.android.volley.RequestQueue;
import com.android.volley.Response;
import com.android.volley.VolleyError;
import com.android.volley.toolbox.JsonObjectRequest;
import com.android.volley.toolbox.StringRequest;
import com.android.volley.toolbox.Volley;
import com.example.chessiegame.components.BoardMap;
import com.example.chessiegame.components.Move;
import com.example.chessiegame.components.Piece;
import com.example.chessiegame.components.Tile;
import com.example.chessiegame.services.BluetoothService;
import com.google.firebase.auth.FirebaseAuth;
import com.google.firebase.auth.FirebaseUser;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.nio.ByteBuffer;
import java.util.Arrays;
import java.util.HashSet;
import java.util.List;
import java.util.Set;
import java.util.stream.IntStream;
import java.util.stream.Stream;

public class ChessScreen extends AppCompatActivity implements View.OnDragListener, View.OnTouchListener {

    private static final int REQUEST_ENABLE_BT = 0;
    private static final int REQUEST_DISCOVER_BT = 1;
    BluetoothAdapter mBlueAdapter;
    BTReceiver btReceiver;
    boolean err;

    private FirebaseAuth mAuth;
    private FirebaseUser user;
    private RequestQueue queue;

    public TableLayout chessBoard;
    private BoardMap imageMap;
    public Tile[][] tiles;
    public int[][] AIMove; // incoming board layout
    public int gameID;
    public HashSet<int[][]> validMoves; // set of all valid player move boards
    private final int rows = 8;
    private final int cols = 8;
    private final int word = 4;

    private boolean startNewGame;
    public int[][] resumedLayout;
    private boolean newGameFlag;
    private boolean resumeGameFlag;
    private boolean pauseGameFlag;
    private int gameResult;
    private int sequenceNum;

    boolean start_game_ack = false;
    boolean game_over = false;
    boolean white_wins = false;
    boolean wrook0_moved = false;
    boolean wrook1_moved = false;
    boolean brook0_moved = false;
    boolean brook1_moved = false;
    boolean wking_moved = false;
    boolean bking_moved = false;
    int num_player_moves;

    int width;
    int height;
    private CountDownTimer timer;
    long milliLeft;

    //TODO: notes
    // on resume, send the player's last move (second most recent board in DB) and send to DE1 to get possible moves
    TextView timerTextView;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_chess_screen);

        DisplayMetrics dm = new DisplayMetrics();
        getWindowManager().getDefaultDisplay().getMetrics(dm);
        width = dm.widthPixels;
        height = dm.heightPixels;

        mBlueAdapter = BluetoothAdapter.getDefaultAdapter();
        btReceiver = new BTReceiver(new Handler());
        err = false;

        queue = Volley.newRequestQueue(this);
        mAuth = FirebaseAuth.getInstance();
        user = mAuth.getCurrentUser();

        gameID = getIntent().getIntExtra("gameID", 0);
        resumedLayout = new int[rows][cols]; // layout of game in progress
        resumedLayout = (int[][]) getIntent().getSerializableExtra("resumedLayout");
        tiles = new Tile[rows][cols];
        AIMove = new int[rows][cols];
        chessBoard = findViewById(R.id.chess);
        imageMap = new BoardMap(this);
        validMoves = new HashSet<>();
        sequenceNum = getIntent().getIntExtra("sequenceNum", 1); // which move we're on

        newGameFlag = false;
        resumeGameFlag = false;
        pauseGameFlag = false;
        gameResult = -1;

        startNewGame = getIntent().getBooleanExtra("newGame", true);
        if (startNewGame) { // start a new game
            newGameFlag = true;
            postNewGameResult(user.getUid(), gameID);
        } else { // resume a game in progress
            resumeGameFlag = true;
        }

        initChessboard(startNewGame, resumedLayout);

        /*if (mBlueAdapter == null) {
            showToast("Bluetooth is not available");
        } else {
            // automatically turn on Bluetooth
            if (!mBlueAdapter.isEnabled()) {
                // intent to turn BT on
                Intent intent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
                startActivityForResult(intent, REQUEST_ENABLE_BT);
            }

            if (!err) {
                // intent to make BT discoverable
                Intent discover = new Intent(BluetoothAdapter.ACTION_REQUEST_DISCOVERABLE);
                startActivityForResult(discover, REQUEST_DISCOVER_BT);

                BluetoothDevice device = mBlueAdapter.getRemoteDevice("20:17:01:09:52:49"); // replace MAC address if need
                showToast("Connected to " + device.getName());

                if (device != null) {
                    Intent btIntent = new Intent(this, com.example.chessiegame.services.BluetoothService.class);
                    btIntent.putExtra("btDevice", device);
                    btIntent.putExtra("btReceiver", btReceiver);
                    btIntent.putExtra("gameID", gameID);
                    startService(btIntent);
                    //stopService(new Intent(this, BluetoothService.class)); --> to stop service
                } else {
                    showToast("Encountered a bluetooth error");
                }

            } else {
                showToast("Encountered a bluetooth error");
            }
        }*/

        timerTextView = (TextView) findViewById(R.id.timerTextView);
        timerTextView.setTextSize(20);
        timerTextView.setTextColor(Color.BLACK);

        /*String btTest = "Bluetooth Test";
        Intent bti = new Intent(this, com.example.chessiegame.services.BluetoothService.class);
        // MUST send byte[] data to bluetooth service using "userMove" tag
        bti.putExtra("userMove", btTest.getBytes());
        startService(bti);*/

        timerStart(10000);

        //POPUP
        // inflate the layout of the popup window
        LayoutInflater inflater = (LayoutInflater) getSystemService(LAYOUT_INFLATER_SERVICE);
        View popupView = inflater.inflate(R.layout.pause_game, null);
        // create the popup window
        int width_f = (int) (width*.9);
        //int height = LinearLayout.LayoutParams.WRAP_CONTENT;
        int height_f = (int) (height*.7);
        boolean focusable = true; // lets taps outside the popup also dismiss it
        final PopupWindow popupWindow = new PopupWindow(popupView, width_f, height_f, focusable);

        Button pause = (Button) findViewById(R.id.pause_button);
        Button quit = (Button) popupView.findViewById(R.id.quit);
        Button resume = (Button) popupView.findViewById(R.id.resume);
        ImageButton closeButton3 = (ImageButton) popupView.findViewById(R.id.close_button3);

        pause.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                //onButtonShowPopUp(view, width, height);
                // show the popup window
                // which view you pass in doesn't matter, it is only used for the window tolken
                popupWindow.showAtLocation(view, Gravity.CENTER, 0, 0);
                timerPause();
            }

        });

        quit.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                navigateToHome(gameID);
            }
        });

        resume.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                popupWindow.dismiss();
                timerResume();
            }
        });

        closeButton3.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                popupWindow.dismiss();
                timerResume();
            }
        });

        int targetSeqNum = 0;
        if (!startNewGame) {
            targetSeqNum = sequenceNum; // we want to get the last AI move (the last move by default)
            sequenceNum++; // increment seqNum by 1 to recalibrate to player moves
        }
        Intent btIntent = new Intent(this, BluetoothService.class);
        //btIntent.putExtra("btDevice", device);
        if (btReceiver != null) {
            btIntent.putExtra("btReceiver", btReceiver);
            btIntent.putExtra("gameID", gameID);
            btIntent.putExtra("sequenceNum", targetSeqNum);
            startService(btIntent);
        } else {
            Log.d("ChessScreen", "BTReceiver is null");
        }


    }

    public void timerStart(long timeLengthMili) {
        timer = new CountDownTimer(timeLengthMili, 1000) {
            public void onTick(long millisUntilFinished) {
                milliLeft= millisUntilFinished;
                long millis = millisUntilFinished;
                int seconds = (int) (millis / 1000);
                int minutes = seconds / 60;
                seconds = seconds % 60;
                timerTextView.setText(String.format("Time Remaining: %d:%02d", minutes, seconds));
            }

            public void onFinish() {
                timerTextView.setText("Over! You lost");
                updateGameResult(gameID, 0, 0); // update game with you lost

                LayoutInflater inflater = (LayoutInflater) getSystemService(LAYOUT_INFLATER_SERVICE);
                // create the popup window
                int width_f = (int) (width*.9);
                //int height = LinearLayout.LayoutParams.WRAP_CONTENT;
                int height_f = (int) (height*.7);
                boolean focusable = true; // lets taps outside the popup also dismiss it
               // final PopupWindow popupWindow = new PopupWindow(popupView, width_f, height_f, focusable);

                View popupViewlost = inflater.inflate(R.layout.popup_lost, null);
                // create the popup window
                final PopupWindow popupWindowLost = new PopupWindow(popupViewlost, width_f, height_f, focusable);
                popupWindowLost.showAtLocation(popupViewlost, Gravity.CENTER, 0, 0);
                Handler h = new Handler();
                Runnable r = new Runnable() {
                    public void run() {
                        Intent intent = new Intent(getApplicationContext(), HomeScreen.class);
                        startActivity(intent);
                    }
                };
                h.postDelayed(r,10000); // after 10 seconds, automatically go back to home
            }
        }.start();
    }

    public void timerPause() {
        timer.cancel();
    }

    private void timerResume() {
        timerStart(milliLeft);
    }

    private void navigateToHome(int gameID) {
        updateGameResult(gameID, -1, 500000); // TODO: make timeRemaining the actual time on the timer
        Intent intent = new Intent(ChessScreen.this, HomeActivity.class);
        intent.addFlags(Intent.FLAG_ACTIVITY_NO_ANIMATION);
        startActivity(intent);
        overridePendingTransition(0,0);
    }

    /**
     * Posts a new game result with ID gameID
     */
    public void postNewGameResult(String uid, int gameID) {
        String url = "http://ec2-user@ec2-54-153-82-188.us-west-1.compute.amazonaws.com:3000/postresult";
        JSONObject postData = new JSONObject();
        try {
            postData.put("userID", uid);
            postData.put("gameID", gameID);
            postData.put("result", gameResult);
        } catch (JSONException e) {
            e.printStackTrace();
        }

        Log.d("ChessScreen", "Posting result for " + gameID);

        JsonObjectRequest jsonObjectRequest = new JsonObjectRequest(Request.Method.POST, url, postData,
                response -> {
                    Log.d("ChessScreen", "Successfully posted game result");
                }, error -> {
            Log.d("ChessScreen", error.toString());
        });

        queue.add(jsonObjectRequest);
    }

    /**
     * Updates the game result in the DB
     */
    public void updateGameResult(int gameID, int result, int timeRemaining) {
        String url = "http://ec2-user@ec2-54-153-82-188.us-west-1.compute.amazonaws.com:3000/postresult";
        JSONObject postData = new JSONObject();
        try {
            postData.put("gameID", gameID);
            postData.put("result", result); // 0 for lose game, 1 for win game
            postData.put("timeleft", timeRemaining);
        } catch (JSONException e) {
            e.printStackTrace();
        }

        JsonObjectRequest jsonObjectRequest = new JsonObjectRequest(Request.Method.POST, url, postData,
                response -> {
                    Log.d("ChessScreen", "Successfully updated game result");
                }, error -> {
            Log.d("ChessScreen", error.toString());
        });

        queue.add(jsonObjectRequest);
    }

    /**
     * Method to get the tiles[][] and transform it to a 2D array of ints
     */
    int[][] boardToIntArray() {
        int[][] board = new int[rows][cols];

        for (int i = 0; i < cols ; i ++) {
            for (int j = 0; j < rows; j++) {
                if (tiles[i][j].getPiece() == null) {
                    board[i][j] = 0;
                } else {
                    board[i][j] = tiles[i][j].getPiece().id;
                }
            }
        }

        return board;
    }

    /**
     * Converts the current board into a String and send to the DB
     */
    public String boardToStringAndPost() {
        StringBuilder sb = new StringBuilder();

        for (int i = 0; i < rows ; i ++) {
            for (int j = 0; j < cols; j++) {
                if (tiles[i][j].getPiece() == null) {
                    sb.append(0);
                } else {
                    sb.append(tiles[i][j].getPiece().id);
                }
                sb.append(' ');
            }
        }

        // boardMoves contains piece IDs each separated by a space
        String boardMoves = sb.toString();
        String url = "http://ec2-user@ec2-54-153-82-188.us-west-1.compute.amazonaws.com:3000/makeboard";
        JSONObject postData = new JSONObject();
        try {
            postData.put("placements", boardMoves);
            postData.put("gameID", gameID);
            postData.put("sequenceNum", sequenceNum);
        } catch (JSONException e) {
            e.printStackTrace();
        }

        Log.d("ChessScreen", "Post board gameID is: " + gameID);
        Log.d("ChessScreen", "Post board placement string: " + boardMoves);
        Log.d("ChessScreen", "Post board sequenceNum: " + sequenceNum);

        JsonObjectRequest jsonObjectRequest = new JsonObjectRequest(Request.Method.POST, url, postData,
                response -> {
                    Log.d("ChessScreen", "Successfully posted board");
                    sequenceNum += 2; // increment to next player seqNum, all player sequenceNums are ODD
                }, error -> {
                    Log.d("ChessScreen", error.toString());
        });

        queue.add(jsonObjectRequest);

        return boardMoves;
    }

    public void initChessboard(boolean newGame, int[][] resumedLayout) {
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
                tiles[i][j].setOnDragListener(this);

                Piece p = null;

                if (newGame) {
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
                } else { // assign layout based on prev game state
                    Log.d("ChessScreen", "In progress");
                    if (resumedLayout[i][j] != 0) { // there is a piece on this tile
                        int id = resumedLayout[i][j];
                        p = new Piece(this, i, j, "", id);
                        p.setImageResource(imageMap.get(id));
                    }
                }

                if (p != null) {
                    tiles[i][j].setPiece(p);
                    p.setOnTouchListener(this);
                }

                row.addView(tiles[i][j], j);
            }
        }
    }

    public static int getScreenWidth() {
        return Resources.getSystem().getDisplayMetrics().widthPixels;
    }

    @Override
    public boolean onTouch(View v, MotionEvent event) {
        if (event.getAction() == MotionEvent.ACTION_DOWN) {
            ClipData data = ClipData.newPlainText("", "");
            View.DragShadowBuilder shadowBuilder = new View.DragShadowBuilder(
                    v);
            v.startDrag(data, shadowBuilder, v, 0);
            v.setVisibility(View.INVISIBLE);
            return true;
        } else {
            return false;
        }
    }

    @Override
    public boolean onDrag(View v, DragEvent event) {
        // Defines a variable to store the action type for the incoming event
        int action = event.getAction();
        // Handles each of the expected events
        switch (action) {

            case DragEvent.ACTION_DRAG_STARTED:
                // Determines if this View can accept the dragged data
                if (event.getClipDescription().hasMimeType(ClipDescription.MIMETYPE_TEXT_PLAIN)) {
                    return true;
                }

                // Returns false. During the current drag and drop operation, this View will
                // not receive events again until ACTION_DRAG_ENDED is sent.
                return false;

            case DragEvent.ACTION_DRAG_ENTERED:
                // Applies a GRAY or any color tint to the View. Return true; the return value is ignored.
                v.getBackground().setColorFilter(Color.GRAY, PorterDuff.Mode.SRC_IN);
                // Invalidate the view to force a redraw in the new tint
                v.invalidate();
                return true;

            case DragEvent.ACTION_DRAG_LOCATION:
                // Ignore the event
                return true;

            case DragEvent.ACTION_DRAG_EXITED:

            case DragEvent.ACTION_DRAG_ENDED:
                // Turns off any color tinting
                v.getBackground().clearColorFilter();
                // Invalidate the view to force a redraw in the new tint
                v.invalidate();
                return true;

            case DragEvent.ACTION_DROP:
                // Turns off any color tints
                v.getBackground().clearColorFilter();
                // Invalidates the view to force a redraw
                v.invalidate();

                // the view that was dropped
                View vw = (View) event.getLocalState();
                Piece p = (Piece) vw;
                Log.d("ChessScreen", "Dropped piece: " + p.name);

                wrook0_moved = false;
                wrook1_moved = false;
                brook0_moved = false;
                brook1_moved = false;
                wking_moved = false;
                bking_moved = false;

                ViewGroup owner = (ViewGroup) vw.getParent();
                owner.removeView(vw); // remove the dragged view

                // start updating tiles[][] with new arrangement
                Tile t = (Tile) v;
                int c = t.id % 8;
                int r = t.id / 8;
                Log.d("ChessScreen", "Dropped on tile: " + t.id);

                // piece stores information on its previous location
                int prevRow = p.row;
                int prevCol = p.col;
                Log.d("ChessScreen", "Piece removed from row: " + prevRow + ", column: " + prevCol);
                tiles[prevRow][prevCol].removePiece(p); // remove the piece from prev tile

                p.updateCoordinates(r, c); // update p's coordinates
                boolean removedPiece = false;
                boolean validMove = true;
                Piece tmp = tiles[r][c].getPiece();
                if (tiles[r][c].hasPiece() && tmp != null) { // check if there was already a piece in the drop view
                    tiles[r][c].removePiece(tmp);
                    removedPiece = true;
                }

                tiles[r][c].setPiece(p); // drop p in its new location
                int[][] newLayout = boardToIntArray();

                if (!validMoves.contains(newLayout)) { // MOVE VALIDATION
                    // TODO: reject the move and undo everything, do nothing for now
                    /*
                    showToast("You played an invalid move");
                    validMove = false;
                    if (removedPiece) { // restore temp
                        tiles[r][c].removePiece(p);
                        tiles[r][c].setPiece(tmp);
                    }
                    tiles[prevRow][prevCol].setPiece(p);
                    p.updateCoordinates(prevRow, prevCol);*/
                }
                vw.setVisibility(View.VISIBLE); // finally set Visibility to VISIBLE

                if (validMove) {
                    clearValidMoves(); // delete the table of valid moves in prep for the next one
                    if (p.id == 9) wrook0_moved = true;
                    else if (p.id == 10) wrook1_moved = true;
                    else if (p.id == -9) brook0_moved = true;
                    else if (p.id == -10) brook1_moved = true;
                    else if (p.id == 48) wking_moved = true;
                    else if (p.id == -48) bking_moved = true;
                    forwardPlayerMove(); // send player move to Service and POST to db
                }

                return true;
            default:
                Log.e("DragDrop Example", "Unknown action type received by OnDragListener.");
                break;
        }
        return false;
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        switch (requestCode){
            case REQUEST_ENABLE_BT:
                if (resultCode == RESULT_OK){
                    //bluetooth is on
                    showToast("Bluetooth is on");
                }
                else {
                    //user denied to turn bluetooth on
                    showToast("Couldn't turn on Bluetooth");
                    err = true;
                }
                break;
        }
        super.onActivityResult(requestCode, resultCode, data);
    }

    private void showToast(String msg){
        Toast.makeText(this, msg, Toast.LENGTH_SHORT).show();
    }

    /**
     * Clears the valid moves table after a player has made a valid move
     */
    private void clearValidMoves() {
        String url = "http://ec2-user@ec2-54-153-82-188.us-west-1.compute.amazonaws.com:3000/deleteallmoves";

        StringRequest stringRequest = new StringRequest(Request.Method.DELETE, url,
                response -> {
                    Log.d("ChessScreen", "Successfully deleted valid moves table");
                }, error -> {
            Log.d("ChessScreen", error.toString());
        });

        queue.add(stringRequest);
    }

    /**
     * Post the player's move to the database and send a signal to the service
     */
    private void forwardPlayerMove() {
        boardToStringAndPost(); // sends board to the db
        Intent intent = new Intent(this, com.example.chessiegame.services.BluetoothService.class);
        startService(intent);
    }

    /**
     * Posts the player move to the DB and send player data to the BT module
     * ---> Assumes that the player's move has already been validated
     * bool start_game; -> set only when starting a new game
     * bool pause_game; -> 0 for false, 1 for true
     * bool resume_game;
     * bool wrook0_moved;
     * bool wrook1_moved;
     * bool brook0_moved;
     * bool brook1_moved;
     * bool wking_moved;
     * bool bking_moved;
     * int playebyte[] wrook0 = wrook0_moved ? intToBytes(1) : intToBytes(0);r_move[8][8]; -> words 36 to 291
     * Total length of byte array: 292 bytes
     */
    byte[] userMove = new byte[292];
    private void sendPlayerMoveBT() {
        boardToStringAndPost(); // sends board to the db
        Intent intent = new Intent(this, com.example.chessiegame.services.BluetoothService.class);

        // newGame flag
        byte[] newGame = new byte[word];
        if (newGameFlag) {
            newGame = intToBytes(1);
            newGameFlag = false; // only send this as true once
        } else { newGame = intToBytes(0); }
        fillUserDataArray(newGame, 0, 3);

        // pauseGame flag
        byte[] pauseGame = new byte[word];
        if (pauseGameFlag) {
            pauseGame = intToBytes(1);
            pauseGameFlag = false; // only send this as true once
        } else { pauseGame = intToBytes(0); }
        fillUserDataArray(pauseGame, 4, 7);

        // resumeGame flag
        byte[] resumeGame = new byte[word];
        if (resumeGameFlag) {
            resumeGame = intToBytes(1);
            resumeGameFlag = false; // only send this as true once
        } else { resumeGame = intToBytes(0); }
        fillUserDataArray(resumeGame, 8, 11);

        // set pieces
        byte[] wrook0 = wrook0_moved ? intToBytes(1) : intToBytes(0);
        byte[] wrook1 = wrook0_moved ? intToBytes(1) : intToBytes(0);
        byte[] brook0 = wrook0_moved ? intToBytes(1) : intToBytes(0);
        byte[] brook1 = wrook0_moved ? intToBytes(1) : intToBytes(0);
        byte[] wking = wrook0_moved ? intToBytes(1) : intToBytes(0);
        byte[] bking = wrook0_moved ? intToBytes(1) : intToBytes(0);

        fillUserDataArray(wrook0, 12, 15);
        fillUserDataArray(wrook1, 16, 19);
        fillUserDataArray(brook0, 20, 23);
        fillUserDataArray(brook1, 24, 27);
        fillUserDataArray(wking, 28, 31);
        fillUserDataArray(bking, 32, 35);

        // parse chessboard (tiles[][]) into userMove array
        int[][] pieceGrid = boardToIntArray();
        int index = 36;
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                fillUserDataArray(intToBytes(pieceGrid[i][j]), index, index + 3);
                index += 4;
            }
        }

        intent.putExtra("userMove", userMove);
        startService(intent);
    }

    /**
     * Populates the byte array to be sent to BT from index start to end (inclusive)
     * Input: data - the byte data to be put in the userMove array
     * Input: start - the index at which to start filling userMove array
     * Input: end - the index to which the userMove array will be populated
     */
    private void fillUserDataArray(byte[] data, int start, int end) {
        int j = 0;
        for (int i = start; i < end + 1; i++) {
            userMove[i] = data[j];
            j++;
        }
    }

    /**
     * Converts an integer into a word
     */
    private byte[] intToBytes( final int i ) {
        ByteBuffer bb = ByteBuffer.allocate(4);
        bb.putInt(i);
        return bb.array();
    }

    /**
     * Renders the opponent's move on the chess board
     * -> checks for differences to the current tiles[][] layout and applies them
     */
    private void renderOpponentMove() {
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                Piece p = tiles[i][j].getPiece();
                Tile t = tiles[i][j];
                int newPieceID = AIMove[i][j];

                if (t.hasPiece() && p != null) { // there was a already piece on that square
                    if (newPieceID == 0) { // piece gets replaced with a blank
                        t.removePiece(p);
                    } else if (newPieceID != p.id) { // AIMove[i][j] is not 0
                        // if the new board is not the same as the current board
                        t.removePiece(p);
                        Piece pc = new Piece(this, i, j, "Piece", newPieceID);
                        t.setPiece(pc);
                        Log.d("Chess Screen", "NewPieceID is " + String.valueOf(newPieceID));
                        pc.setImageResource(imageMap.get(newPieceID));
                    }
                } else { // no piece was on that square
                    if (newPieceID != 0) { // the square on new board has a piece
                        Piece pc = new Piece(this, i, j, "Piece", newPieceID);
                        t.setPiece(pc);
                        Log.d("Chess Screen", "NewPieceID is " + String.valueOf(newPieceID));
                        pc.setImageResource(imageMap.get(newPieceID));
                    }
                }
            }
        }
    }

    public class BTReceiver extends ResultReceiver {

        /**
         * Create a new ResultReceive to receive results.  Your
         * {@link #onReceiveResult} method will be called from the thread running
         * <var>handler</var> if given, or from an arbitrary thread if null.
         *
         * @param handler
         */
        public BTReceiver(Handler handler) {
            super(handler);
        }

        /**
         * Receives info from DE1 in byte array format then:
         * - verifies that it has all the data it is expecting
         * - extracts fields and stores into local variables
         * - repackages the valid user moves into a map structure (TOOD)
         * - parses the AI move to an int[][] of piece IDs
         * - applies the AI move to the chessboard
         */
        @Override
        protected void onReceiveResult(int resultCode, Bundle resultData) {
            super.onReceiveResult(resultCode, resultData);
            /* From DE1:
                bool start_game_ack; (0th word) -> 4 bytes
                bool game-over; (1st word)
                bool white_wins; (2nd word)
                bool wrook0_moved; (3rd word)
                bool wrook1_moved; (4th word)
                bool brook0_moved; (5th word)
                bool brook1_moved; (6th word)
                bool wking_moved; (7th word)
                bool bking_moved; (8th word)
                int ai_move[8][8]; (9th - 72nd words)
                int num_player_moves; (73rd word)
                int possible_player_moves[86][8][8]; (74th to 5578th words)

                Total: ~12000 bytes
             */
            byte[] data = resultData.getByteArray("readData");
            Log.d("ChessScreen", "Chess screen received data");

            if (data != null && data.length > 300) { // bluetooth receiver
                start_game_ack = fourByteToBoolean(Arrays.copyOfRange(data, 0, 3));
                game_over = fourByteToBoolean(Arrays.copyOfRange(data, 4, 7));
                white_wins = fourByteToBoolean(Arrays.copyOfRange(data, 8, 11));
                wrook0_moved = fourByteToBoolean(Arrays.copyOfRange(data, 12, 15));
                wrook1_moved = fourByteToBoolean(Arrays.copyOfRange(data, 16, 19));
                brook0_moved = fourByteToBoolean(Arrays.copyOfRange(data, 20, 23));
                brook1_moved = fourByteToBoolean(Arrays.copyOfRange(data, 24, 27));
                wking_moved = fourByteToBoolean(Arrays.copyOfRange(data, 28, 31));
                bking_moved = fourByteToBoolean(Arrays.copyOfRange(data, 32, 35));

                // parse ai move from byte array to int[][] board layout
                int index = 36;
                for (int i = 0; i < rows; i++) {
                    for (int j = 0; j < cols; j++) {
                        AIMove[i][j] = fourByteToInt(Arrays.copyOfRange(data, index, index + 3));
                        index = index + 4;
                    }
                }

                // index = 292 at this point
                renderOpponentMove(); // render the ai move on the gameboard
                num_player_moves = fourByteToInt(Arrays.copyOfRange(data, 292, 295));

                // TODO: parse possible player moves
            } else { // alternate protocol - receive AI move and possible player moves
                // Log.d("ChessScreen", "Received string from BT: " + Arrays.toString(data)); - test receive string
                AIMove = (int[][]) resultData.getSerializable("AIMove");
                validMoves = (HashSet<int[][]>) resultData.getSerializable("validMoves");
                gameResult = resultData.getInt("result");

                if (gameResult == -1) { // player was not the winner
                    renderOpponentMove();
                } else if (gameResult == 0) { // AI won
                    // TODO: display that AI won
                    renderOpponentMove();

                    updateGameResult(gameID, gameResult, 0);
                } else {
                    // TODO: display that player won
                    updateGameResult(gameID, gameResult, 0);
                }
            }
        }

        /**
         * Helper function: Merges the value of 2 bytes into a boolean
         */
        public boolean fourByteToBoolean(byte[] arr) {
            return ByteBuffer.wrap(arr).getInt() != 0;
        }

        /**
         * Helper function: Merges the value of 2 bytes into an integer
         */
        public int fourByteToInt(byte[] arr) {
            return ByteBuffer.wrap(arr).getInt();
        }

    }

}

