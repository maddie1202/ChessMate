package com.example.chessiegame;

import androidx.annotation.NonNull;
import androidx.appcompat.app.ActionBarDrawerToggle;
import androidx.appcompat.app.AppCompatActivity;
import androidx.appcompat.widget.Toolbar;
import androidx.core.view.GravityCompat;
import androidx.drawerlayout.widget.DrawerLayout;
import androidx.fragment.app.Fragment;
import androidx.fragment.app.FragmentManager;

import android.content.Intent;
import android.widget.Button;
import android.widget.Toast;
import android.widget.ImageView;
import android.view.MenuItem;
import android.view.View;
import com.google.android.material.navigation.NavigationView;

import android.os.Bundle;

import java.util.List;

public class HomeActivity extends AppCompatActivity implements NavigationView.OnNavigationItemSelectedListener {
    private DrawerLayout drawerLayout;
    private Toolbar tb;
    private NavigationView navView;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_home_screen);

        drawerLayout = findViewById(R.id.drawer_layout);
        navView = findViewById(R.id.nav_view);
        tb = findViewById(R.id.toolbar);

        navView.setNavigationItemSelectedListener(this);
        getSupportFragmentManager().beginTransaction().replace(R.id.fragment_container, new HomeScreen());

        ActionBarDrawerToggle toggle = new ActionBarDrawerToggle(this, drawerLayout, tb,
                R.string.navigation_drawer_open, R.string.navigation_drawer_close);

        drawerLayout.addDrawerListener(toggle);
        toggle.syncState();
       // setupDrawerContent(navView);

        if (savedInstanceState == null) {
            getSupportFragmentManager().beginTransaction().replace(R.id.fragment_container,
                    new HomeScreen()).commit();

            navView.setCheckedItem(R.id.nav_home);
        }

        /*menu.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                drawerLayout.openDrawer(GravityCompat.START);
            }
        });*/

    }

    /*public boolean selectDrawerItem(@NonNull MenuItem menuItem) {
        showToast("Selected Item");
        switch(menuItem.getItemId()) {
            case R.id.nav_home:
                getSupportFragmentManager().beginTransaction().replace(R.id.fragment_container,
                        new HomeScreen()).commit();
                showToast("Home Screen");
                break;
            case R.id.nav_account:
                getSupportFragmentManager().beginTransaction().replace(R.id.fragment_container,
                        new AccountScreen()).commit();
                showToast("Account Screen");
                break;
            case R.id.nav_achievements:
                getSupportFragmentManager().beginTransaction().replace(R.id.fragment_container,
                        new AchievementsScreen()).commit();
                showToast("Achievements Screen");
                break;
            default:
                getSupportFragmentManager().beginTransaction().replace(R.id.fragment_container,
                        new PastGamesScreen()).commit();
                showToast("Past Games Screen");
        }

        menuItem.setChecked(true);
        drawerLayout.closeDrawer(GravityCompat.START);
        return true;
    }*/

    @Override
    public boolean onNavigationItemSelected(@NonNull MenuItem menuItem) {
        switch(menuItem.getItemId()) {
            case R.id.nav_home:
                getSupportFragmentManager().beginTransaction().replace(R.id.fragment_container,
                        new HomeScreen()).commit();
                showToast("Home Screen");
                break;
            case R.id.nav_account:
                getSupportFragmentManager().beginTransaction().replace(R.id.fragment_container,
                        new AccountScreen()).commit();
                showToast("Account Screen");
                break;
            case R.id.nav_achievements:
                getSupportFragmentManager().beginTransaction().replace(R.id.fragment_container,
                        new AchievementsScreen()).commit();
                showToast("Achievements Screen");
                break;
            default:
                getSupportFragmentManager().beginTransaction().replace(R.id.fragment_container,
                        new PastGamesScreen()).commit();
                showToast("Past Games Screen");
        }
        drawerLayout.closeDrawer(GravityCompat.START);
        return true;

    }

    /*private void setupDrawerContent(NavigationView navigationView) {
        navigationView.setNavigationItemSelectedListener(
                new NavigationView.OnNavigationItemSelectedListener() {
                    @Override
                    public boolean onNavigationItemSelected(MenuItem menuItem) {
                        selectDrawerItem(menuItem);
                        return true;
                    }
                });
    }*/

    /*public void selectDrawerItem(MenuItem menuItem) {
        // Create a new fragment and specify the fragment to show based on nav item clicked
        Fragment fragment = null;
        Class fragmentClass;


        if (menuItem.getItemId() == R.id.nav_home) {
            Intent intent = new Intent( this, HomeActivity.class);
            startActivity( intent );
            showToast("home icon was clicked");
            menuItem.setChecked(true);
            drawerLayout.closeDrawers();
            return;
        }

        switch(menuItem.getItemId()) {
            case R.id.nav_home:
                fragmentClass = HomeScreen.class;
                showToast("Home Screen");
                break;
            case R.id.nav_account:
                fragmentClass = AccountScreen.class;
                showToast("Account Screen");
                break;
            case R.id.nav_achievements:
                fragmentClass = AchievementsScreen.class;
                showToast("Achievements Screen");
                break;
            default:
                fragmentClass = PastGamesScreen.class;
                showToast("Past Games Screen");
        }

        try {
            fragment = (Fragment) fragmentClass.newInstance();
        } catch (Exception e) {
            e.printStackTrace();
        }

        // Insert the fragment by replacing any existing fragment
        FragmentManager fragmentManager = getSupportFragmentManager();
        fragmentManager.beginTransaction().replace(R.id.fragment_container, fragment).commit();

        // Highlight the selected item has been done by NavigationView
        menuItem.setChecked(true);
        // Set action bar title
        setTitle(menuItem.getTitle());
        // Close the navigation drawer
        drawerLayout.closeDrawers();
    }*/

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