package com.example.chessiegame;



import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.util.DisplayMetrics;
import android.view.View;
import android.widget.Button;
import android.widget.ImageButton;
import android.widget.TextView;

public class ResumeGamePopUp extends Activity {

    public int gameID;
    public int difficulty;
    public int[][] layout;

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

        // TODO: show gameID in binary, get user to set switches, then press KEY2, then press start

        /* When navigating to chessscreen:
        Intent intent = new Intent(getActivity(), ChessScreen.class);
        intent.putExtra("gameID", gameID);
        intent.putExtra("resumedLayout", layout);
        intent.putExtra("newGame", false);
        intent.putExtra("difficulty", difficulty);
        startActivity(intent);
        */

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
                if (gameID != -1 && difficulty != -1) {
                    navigateToChess(gameID, difficulty);
                }
            }
        });
    }

    private void navigateToChess(int gameID, int difficulty) {
        Intent intent = new Intent(ResumeGamePopUp.this, ChessScreen.class);
        //intent.addFlags(Intent.FLAG_ACTIVITY_NO_ANIMATION);
        intent.putExtra("gameID", gameID);
        //intent.putExtra("resumedLayout", layout);
        intent.putExtra("newGame", false);
        intent.putExtra("difficulty", difficulty);
        startActivity(intent);
        overridePendingTransition(0,0);
    }
}