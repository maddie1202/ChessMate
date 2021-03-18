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
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.PorterDuff;
import android.graphics.drawable.Drawable;
import android.os.Build;
import android.os.Bundle;
import android.util.Log;
import android.view.DragEvent;
import android.view.MotionEvent;
import android.view.View;
import android.view.ViewGroup;
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

import com.example.chessiegame.components.Board;
import com.example.chessiegame.components.Piece;
import com.example.chessiegame.components.Tile;

import java.util.Set;

public class ChessScreen extends AppCompatActivity implements View.OnDragListener, View.OnTouchListener {

    private static final int REQUEST_ENABLE_BT = 0;
    private static final int REQUEST_DISCOVER_BT = 1;
    BluetoothAdapter mBlueAdapter;
    TextView paired_devices;

    boolean err;
    public Board board_view;
    public TableLayout chessBoard;
    public Tile[][] tiles;
    private final int rows = 8;
    private final int cols = 8;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        //board_view = new Board(this);
        setContentView(R.layout.activity_chess_screen);
        //bbishop = findViewById(R.id.bbishop);
        //bbishop.setOnTouchListener(this);
        //findViewById(R.id.tile00).setOnDragListener(this);

        mBlueAdapter = BluetoothAdapter.getDefaultAdapter();
        paired_devices = (TextView) findViewById(R.id.paired_devices);
        err = false;

        tiles = new Tile[rows][cols];
        chessBoard = findViewById(R.id.chess);
        initChessboard();

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

    }

    public void initChessboard() {
        for (int i = 0; i < rows; i++) {
            TableRow row = new TableRow(this);
            ConstraintLayout.LayoutParams tbl = (ConstraintLayout.LayoutParams) chessBoard.getLayoutParams();
            TableRow.LayoutParams rp = new TableRow.LayoutParams(TableRow.LayoutParams.MATCH_PARENT, TableRow.LayoutParams.MATCH_PARENT);
            rp.height = 60;
            rp.width = 60;
            row.setLayoutParams(tbl);
            chessBoard.addView(row, i);

            for (int j = 0; j < cols; j++) {
                tiles[i][j] = new Tile(this, i, j);
                if ((i + j) % 2 == 0) {
                    tiles[i][j].setBackgroundColor(Color.WHITE);
                }
                else {
                    tiles[i][j].setBackgroundColor(Color.argb(100, 151, 182, 181 ));
                }
                tiles[i][j].setLayoutParams(rp);
                tiles[i][j].setOnDragListener(this);

                Piece p = null;

                //Pawn placement
                if (i == 1) {
                    Drawable wpawn = getResources().getDrawable(R.drawable.wpawn);
                    p = new Piece (this, i, j, "wpawn");
                    p.setImageResource(R.drawable.wpawn);
                }
                else if(i == 6) {
                    p = new Piece (this, i, j, "bpawn");
                    p.setImageResource(R.drawable.bpawn);
                }
                //Rook
                else if(j == 7 && i == 7 || j == 0 && i == 7) {
                    p = new Piece (this, i, j, "brook");
                    p.setImageResource(R.drawable.brook);
                }
                else if(j == 7 && i == 0 || j == 0 && i == 0) {
                    p = new Piece (this, i, j, "wrook");
                    p.setImageResource(R.drawable.wrook);
                }
                //Knights
                else if(j == 6 && i == 7 || j == 1 && i == 7) {
                    p = new Piece (this, i, j, "bknight");
                    p.setImageResource(R.drawable.bknight);
                }
                else if(j == 6 && i == 0 || j == 1 && i == 0) {
                    p = new Piece (this, i, j, "wknight");
                    p.setImageResource(R.drawable.wknight);
                }
                //Bishops
                else if(j == 5 && i == 7 || j == 2 && i == 7) {
                    p = new Piece (this, i, j, "bbishop");
                    p.setImageResource(R.drawable.bbishop);
                }
                else if(j == 5 && i == 0 || j == 2 && i == 0) {
                    p = new Piece (this, i, j, "wbishop");
                    p.setImageResource(R.drawable.wbishop);
                }
                //Queen
                else if(j == 4 && i == 7) {
                    p = new Piece (this, i, j, "bqueen");
                    p.setImageResource(R.drawable.bqueen);
                }
                else if(j == 4 && i == 0 ) {
                    p = new Piece (this, i, j, "wqueen");
                    p.setImageResource(R.drawable.wqueen);
                }
                //Queen
                else if(j == 3 && i == 7) {
                    p = new Piece (this, i, j, "bking");
                    p.setImageResource(R.drawable.bking);
                }
                else if(j == 3 && i == 0 ) {
                    p = new Piece (this, i, j, "wking");
                    p.setImageResource(R.drawable.wking);
                }

                if (p != null) {
                    tiles[i][j].setPiece(p);
                    p.setOnTouchListener(this);
                }

                row.addView(tiles[i][j], j);
            }
        }
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

    /*


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

     */


    /*
    View.OnTouchListener ClickListener = new View.OnTouchListener(){
        @Override
        public boolean onTouch(View view, MotionEvent motionEvent) {
            //if (motionEvent.getAction() == MotionEvent.ACTION_DOWN){
                View.DragShadowBuilder shadowBuilder = new View.DragShadowBuilder(view);
                view.startDrag(null, shadowBuilder, view, 0);
                view.setVisibility(View.VISIBLE);
                return true;
           // } else {
           //     return false;
          //  }

        }
    };


    View.OnDragListener dragListener = new View.OnDragListener() {
        @Override
        public boolean onDrag(View view, DragEvent event) {
            View v = (View) event.getLocalState();
            int dragEvent = event.getAction();
            switch (dragEvent){
                case DragEvent.ACTION_DRAG_ENTERED:
                   break;
                case DragEvent.ACTION_DRAG_EXITED:
                    break;
                case DragEvent.ACTION_DROP:
                    ViewGroup owner = (ViewGroup) v.getParent();
                    owner.removeView(v);
                    LinearLayout container = (LinearLayout) v;
                    container.addView(view);
                    view.setVisibility(View.VISIBLE);
                   // view.animate()
                    //        .x(gir.getX())
                   //         .y(gir.getY())
                    //        .setDuration(700)
                    //        .start();
                    break;
            }

            return true;
        }
    };


     */
}

