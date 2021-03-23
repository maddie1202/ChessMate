package com.example.chessiegame;

import androidx.annotation.NonNull;
import androidx.appcompat.app.ActionBarDrawerToggle;
import androidx.appcompat.app.AppCompatActivity;
import androidx.appcompat.widget.Toolbar;
import androidx.core.view.GravityCompat;
import androidx.drawerlayout.widget.DrawerLayout;
import androidx.fragment.app.Fragment;
import androidx.fragment.app.FragmentManager;

import com.google.firebase.auth.FirebaseAuth;
import com.google.firebase.auth.FirebaseUser;
import com.bumptech.glide.Glide;

import android.content.Intent;
import android.widget.Button;
import android.widget.Toast;
import android.widget.ImageView;
import android.view.MenuItem;
import android.view.View;
import com.google.android.material.navigation.NavigationView;

import android.os.Bundle;

import java.util.List;

public class HomeActivity extends AppCompatActivity {
    private DrawerLayout drawerLayout;
    private Toolbar tb;
    private NavigationView navView;
    private FirebaseUser user;
    private FirebaseAuth mAuth;
    private String uid;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_home_screen);

        /*mAuth = FirebaseAuth.getInstance();
        user = mAuth.getCurrentUser();
        uid = user.getUid();*/

        drawerLayout = findViewById(R.id.drawer_layout);
        navView = findViewById(R.id.nav_view);
        tb = findViewById(R.id.toolbar);

        setupDrawerContent(navView);

        getSupportFragmentManager().beginTransaction().replace(R.id.fragment_container, new HomeScreen());

        ActionBarDrawerToggle toggle = new ActionBarDrawerToggle(this, drawerLayout, tb,
                R.string.navigation_drawer_open, R.string.navigation_drawer_close);

        drawerLayout.addDrawerListener(toggle);
        toggle.syncState();

        if (savedInstanceState == null) {
            getSupportFragmentManager().beginTransaction().replace(R.id.fragment_container,
                    new HomeScreen()).commit();

            navView.setCheckedItem(R.id.nav_home);
        }

    }

    public boolean selectDrawerItem(@NonNull MenuItem menuItem) {
        switch(menuItem.getItemId()) {
            case R.id.nav_home:
                getSupportFragmentManager().beginTransaction().replace(R.id.fragment_container,
                        new HomeScreen()).commit();
                break;
            case R.id.nav_account:
                getSupportFragmentManager().beginTransaction().replace(R.id.fragment_container,
                        new AccountScreen()).commit();
                break;
            case R.id.nav_achievements:
                getSupportFragmentManager().beginTransaction().replace(R.id.fragment_container,
                        new AchievementsScreen()).commit();
                break;
            default:
                getSupportFragmentManager().beginTransaction().replace(R.id.fragment_container,
                        new PastGamesScreen()).commit();
        }

        drawerLayout.closeDrawer(GravityCompat.START);
        return true;
    }

    private void setupDrawerContent(NavigationView navigationView) {
        navigationView.setNavigationItemSelectedListener(
                new NavigationView.OnNavigationItemSelectedListener() {
                    @Override
                    public boolean onNavigationItemSelected(@NonNull MenuItem menuItem) {
                        selectDrawerItem(menuItem);
                        return true;
                    }
                });
    }

    @Override
    public void onBackPressed(){
        if (drawerLayout.isDrawerOpen(GravityCompat.START)) {
            drawerLayout.closeDrawer(GravityCompat.START);
        } else {
            super.onBackPressed();
        }
    }

    private void showToast(String msg){
        Toast.makeText(this, msg, Toast.LENGTH_SHORT).show();
    }

}