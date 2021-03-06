package com.example.chessiegame;

import android.content.Intent;
import android.os.Bundle;

import androidx.fragment.app.Fragment;
import androidx.gridlayout.widget.GridLayout;

import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.view.ViewGroup.LayoutParams;
import android.view.Gravity;
import android.widget.ImageView;
import android.widget.Button;
import android.widget.TextView;

import com.android.volley.Request;
import com.android.volley.RequestQueue;
import com.android.volley.Response;
import com.android.volley.VolleyError;
import com.android.volley.toolbox.StringRequest;
import com.android.volley.toolbox.Volley;
import com.bumptech.glide.Glide;

import com.google.android.gms.auth.api.signin.GoogleSignIn;
import com.google.android.gms.auth.api.signin.GoogleSignInOptions;
import com.google.firebase.auth.FirebaseAuth;
import com.google.firebase.auth.FirebaseUser;

import org.json.JSONArray;
import org.json.JSONException;

import java.util.ArrayList;

/**
 * A simple {@link Fragment} subclass.
 * Use the {@link AccountScreen#newInstance} factory method to
 * create an instance of this fragment.
 */
public class AccountScreen extends Fragment {

    // the fragment initialization parameters, e.g. ARG_ITEM_NUMBER
    private static final String ARG_PARAM1 = "param1";
    private static final String ARG_PARAM2 = "param2";
    private Button signOutBtn;
    private FirebaseUser user;
    private FirebaseAuth mAuth;
    private ImageView profile;
    private GridLayout grid;
    private ArrayList<String> gridArray;

    private String mParam1;
    private String mParam2;
    public int id;

    public AccountScreen() {
        // Required empty public constructor
    }

    /**
     * Use this factory method to create a new instance of
     * this fragment using the provided parameters.
     *
     * @param param1 Parameter 1.
     * @param param2 Parameter 2.
     * @return A new instance of fragment AccountScreen.
     */
    public static AccountScreen newInstance(String param1, String param2) {
        AccountScreen fragment = new AccountScreen();
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
        View v = inflater.inflate(R.layout.fragment_account_screen, container, false);

        mAuth = FirebaseAuth.getInstance();
        user = mAuth.getCurrentUser();
        profile = v.findViewById(R.id.accountPic);
        signOutBtn = v.findViewById(R.id.signOutBtn);

        Glide.with(this).load(user.getPhotoUrl()).into(profile);

        grid = v.findViewById(R.id.account_grid);
        gridArray = new ArrayList<String>();

        RequestQueue queue = Volley.newRequestQueue(getActivity());
        getPastGames(queue, "http://ec2-54-153-82-188.us-west-1.compute.amazonaws.com:3000/getallgames/" + user.getUid());

        if (!gridArray.isEmpty()) {
            TextView titleText;
            for (int i = 0; i < gridArray.size(); i++) {
                titleText = new TextView(getContext());
                titleText.setText(gridArray.get(i));
                grid.addView(titleText, i);

                GridLayout.LayoutParams param = new GridLayout.LayoutParams();
                param.height = LayoutParams.WRAP_CONTENT;
                param.width = LayoutParams.WRAP_CONTENT;
                param.setGravity(Gravity.CENTER);
                titleText.setLayoutParams (param);
            }
        }

        signOutBtn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                mAuth.signOut();
                GoogleSignIn.getClient(
                        getContext(),
                        new GoogleSignInOptions.Builder(GoogleSignInOptions.DEFAULT_SIGN_IN).build()
                ).signOut();
                Intent intent = new Intent(getActivity(), MainActivity.class);
                startActivity(intent);
            }
        });

        return v;
    }

    /**
     * Queries the database for all of the user's past games
     */
    public void getPastGames(RequestQueue queue, String url) {
        StringRequest stringRequest = new StringRequest(Request.Method.GET, url,
                new Response.Listener<String>() {
                    @Override
                    public void onResponse(String response) {
                        try {
                            JSONArray jsonArray = new JSONArray(response);
                            gridArray.clear();
                            for (int i = 0; i < jsonArray.length(); i++) {
                                gridArray.add(jsonArray.getString(i));
                            }
                        } catch (JSONException e) {
                            e.printStackTrace();
                        }

                    }
                }, new Response.ErrorListener() {
            @Override
            public void onErrorResponse(VolleyError error) {
                Log.d("AccountScreen", "Could not pull account details");
            }
        });

        // Add the request to the RequestQueue.
        queue.add(stringRequest);
    }
}