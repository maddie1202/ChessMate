package com.example.chessiegame;

import android.graphics.Color;
import android.os.Build;
import android.os.Bundle;

import androidx.annotation.RequiresApi;
import androidx.fragment.app.Fragment;
import androidx.recyclerview.widget.LinearLayoutManager;
import androidx.recyclerview.widget.RecyclerView;

import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import androidx.cardview.widget.CardView;
import android.widget.TextView;
import android.widget.ImageView;
import android.widget.Button;
import android.content.res.ColorStateList;

import com.android.volley.Request;
import com.android.volley.RequestQueue;
import com.android.volley.toolbox.StringRequest;
import com.android.volley.toolbox.Volley;
import com.google.firebase.auth.FirebaseAuth;
import com.google.firebase.auth.FirebaseUser;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.util.ArrayList;
import java.util.List;

/**
 * A simple {@link Fragment} subclass.
 * Use the {@link AchievementsScreen#newInstance} factory method to
 * create an instance of this fragment.
 */
public class AchievementsScreen extends Fragment {

    // the fragment initialization parameters, e.g. ARG_ITEM_NUMBER
    private static final String ARG_PARAM1 = "param1";
    private static final String ARG_PARAM2 = "param2";

    private String mParam1;
    private String mParam2;
    public int id;

    private RequestQueue queue;
    private RecyclerView rv;
    private ArrayList<Achievement> items = new ArrayList<Achievement>();
    private ArrayList<Achievement> doneItems = new ArrayList<Achievement>();
    private ArrayList<Achievement> progressItems = new ArrayList<Achievement>();
    private Button progressBtn;
    private Button doneBtn;
    private boolean progressPressed; // 1 if progress pressed, 0 otherwise

    private FirebaseUser user;
    private FirebaseAuth mAuth;

    public AchievementsScreen() {
        // Required empty public constructor
    }

    /**
     * Use this factory method to create a new instance of
     * this fragment using the provided parameters.
     *
     * @param param1 Parameter 1.
     * @param param2 Parameter 2.
     * @return A new instance of fragment AchievementsScreen.
     */
    public static AchievementsScreen newInstance(String param1, String param2) {
        AchievementsScreen fragment = new AchievementsScreen();
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

    @RequiresApi(api = Build.VERSION_CODES.LOLLIPOP)
    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        // Inflate the layout for this fragment
        id = container.getId();
        View v = inflater.inflate(R.layout.fragment_achievements_screen, container, false);
        queue = Volley.newRequestQueue(getContext());

        rv = v.findViewById(R.id.achievements_list);
        LinearLayoutManager llm = new LinearLayoutManager(getActivity());
        rv.setLayoutManager(llm);

        progressBtn = v.findViewById(R.id.progress_button);
        doneBtn = v.findViewById(R.id.completed_button);
        progressBtn.setBackgroundTintList(ColorStateList.valueOf(Color.parseColor("#E5E5E5")));
        progressBtn.setElevation((float) -2.0);

        mAuth = FirebaseAuth.getInstance();
        user = mAuth.getCurrentUser();

        getStatusAchievements(true);
        getStatusAchievements(false);
        getAchievements(true);

        RVAdapter adapter = new RVAdapter(items);
        rv.setAdapter(adapter);

        progressBtn.setOnClickListener(new View.OnClickListener() {
            @RequiresApi(api = Build.VERSION_CODES.LOLLIPOP)
            @Override
            public void onClick(View v) {
                getAchievements(true);

                // select progress btn, unselect completed btn
                progressBtn.setBackgroundTintList(ColorStateList.valueOf(Color.parseColor("#E5E5E5")));
                progressBtn.setElevation((float) -2.0);

                doneBtn.setBackgroundTintList(ColorStateList.valueOf(Color.parseColor("#FFFFFF")));
                //doneBtn.setBackgroundColor(Color.parseColor("#FFFFFF"));
                doneBtn.setElevation((float) 5.0);

                adapter.notifyDataSetChanged();
            }
        });

        doneBtn.setOnClickListener(new View.OnClickListener() {
            @RequiresApi(api = Build.VERSION_CODES.LOLLIPOP)
            @Override
            public void onClick(View v) {
                getAchievements(false);

                // select completed btn, unselect progress btn
                doneBtn.setBackgroundTintList(ColorStateList.valueOf(Color.parseColor("#E5E5E5")));
                doneBtn.setElevation((float) -2.0);

                progressBtn.setBackgroundTintList(ColorStateList.valueOf(Color.parseColor("#FFFFFF")));
                progressBtn.setElevation((float) 5.0);

                adapter.notifyDataSetChanged();
            }
        });

        return v;
    }

