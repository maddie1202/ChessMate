package com.example.chessiegame;

import androidx.appcompat.app.AppCompatActivity;

import android.content.ClipData;
import android.graphics.Canvas;
import android.os.Bundle;
import android.view.DragEvent;
import android.view.MotionEvent;
import android.view.View;
import android.view.ViewGroup;
import android.widget.GridLayout;
import android.widget.ImageView;
import android.widget.RelativeLayout;
import android.widget.TextView;

import com.example.chessiegame.components.Board;

public class ChessScreen extends AppCompatActivity {

    /*
    TextView piece, place;
    GridLayout gir;
    */

    Board board_view;



    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        board_view = new Board(this);

        setContentView(board_view);

        /*
        piece = (TextView) findViewById(R.id.pawn_queen);
        gir = (GridLayout) findViewById(R.id.grid);

        //Test piece , delete later
        place = (TextView) findViewById(R.id.P00);


        piece.setOnLongClickListener(longClickListener);
        gir.setOnDragListener(dragListener);
        place.setOnDragListener(dragListener);
        */
    }




        /*
    View.OnLongClickListener longClickListener = new View.OnLongClickListener(){
        @Override
        public boolean onLongClick(View view) {
            ClipData data = ClipData.newPlainText("","");
            View.DragShadowBuilder myShadoew = new View.DragShadowBuilder(view);
            view.startDrag(data, myShadoew, view, 0);
            return true;
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
                    view.animate()
                            .x(gir.getX())
                            .y(gir.getY())
                            .setDuration(700)
                            .start();
                    break;
            }

            return true;
        }
    };

`
     */


}

