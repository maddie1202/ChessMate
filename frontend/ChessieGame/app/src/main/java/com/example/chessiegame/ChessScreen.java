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

public class ChessScreen extends AppCompatActivity {

    TextView piece;
    GridLayout gir;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_chess_screen);

        piece = (TextView) findViewById(R.id.pawn_queen);
        gir = (GridLayout) findViewById(R.id.grid);


        gir.setOnDragListener(dragListener);

        piece.setOnLongClickListener(longClickListener);

    }

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

            int dragEvent = event.getAction();
            switch (dragEvent){
                case DragEvent.ACTION_DRAG_ENTERED:

                    final View v = (View) event.getLocalState();

                   break;
                case DragEvent.ACTION_DRAG_EXITED:
                    break;
                case DragEvent.ACTION_DROP:
                    break;
            }

            return true;
        }
    };



}