    /**
     * Gets the status of all the user achievements and classifies them as either completed or in-progress
     */
    private void getStatusAchievements(boolean progressSelected) {
        String url = "http://ec2-user@ec2-54-153-82-188.us-west-1.compute.amazonaws.com:3000/getgoals" + user.getUid();

        StringRequest stringRequest = new StringRequest(Request.Method.GET, url,
                response -> {
                    try {
                        JSONArray arr = new JSONArray(response);
                        for (int i = 0; i < arr.length(); i++) {
                            JSONObject elem = arr.getJSONObject(i);
                            int reqCount = (int) elem.get("reqCount");
                            int realCount = (int) elem.get("realCount");
                            int difficulty = (int) elem.get("difficulty");
                            String level = (difficulty == 1) ? "easy" :
                                           (difficulty == 2) ? "medium" : "hard";
                            boolean status = reqCount == realCount;
                            if (status) {
                                doneItems.add(new Achievement("Win " + reqCount + " " + level + " games", status));
                            } else {
                                progressItems.add(new Achievement("Win " + reqCount + " " + level + " games", status));
                            }
                        }
                    } catch (JSONException e) {
                        e.printStackTrace();
                    }
                },
                error -> {
                    Log.d("AchievementScreen", "Error fetching goals");
                });

        // Add the request to the RequestQueue.
        queue.add(stringRequest);
    }

    /**
     * Populate the list of achievements to be displayed, depending on their status
     */
    private void getAchievements(boolean progressSelected) {
        items.clear();
        if (progressSelected) {
            items.addAll(progressItems);
        } else {
            items.addAll(doneItems);
        }
    }

    /**
     * Custom Achievement class for RecyclerView
     */
    public class Achievement {
        String achievement;
        boolean done;
        int iconId;

        public Achievement(String achievement, boolean done) {
            this.achievement = achievement;
            this.done = done;
            this.iconId = done ? R.drawable.trophy_filled : R.drawable.trophy_outlined;
        }
    }

    public class RVAdapter extends RecyclerView.Adapter<RVAdapter.AchievementViewHolder> {

        public class AchievementViewHolder extends RecyclerView.ViewHolder {
            CardView cv;
            TextView task;
            TextView completed;
            ImageView trophy;

            AchievementViewHolder(View itemView) {
                super(itemView);
                cv = (CardView) itemView.findViewById(R.id.cv);
                task = (TextView) itemView.findViewById(R.id.task);
                trophy = (ImageView) itemView.findViewById(R.id.trophy);
            }

        }

        List<Achievement> achs;

        RVAdapter(List<Achievement> achievements){
            this.achs = achievements;
        }

        @Override
        public int getItemCount() {
            return achs.size();
        }

        @Override
        public AchievementViewHolder onCreateViewHolder(ViewGroup viewGroup, int i) {
            View v = LayoutInflater.from(viewGroup.getContext()).inflate(R.layout.achievement_item, viewGroup, false);
            AchievementViewHolder avh = new AchievementViewHolder(v);
            return avh;
        }

        @Override
        public void onBindViewHolder(AchievementViewHolder avh, int i) {
            avh.task.setText(achs.get(i).achievement);
            avh.trophy.setImageResource(achs.get(i).iconId);
        }

        @Override
        public void onAttachedToRecyclerView(RecyclerView recyclerView) {
            super.onAttachedToRecyclerView(recyclerView);
        }
    }

}