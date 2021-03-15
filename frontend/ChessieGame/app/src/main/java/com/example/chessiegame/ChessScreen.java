package com.example.chessiegame;

import androidx.annotation.RequiresApi;
import androidx.appcompat.app.AppCompatActivity;

import android.bluetooth.BluetoothDevice;
import android.content.ClipData;
import android.content.Intent;
import android.graphics.Canvas;
import android.os.Build;
import android.os.Bundle;
import android.view.DragEvent;
import android.view.MotionEvent;
import android.view.View;
import android.view.ViewGroup;
import android.widget.GridLayout;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.RelativeLayout;
import android.widget.TextView;
import android.bluetooth.BluetoothAdapter;
import android.widget.Toast;

import com.example.chessiegame.components.Board;

import java.util.Set;

public class ChessScreen extends AppCompatActivity {

    /*
    TextView piece, place;
    GridLayout gir;
    */
    private static final int REQUEST_ENABLE_BT = 0;
    private static final int REQUEST_DISCOVER_BT = 1;

    Board board_view;
    BluetoothAdapter mBlueAdapter;
    TextView paired_devices;
    boolean err;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        board_view = new Board(this);
        setContentView(board_view);
        /*
        for(int i = 0; i < 8; i++){
            for(int j = 0; j < 8; j++){
                board_view.piece_board[i][j].setOnTouchListener(ClickListener);

            }
        }

         */
        //piece.setOnLongClickListener(longClickListener);
        //gir.setOnDragListener(dragListener);
        //place.setOnDragListener(dragListener);

        mBlueAdapter = BluetoothAdapter.getDefaultAdapter();
        paired_devices = (TextView) findViewById(R.id.paired_devices);
        err = false;

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

