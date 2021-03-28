package com.example.chessiegame;

import android.os.Bundle;

import androidx.constraintlayout.widget.ConstraintLayout;
import androidx.fragment.app.Fragment;

import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.LinearLayout;
import android.widget.TableLayout;
import android.widget.TableRow;

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
    private JSONArray gameList;

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
        fetchPastGames(user.getUid());

        return v;
    }

    public void fetchPastGames(String uid) {
        String url = "http://ec2-user@ec2-54-153-82-188.us-west-1.compute.amazonaws.com:3000/getallgames/" + uid;

        StringRequest stringRequest = new StringRequest(Request.Method.GET, url,
                response -> {
                    try {
                        gameList = new JSONArray(response);
                        renderPastGames();
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

    public void renderPastGames() {
        int length = gameList.length();
        int i = 0;
        TableRow row;
        String elem = "";

        for (int j = 0; j < length; j++) {
            try {
                elem = gameList.get(j).toString();
            } catch (JSONException e) {
                e.printStackTrace();
            }

            Log.d("Past Games Screen", elem);
        }

        while (i < length) {
            if (i % 2 == 0) { // start a new row
                row = new TableRow(getActivity());
                ConstraintLayout.LayoutParams tbl = (ConstraintLayout.LayoutParams) gameTable.getLayoutParams();
                tbl.width = tableWidth;
                tbl.height = tableHeight;
                row.setLayoutParams(tbl);
                gameTable.addView(row);
            }

            TableRow.LayoutParams rp = new TableRow.LayoutParams(TableRow.LayoutParams.MATCH_PARENT, TableRow.LayoutParams.MATCH_PARENT);
            rp.height = 160;
            rp.width = 125;
            rp.rightMargin = 15;

            //LinearLayout gameItem = new LinearLayout(getActivity());
            //gameItem.setLayoutParams(rp);
            i += 10; // remove later
        }
    }
}