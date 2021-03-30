package com.example.chessiegame;

import androidx.annotation.RequiresApi;
import androidx.appcompat.app.AppCompatActivity;
import androidx.cardview.widget.CardView;
import androidx.constraintlayout.widget.ConstraintLayout;

import android.annotation.SuppressLint;
import android.bluetooth.BluetoothDevice;
import android.content.ClipData;
import android.content.ClipDescription;
import android.content.Intent;
import android.content.res.Resources;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.PorterDuff;
import android.graphics.drawable.Drawable;
import android.os.Build;
import android.os.Bundle;
import android.os.CountDownTimer;
import android.os.Handler;
import android.util.Log;
import android.view.DragEvent;
import android.view.MotionEvent;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.FrameLayout;
import android.widget.GridLayout;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.RelativeLayout;
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
import com.example.chessiegame.components.Move;
import com.example.chessiegame.components.Piece;
import com.example.chessiegame.components.Tile;
import com.google.firebase.auth.FirebaseAuth;
import com.google.firebase.auth.FirebaseUser;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.util.ArrayList;
import java.util.List;
import java.util.Set;
import java.util.concurrent.TimeUnit;
import java.lang.Integer.*;

public class ChessScreen extends AppCompatActivity implements View.OnDragListener, View.OnTouchListener {

    private static final int REQUEST_ENABLE_BT = 0;
    private static final int REQUEST_DISCOVER_BT = 1;
    BluetoothAdapter mBlueAdapter;
    TextView paired_devices;
    boolean err;

    private FirebaseAuth mAuth;
    private FirebaseUser user;
    private RequestQueue queue;

    public TableLayout chessBoard;
    public Tile[][] tiles;
    public char[] prevGame;
    public int gameID;
    private final int rows = 8;
    private final int cols = 8;

    private boolean startNewGame;
    public String boardString;

    //TODO: notes
    // on resume, send the player's last move (second most recent board in DB) and send to DE1 to get possible moves
    TextView timerTextView;
    long startTime = 0;

