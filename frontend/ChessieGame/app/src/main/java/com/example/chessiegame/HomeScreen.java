package com.example.chessiegame;

import android.content.Intent;
import android.os.Build;
import android.os.Bundle;

import androidx.annotation.RequiresApi;
import androidx.fragment.app.Fragment;

import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

import com.google.firebase.auth.FirebaseAuth;
import com.google.firebase.auth.FirebaseUser;
import com.android.volley.RequestQueue;
import com.android.volley.toolbox.Volley;
import com.android.volley.toolbox.StringRequest;
import com.android.volley.Request;
import com.android.volley.Response;
import com.android.volley.VolleyError;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.sql.Timestamp;
import java.util.Date;

/**
 * A simple {@link Fragment} subclass.
 * Use the {@link HomeScreen#newInstance} factory method to
 * create an instance of this fragment.
 */
public class HomeScreen extends Fragment {

    // TODO: Rename parameter arguments, choose names that match
    // the fragment initialization parameters, e.g. ARG_ITEM_NUMBER
    private static final String ARG_PARAM1 = "param1";
    private static final String ARG_PARAM2 = "param2";
    private String mParam1;
    private String mParam2;


    private FirebaseUser user;
    private FirebaseAuth mAuth;
    private RequestQueue queue;
    private int gameID;
    public String boardString;
    private final int size = 8;

    public int id;
    private Button start;
    private Button resume;

    public HomeScreen() {
        // Required empty public constructor
    }

    /**
     * Use this factory method to create a new instance of
     * this fragment using the provided parameters.
     *
     * @param param1 Parameter 1.
     * @param param2 Parameter 2.
     * @return A new instance of fragment HomeScreen.
     */
    // TODO: Rename and change types and number of parameters
    public static HomeScreen newInstance(String param1, String param2) {
        HomeScreen fragment = new HomeScreen();
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

        View v = inflater.inflate(R.layout.fragment_home_screen, container, false);

        start = v.findViewById(R.id.start_new_game);
        resume = v.findViewById(R.id.resume_previous);

        gameID = 0;
        queue = Volley.newRequestQueue(getActivity());
        mAuth = FirebaseAuth.getInstance();
        user = mAuth.getCurrentUser();

        start.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent(getActivity(), PopDifficulty.class);
                intent.addFlags(Intent.FLAG_ACTIVITY_NO_ANIMATION);
                startActivity(intent);
            }
        });

        resume.setOnClickListener(new View.OnClickListener() {
            @RequiresApi(api = Build.VERSION_CODES.KITKAT)
            @Override
            public void onClick(View v) {
                getLatestGame(user.getUid());
            }
        });

        return v;
    }

    @RequiresApi(api = Build.VERSION_CODES.KITKAT)
    private void getLatestGame(String uid) {
        String url = "http://ec2-user@ec2-54-153-82-188.us-west-1.compute.amazonaws.com:3000/getallgames/" + uid;

        StringRequest stringRequest = new StringRequest(Request.Method.GET, url,
                response -> {
                    try {
                        JSONArray jsonArray = new JSONArray(response);

                        // find the most recent game
                        for (int i = 0; i < jsonArray.length(); i++) {
                            JSONObject j = jsonArray.getJSONObject(i);
                            if ((int) j.get("gameID") > gameID && (int) j.get("result") == -1) {
                                gameID = (int) j.get("gameID");
                            }
                        }

                        // get the result of the most recent game
                        if (gameID == 0) {
                            showToast("No Games Currently in Progress");
                        } else {
                            getLatestBoard(gameID);
                        }

                    } catch (JSONException e) {
                        e.printStackTrace();
                    }
                },
                error -> {
                    Log.d("ChessScreen", "Error fetching most recent game");
                });

        // Add the request to the RequestQueue.
        queue.add(stringRequest);
    }

    public void getLatestBoard(int gameID) {
        String url = "http://ec2-user@ec2-54-153-82-188.us-west-1.compute.amazonaws.com:3000/getgame/" + gameID;

        StringRequest stringRequest = new StringRequest(Request.Method.GET, url,
                response -> {
                    try {
                        // Gets the board with the highest sequence number from game with gameID to render
                        int maxNum = 0;
                        JSONArray jsonArray = new JSONArray(response);
                        for (int i = 0; i < jsonArray.length(); i++) {
                            JSONObject j = jsonArray.getJSONObject(i);
                            // get the max sequenceNumber to find the most recent board
                            if ((int) j.get("boardID") > maxNum) {
                                maxNum = (int) j.get("boardID");
                                boardString = (String) j.get("placements");
                            }
                        }

                        getGameDetails(gameID);

                    } catch (JSONException e) {
                        e.printStackTrace();
                    }
                },
                error -> {
                    Log.d("ChessScreen", "Error fetching most recent board");
                });

        // Add the request to the RequestQueue.
        queue.add(stringRequest);
    }

    public void getGameDetails(int gameID) {
        String url = "http://ec2-user@ec2-54-153-82-188.us-west-1.compute.amazonaws.com:3000/getgamedetails/" + gameID;

        StringRequest stringRequest = new StringRequest(Request.Method.GET, url,
                response -> {
                    try {
                        // Gets the board with the highest sequence number from game with gameID to render
                        JSONArray arr = new JSONArray(response);
                        JSONObject res = (JSONObject) arr.get(0);

                        // finally navigate to Resume Game PopUp
                        Intent intent = new Intent(getActivity(), ResumeGamePopUp.class);
                        intent.putExtra("gameID", gameID);
                        int[][] layout = parseBoard(boardString);

                        intent.putExtra("resumedLayout", layout);
                        intent.putExtra("difficulty", (int) res.get("difficulty"));
                        startActivity(intent);

                        Log.d("HomeScreen", response);
                    } catch (JSONException e) {
                        e.printStackTrace();
                    }
                },
                error -> {
                    Log.d("ChessScreen", "Error fetching most recent game's details");
                });

        // Add the request to the RequestQueue.
        queue.add(stringRequest);
    }

    public int[][] parseBoard(String b) {
        int[][] layout = new int[size][size];
        String[] boardString = b.split("\\s+");

        for (int j = 0; j < size; j++) {
            for (int k = 0; k < size; k++) {
                layout[j][k] = Integer.parseInt(boardString[j * size + k]);
            }
        }

        return layout;
    }

    private void showToast(String msg) {
        Toast.makeText(getActivity(), msg, Toast.LENGTH_SHORT).show();
    }

}