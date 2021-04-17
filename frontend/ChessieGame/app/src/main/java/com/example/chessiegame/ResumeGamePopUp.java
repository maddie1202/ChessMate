package com.example.chessiegame;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.ImageButton;
import android.widget.TextView;

public class ResumeGamePopUp extends Activity {

    public int gameID;
    public int difficulty;
    public int[][] layout;
    public int sequenceNum;
    public int timeLeft;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        setContentView(R.layout.activity_resume_game_pop_up);
        DisplayMetrics dm = new DisplayMetrics();
        getWindowManager().getDefaultDisplay().getMetrics(dm);
        int width = dm.widthPixels;
        int height = dm.heightPixels;

        getWindow().setLayout((int) (width*.9), (int) (height*.9));

        gameID = getIntent().getIntExtra("gameID", 0);
        difficulty = getIntent().getIntExtra("difficulty", 0);
        layout = (int[][]) getIntent().getSerializableExtra("resumedLayout");
        sequenceNum = getIntent().getIntExtra("sequenceNum", 0);
        timeLeft = getIntent().getIntExtra("timeLeft", 0);

        ImageButton closeButton = (ImageButton) findViewById(R.id.close_button2);
        Button startGame = (Button) findViewById(R.id.start_button2);
        TextView game = (TextView)findViewById(R.id.gameIDresume);
        String bin = Integer.toBinaryString(gameID);
        game.setText("The game id is " + bin);

        closeButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent(ResumeGamePopUp.this, HomeActivity.class);
                intent.addFlags(Intent.FLAG_ACTIVITY_NO_ANIMATION);
                startActivity(intent);
                overridePendingTransition(0,0);
            }
        });

        startGame.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if (gameID != 0 && difficulty != 0 && layout.length == 8) {
                    navigateToChess(gameID, difficulty);
                } else {
                    Log.d("ResumeGamePopUp", "Unable to resume game");
                }
            }
        });
    }

    /**
     * Sends information and navigates to the chess screen
     */
    private void navigateToChess(int gameID, int difficulty) {
        Intent intent = new Intent(ResumeGamePopUp.this, ChessScreen.class);
        //intent.addFlags(Intent.FLAG_ACTIVITY_NO_ANIMATION);
        intent.putExtra("gameID", gameID);
        intent.putExtra("resumedLayout", layout);
        intent.putExtra("newGame", false);
        intent.putExtra("difficulty", difficulty);
        intent.putExtra("sequenceNum", sequenceNum);
        intent.putExtra("timeLeft", timeLeft);
        startActivity(intent);
        overridePendingTransition(0,0);
    }
}