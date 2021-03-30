package com.example.chessiegame;

import android.content.Intent;
import android.graphics.Color;
import android.graphics.drawable.Drawable;
import android.os.Bundle;

import androidx.constraintlayout.widget.ConstraintLayout;
import androidx.fragment.app.Fragment;

import android.util.Log;
import android.view.Gravity;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.FrameLayout;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.ScrollView;
import android.widget.TableLayout;
import android.widget.TableRow;
import android.widget.TextView;

import com.android.volley.Request;
import com.android.volley.RequestQueue;
import com.android.volley.toolbox.StringRequest;
import com.android.volley.toolbox.Volley;
import com.google.firebase.auth.FirebaseAuth;
import com.google.firebase.auth.FirebaseUser;
import com.google.gson.JsonArray;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.sql.Timestamp;
import java.util.ArrayList;
import java.util.Date;
import java.util.HashMap;
import java.util.Map;
import java.util.stream.Collectors;

/**
 * A simple {@link Fragment} subclass.
 * Use the {@link PastGamesScreen#newInstance} factory method to
 * create an instance of this fragment.
 */
public class PastGamesScreen extends Fragment {

    // TODO: Rename parameter arguments, choose names that match
    // the fragment initialization parameters, e.g. ARG_ITEM_NUMBER
    private static final String ARG_PARAM1 = "param1";
    private static final String ARG_PARAM2 = "param2";

    // TODO: Rename and change types of parameters
    private String mParam1;
    private String mParam2;
    public int id;

    private FirebaseAuth mAuth;
    private FirebaseUser user;
    private RequestQueue queue;
    private ArrayList<Integer> gameIDList;
    private ArrayList<PastGame> gameList;
    private final Map<String, String> monthMap = new HashMap<String, String>() {{
        put("01", "January");
        put("02", "February");
        put("03", "March");
        put("04", "April");
        put("05", "May");
        put("06", "June");
        put("07", "July");
        put("08", "August");
        put("09", "September");
        put("10", "October");
        put("11", "November");
        put("12", "December");
    }};

    TableLayout gameTable;
    private int tableHeight;
    private int tableWidth;

    public PastGamesScreen() {
        // Required empty public constructor
    }

    /**
     * Use this factory method to create a new instance of
     * this fragment using the provided parameters.
     *
     * @param param1 Parameter 1.
     * @param param2 Parameter 2.
     * @return A new instance of fragment PastGamesScreen.
     */
    // TODO: Rename and change types and number of parameters
    public static PastGamesScreen newInstance(String param1, String param2) {
        PastGamesScreen fragment = new PastGamesScreen();
        Bundle args = new Bundle();
        args.putString(ARG_PARAM1, param1);
        args.putString(ARG_PARAM2, param2);
        fragment.setArguments(args);
        return fragment;
    }

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        if (getArguments() != null) {
            mParam1 = getArguments().getString(ARG_PARAM1);
            mParam2 = getArguments().getString(ARG_PARAM2);
        }
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        // Inflate the layout for this fragment
        id = container.getId();
        View v = inflater.inflate(R.layout.fragment_past_games_screen, container, false);
        gameTable = v.findViewById(R.id.pastGamesTable);
        tableHeight = gameTable.getHeight();
        tableWidth = gameTable.getWidth();

        queue = Volley.newRequestQueue(getActivity());
        mAuth = FirebaseAuth.getInstance();
        user = mAuth.getCurrentUser();
        gameList = new ArrayList<>();
        gameIDList = new ArrayList<>();

        //TODO: change this back to user.getUid() for final - leave this for now
        fetchPastGames("xQYSsLmZ8JU6jCNL1kL7g7QcDqE3");

