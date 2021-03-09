package com.example.chessiegame;

import android.os.Bundle;

import androidx.fragment.app.Fragment;
import androidx.recyclerview.widget.LinearLayoutManager;
import androidx.recyclerview.widget.RecyclerView;

import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import androidx.cardview.widget.CardView;
import android.widget.TextView;
import android.widget.ImageView;

import java.util.ArrayList;
import java.util.List;

/**
 * A simple {@link Fragment} subclass.
 * Use the {@link AchievementsScreen#newInstance} factory method to
 * create an instance of this fragment.
 */
public class AchievementsScreen extends Fragment {

    // TODO: Rename parameter arguments, choose names that match
    // the fragment initialization parameters, e.g. ARG_ITEM_NUMBER
    private static final String ARG_PARAM1 = "param1";
    private static final String ARG_PARAM2 = "param2";

    // TODO: Rename and change types of parameters
    private String mParam1;
    private String mParam2;
    public int id;

    private RecyclerView rv;
    private ArrayList<Achievement> items;

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
    // TODO: Rename and change types and number of parameters
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

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        // Inflate the layout for this fragment
        id = container.getId();
        View v = inflater.inflate(R.layout.fragment_achievements_screen, container, false);

        rv = v.findViewById(R.id.achievements_list);
        LinearLayoutManager llm = new LinearLayoutManager(getActivity());
        rv.setLayoutManager(llm);

        getAchievements();

        RVAdapter adapter = new RVAdapter(items);
        rv.setAdapter(adapter);

        return v;
    }

    public void getAchievements() {
        items = new ArrayList<Achievement>();
        items.add(new Achievement("Win 3 easy games", false));
        items.add(new Achievement("Win 3 medium games", false));
        items.add(new Achievement("Win 3 hard games", false));
    }

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