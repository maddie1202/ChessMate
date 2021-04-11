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

import com.android.volley.Request;
import com.android.volley.RequestQueue;
import com.android.volley.toolbox.JsonObjectRequest;
import com.android.volley.toolbox.Volley;

import org.json.JSONException;
import org.json.JSONObject;


public class PopDifficulty extends Activity {

    public int gameID;
    public int skill;
    private RequestQueue queue;

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        setContentView(R.layout.fragment_difficulty_set);
        DisplayMetrics dm = new DisplayMetrics();
        getWindowManager().getDefaultDisplay().getMetrics(dm);
        queue = Volley.newRequestQueue(this);

        int width = dm.widthPixels;
        int height = dm.heightPixels;
        gameID = -1;
        skill = -1;

        getWindow().setLayout((int) (width*.9), (int) (height*.9));

        ImageButton closeButton = (ImageButton) findViewById(R.id.close_button);
        Button startGame = (Button) findViewById(R.id.start_button);
        Button easyGame = (Button) findViewById(R.id.easy_button);
        Button medGame = (Button) findViewById(R.id.medium_button);
        Button hardGame = (Button) findViewById(R.id.hard_button);

        closeButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent(PopDifficulty.this, HomeActivity.class);
                intent.addFlags(Intent.FLAG_ACTIVITY_NO_ANIMATION);
                startActivity(intent);
                overridePendingTransition(0,0);
            }
        });

        startGame.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if (gameID != -1 && skill != -1) {
                    navigateToChess(gameID, skill);
                }
            }
        });

        easyGame.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                postNewGame(1);
            }
        });

        medGame.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                postNewGame(2);
            }
        });

        hardGame.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                postNewGame(3);
            }
        });
    }

    /**
     * Posts a new game with specified difficulty to current user
     */
    public void postNewGame(int difficulty) {
        String url = "http://ec2-user@ec2-54-153-82-188.us-west-1.compute.amazonaws.com:3000/makegame";
        JSONObject postData = new JSONObject();
        try {
            postData.put("difficulty", difficulty);
            postData.put("timeleft", 600000);
        } catch (JSONException e) {
            e.printStackTrace();
        }

        JsonObjectRequest jsonObjectRequest = new JsonObjectRequest(Request.Method.POST, url, postData,
                response -> {
                    Log.d("Difficulty Popup", "Successfully posted game");
                    JSONObject res = (JSONObject) response;
                    try {
                        gameID = Integer.parseInt(res.get("gameID").toString());
                        skill = difficulty;

                        // TODO: prompt user to enter gameID on switches and key with textViews

                       TextView game = (TextView)findViewById(R.id.gameID);
                       String bin = Integer.toBinaryString(gameID);
                       game.setText("The game id is " + bin);
                        // they will then press the start button on app to begin the game

                    } catch (JSONException e) {
                        e.printStackTrace();
                    }
                    Log.d("Difficulty Popup", String.valueOf(gameID));
                }, error -> {
            Log.d("Difficulty Popup", "Failed to post game");
        });

        queue.add(jsonObjectRequest);
    }

    private void navigateToChess(int gameID, int difficulty) {
        Intent intent = new Intent(PopDifficulty.this, ChessScreen.class);
        intent.addFlags(Intent.FLAG_ACTIVITY_NO_ANIMATION);
        intent.putExtra("newGame", true);
        intent.putExtra("difficulty", difficulty);
        intent.putExtra("gameID", gameID);
        startActivity(intent);
        overridePendingTransition(0,0);
    }
}
