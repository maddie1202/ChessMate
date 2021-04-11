package com.example.chessiegame;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;

public class ResumeGamePopUp extends AppCompatActivity {

    public int gameID;
    public int difficulty;
    public int[][] layout;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_resume_game_pop_up);

        gameID = getIntent().getIntExtra("gameID", 0);
        difficulty = getIntent().getIntExtra("difficulty", 0);
        layout = (int[][]) getIntent().getSerializableExtra("resumedLayout");

        // TODO: show gameID in binary, get user to set switches, then press KEY2, then press start

        /* When navigating to chessscreen:
        Intent intent = new Intent(getActivity(), ChessScreen.class);
        intent.putExtra("gameID", gameID);
        intent.putExtra("resumedLayout", layout);
        intent.putExtra("difficulty", difficulty);
        startActivity(intent);
        */
    }

}