    //runs without a timer by reposting this handler at the end of the runnable
    /*
    Handler timerHandler = new Handler();
    Runnable timerRunnable = new Runnable() {

        @Override
        public void run() {
            long millis = System.currentTimeMillis() - startTime;
            int seconds = (int) (millis / 1000);
            int minutes = seconds / 60;
            seconds = seconds % 60;

            timerTextView.setText(String.format("%d:%02d", minutes, seconds));

            timerHandler.postDelayed(this, 500);
        }
    };

     */

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_chess_screen);

        mBlueAdapter = BluetoothAdapter.getDefaultAdapter();
        paired_devices = (TextView) findViewById(R.id.paired_devices);
        err = false;

        queue = Volley.newRequestQueue(this);
        mAuth = FirebaseAuth.getInstance();
        user = mAuth.getCurrentUser();

        gameID = getIntent().getIntExtra("gameID", 0);
        boardString = getIntent().getStringExtra("boardString");
        tiles = new Tile[rows][cols];
        chessBoard = findViewById(R.id.chess);

        startNewGame = getIntent().getBooleanExtra("newGame", true);
        if (startNewGame) { // get the most recent game and its gamestate
            int difficulty = getIntent().getIntExtra("difficulty", 1);
            postNewGame("xQYSsLmZ8JU6jCNL1kL7g7QcDqE3", difficulty);
        }

        initChessboard(startNewGame, "");

        if (mBlueAdapter == null) {
            showToast("Bluetooth is not available");
        } else {
            // automatically turn on Bluetooth
            if (!mBlueAdapter.isEnabled()) {
                showToast("Turning On Bluetooth...");
                // intent to turn BT on
                Intent intent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
                startActivityForResult(intent, REQUEST_ENABLE_BT);
            } else {
                showToast("Bluetooth is already on");
            }

            if (!err) {
                // intent to make BT discoverable
                Intent discover = new Intent(BluetoothAdapter.ACTION_REQUEST_DISCOVERABLE);
                startActivityForResult(discover, REQUEST_DISCOVER_BT);
                showToast("Bluetooth is discoverable");

                Set<BluetoothDevice> devices = mBlueAdapter.getBondedDevices();
                String paired = "";

                for (BluetoothDevice device : devices) {
                    showToast("Connected to " + device.getName());
                }

            } else {
                showToast("Encountered an error");
            }
        }

        timerTextView = (TextView) findViewById(R.id.timerTextView);

        /*

        Button b = (Button) findViewById(R.id.button);
        b.setText("start");
        b.setOnClickListener(new View.OnClickListener() {

            @Override
            public void onClick(View v) {
                Button b = (Button) v;
                if (b.getText().equals("stop")) {
                    timerHandler.removeCallbacks(timerRunnable);
                    b.setText("start");
                } else {
                    startTime = System.currentTimeMillis();
                    timerHandler.postDelayed(timerRunnable, 0);
                    b.setText("stop");
                }
            }
        });

         */


        new CountDownTimer(600000, 1000) {

            public void onTick(long millisUntilFinished) {
                long millis = millisUntilFinished;
                int seconds = (int) (millis / 1000);
                int minutes = seconds / 60;
                seconds = seconds % 60;

                timerTextView.setText(String.format("%d:%02d", minutes, seconds));
            }

            public void onFinish() {
                timerTextView.setText("Over! You loose");
            }
        }.start();


    }

    /*

    @Override
    public void onPause() {
        super.onPause();
        timerHandler.removeCallbacks(timerRunnable);
        Button b = (Button)findViewById(R.id.button);
        b.setText("start");
    }

     */



    public void postNewGame(String uid, int difficulty) {
        String url = "http://ec2-user@ec2-54-153-82-188.us-west-1.compute.amazonaws.com:3000/makegame";
        JSONObject postData = new JSONObject();
        try {
            postData.put("difficulty", difficulty);
        } catch (JSONException e) {
            e.printStackTrace();
        }

        JsonObjectRequest jsonObjectRequest = new JsonObjectRequest(Request.Method.POST, url, postData,
                response -> {
                    // TODO: parse gameID from response
                    Log.d("ChessScreen", "Successfully posted game");
                    JSONObject res = (JSONObject) response;
                    try {
                        gameID = Integer.parseInt(res.get("gameID").toString());
                        postNewGameResult(uid, gameID);

                    } catch (JSONException e) {
                        e.printStackTrace();
                    }
                    Log.d("ChessScreen", String.valueOf(gameID));
                }, error -> {
                    Log.d("ChessScreen", "Failed to post game");
                });

        queue.add(jsonObjectRequest);
    }

    public void postNewGameResult(String uid, int gameID) {
        String url = "http://ec2-user@ec2-54-153-82-188.us-west-1.compute.amazonaws.com:3000/postresult";
        JSONObject postData = new JSONObject();
        try {
            postData.put("userID", uid);
            postData.put("gameID", gameID);
            postData.put("result", -1);
        } catch (JSONException e) {
            e.printStackTrace();
        }

        JsonObjectRequest jsonObjectRequest = new JsonObjectRequest(Request.Method.POST, url, postData,
                response -> {
                    Log.d("ChessScreen", "Successfully posted game result");
                    boardToString(); // post the initialized chessboard
                }, error -> {
            Log.d("ChessScreen", error.toString());
        });

        queue.add(jsonObjectRequest);

    }

    //Method to get the tiles[][] and transform it to a list of char
    char[][] boardformat(){
        char[][] board = new char[8][8];

        for (int i = 0; i < cols ; i ++) {
            for (int j = 0; j < rows; j++) {
                board [i][j] = tiles[i][j].getPiece().id;
            }
        }

        return board;
    }


    //Takes a list with all possible moves that a player can make, the move that player wants to make
    //Returns if the move is valid or not
    boolean isMoveValid(List<Character> move_list, Move pmove){

        Piece p = mockMove(pmove);
        char[][] presentBoard = boardformat();
        if (move_list.contains(presentBoard)){
            return true;
        }

        undoMove(pmove, p);
        return false;
    }

    //Get our present board and make it as a string
    public String boardToString() {
        char[] boardArray = new char[rows * cols];

        for (int i = 0; i < rows ; i ++) {
            for (int j = 0; j < cols; j++) {
                if (tiles[i][j].getPiece() == null) {
                    boardArray[i * rows + j] = (char) 0;
                } else {
                    boardArray[i * rows + j] = tiles[i][j].getPiece().id;
                }
            }
        }

        String boardMoves = boardArray.toString();
        String url = "http://ec2-user@ec2-54-153-82-188.us-west-1.compute.amazonaws.com:3000/makeboard";
        JSONObject postData = new JSONObject();
        try {
            postData.put("boardID", 1);
            postData.put("placements", boardMoves);
            postData.put("gameID", gameID);
            postData.put("sequenceNum", 1);
        } catch (JSONException e) {
            e.printStackTrace();
        }

        Log.d("ChessScreen", "Post board gameID is: " + String.valueOf(gameID));
        Log.d("ChessScreen", "Post board placement string: " + boardMoves);
        Log.d("ChessScreen", "Post board example piece: " + tiles[0][0].getPiece().name);

        JsonObjectRequest jsonObjectRequest = new JsonObjectRequest(Request.Method.POST, url, postData,
                response -> {
                    Log.d("ChessScreen", "Successfully posted board");
                }, error -> {
                    Log.d("ChessScreen", error.toString());
        });

        queue.add(jsonObjectRequest);

        return boardMoves;
    }

    //Apply a move to our board
    void applyMoveToBoard(Move move){
        Piece empty = new Piece (this, move.getInit_row(), move.getInit_col(), "empty", (char) 0);
        tiles[move.getInit_col()][move.getInit_row()].setPiece(empty);
        tiles[move.getDest_col()][move.getDest_row()].setPiece(move.getPiece());
    }

    //Mock how the board would be if we applied the move
    //Returns the piece that was on the tile that we put our piece
    Piece mockMove(Move move){
        Piece empty = new Piece (this, move.getInit_row(), move.getInit_col(), "empty", (char) 0);
        tiles[move.getInit_col()][move.getInit_row()].setPiece(empty);
        Piece ret = tiles[move.getDest_col()][move.getDest_row()].getPiece();
        tiles[move.getDest_col()][move.getDest_row()].setPiece(move.getPiece());
        return ret;
    }


    void undoMove(Move move, Piece p){
        //Set the intial piece back to its intial positions
        tiles[move.getInit_col()][move.getInit_row()].setPiece(move.getPiece());
        //Set the piece that used to be in the board back to its positions
        tiles[move.getDest_col()][move.getDest_row()].setPiece(p);
    }

    public void initChessboard(boolean newGame, String boardString) {
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
                        p = new Piece(this, i, j, "wpawn", (char) (j + 1));
                        p.setImageResource(R.drawable.wpawn);
                    } else if (i == 6) {
                        p = new Piece(this, i, j, "wpawn", (char) (-1 * j - 1));
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
                } else { // TODO: assign layout based on prev game state
                    Log.d("ChessScreen", "In progress");
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
                    // if you want to apply color when drag started to your view you can uncomment below lines
                    // to give any color tint to the View to indicate that it can accept data.
                    // v.getBackground().setColorFilter(Color.BLUE, PorterDuff.Mode.SRC_IN);
                    // Invalidate the view to force a redraw in the new tint
                    //  v.invalidate();
                    // returns true to indicate that the View can accept the dragged data.
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
                // Re-sets the color tint to blue. Returns true; the return value is ignored.
                // view.getBackground().setColorFilter(Color.BLUE, PorterDuff.Mode.SRC_IN);
                //It will clear a color filter .
                v.getBackground().clearColorFilter();
                // Invalidate the view to force a redraw in the new tint
                v.invalidate();
                return true;

            case DragEvent.ACTION_DROP:
                // Gets the item containing the dragged data
                ClipData.Item item = event.getClipData().getItemAt(0);
                // Gets the text data from the item.
                String dragData = item.getText().toString();
                // Displays a message containing the dragged data.
                Toast.makeText(this, "Dragged data is " + dragData, Toast.LENGTH_SHORT).show();
                // Turns off any color tints
                v.getBackground().clearColorFilter();
                // Invalidates the view to force a redraw
                v.invalidate();

                //TODO IS MOVE VALID??? CHECK

                View vw = (View) event.getLocalState();
                ViewGroup owner = (ViewGroup) vw.getParent();
                owner.removeView(vw); //remove the dragged view
                //caste the view into LinearLayout as our drag acceptable layout is LinearLayout
                CardView container = (CardView) v;
                container.addView(vw);//Add the dragged view
                vw.setVisibility(View.VISIBLE);//finally set Visibility to VISIBLE
                // Returns true. DragEvent.getResult() will return true.
                return true;

            case DragEvent.ACTION_DRAG_ENDED:
                // Turns off any color tinting
                v.getBackground().clearColorFilter();
                // Invalidates the view to force a redraw
                v.invalidate();
                // Does a getResult(), and displays what happened.
                return true;
            // An unknown action type was received.
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

}