        return v;
    }

    public void fetchPastGames(String uid) {
        String url = "http://ec2-user@ec2-54-153-82-188.us-west-1.compute.amazonaws.com:3000/getallgames/" + uid;

        StringRequest stringRequest = new StringRequest(Request.Method.GET, url,
                response -> {
                    try {
                        JSONArray arr = new JSONArray(response);
                        for (int i = 0; i < arr.length(); i++) {
                            int id = (int) arr.getJSONObject(i).get("gameID");
                            gameIDList.add(id);
                            fetchGameDetails(id, i, arr.length() - 1);
                        }
                    } catch (JSONException e) {
                        e.printStackTrace();
                    }
                },
                error -> {
                    Log.d("ChessScreen", "Error fetching all games");
                });

        // Add the request to the RequestQueue.
        queue.add(stringRequest);
    }

    public void fetchGameDetails(int gameID, int gameIndex, int numGames) {
        String url = "http://ec2-user@ec2-54-153-82-188.us-west-1.compute.amazonaws.com:3000/getgamedetails/" + gameID;

        StringRequest stringRequest = new StringRequest(Request.Method.GET, url,
                response -> {
                    try {
                        JSONArray arr = new JSONArray(response);
                        JSONObject res = arr.getJSONObject(0);
                        String date = res.get("startDateTime").toString();
                        gameList.add(new PastGame(gameID, date.substring(5, 7), date.substring(8, 10), date.substring(11, 16)));

                        if (gameIndex == numGames) {
                            renderPastGames();
                        }
                    } catch (JSONException e) {
                        e.printStackTrace();
                    }
                },
                error -> {
                    Log.d("ChessScreen", "Error fetching most game details");
                });

        // Add the request to the RequestQueue.
        queue.add(stringRequest);
    }

    public void renderPastGames() {
        int i = 0;
        int rowNum = 0;
        int colNum = 0;
        TableRow row = new TableRow(getActivity());
        int length = gameList.size();

        while (i < length) {
            if (i % 2 == 0) { // start a new row
                row = new TableRow(getActivity());
                ScrollView.LayoutParams tbl = (ScrollView.LayoutParams) gameTable.getLayoutParams();
                row.setLayoutParams(tbl);
                gameTable.addView(row, rowNum);
                rowNum++;
                colNum = 0;
            }

            TableRow.LayoutParams rp = new TableRow.LayoutParams(TableRow.LayoutParams.MATCH_PARENT, TableRow.LayoutParams.MATCH_PARENT);
            rp.height = 430;
            rp.width = 380;
            rp.rightMargin = 10;
            rp.leftMargin = 10;
            rp.topMargin = 20;
            rp.gravity = Gravity.CENTER;
            rp.gravity = Gravity.CENTER_HORIZONTAL;

            LinearLayout gameItem = new LinearLayout(getActivity());
            gameItem.setOrientation(LinearLayout.VERTICAL);
            gameItem.setLayoutParams(rp);

            PastGame g = gameList.get(i);
            ImageView chessImage = new ImageView(getActivity());
            chessImage.setImageResource(R.drawable.chessimage);
            chessImage.setMaxWidth(360);
            chessImage.setMaxHeight(360);
            chessImage.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View v) {
                    fetchGameBoards(g.gameID);
                }
            });

            gameItem.addView(chessImage, 0);

            TextView gameDate = new TextView(getActivity());
            Log.d("Past Games Screen", g.date);

            LinearLayout.LayoutParams rp2 = new LinearLayout.LayoutParams(TableRow.LayoutParams.MATCH_PARENT, TableRow.LayoutParams.MATCH_PARENT);
            //rp2.setMargins(5, 5, 5, 5);
            //rp2.gravity = Gravity.CENTER;
            //rp2.gravity = Gravity.CENTER_HORIZONTAL;

            //gameDate.setLayoutParams(new LinearLayout.LayoutParams(LinearLayout.LayoutParams.MATCH_PARENT, LinearLayout.LayoutParams.MATCH_PARENT));
            gameDate.setText(g.date); // g.date is date that the game was played
            gameDate.setTextColor(Color.BLACK);
            gameDate.setTextSize(10);
            //gameDate.setPadding(5,5,5,5);
            //rp2.setMargins(5,5,5,5);
            gameDate.setLayoutParams(rp2);
            //gameItem.addView(gameDate, 1);
            gameItem.addView(gameDate, 1);

            row.addView(gameItem, colNum);
            i++;
            colNum++;
        }
    }

    public void fetchGameBoards(int gameID) {
        String url = "http://ec2-user@ec2-54-153-82-188.us-west-1.compute.amazonaws.com:3000/getgame/" + gameID;

        StringRequest stringRequest = new StringRequest(Request.Method.GET, url,
                response -> {
                    try {
                        JSONArray arr = new JSONArray(response);
                        // TODO: sort arr by increasing boardID, get string placements in that order, map to int array
                        // TODO: figure out decoding of boards
                        for (int i = 0; i < arr.length(); i++) {
                            JSONObject elem = arr.getJSONObject(i);
                        }

                        Intent intent = new Intent(getActivity(), ReplayPastGamesScreen.class);
                        intent.putExtra("gameID", gameID);
                        startActivity(intent);
                    } catch (JSONException e) {
                        e.printStackTrace();
                    }
                },
                error -> {
                    Log.d("ChessScreen", "Error fetching most game details");
                });

        // Add the request to the RequestQueue.
        queue.add(stringRequest);
    }

    class PastGame {
        String year = "2021";
        String timeOfDay;
        String date;
        int gameID;

        public PastGame(int gameID, String month, String day, String time) {
            this.timeOfDay = time;
            this.gameID = gameID;
            this.date = monthMap.get(month) + " " + day + " 2021";
        }
    }
}