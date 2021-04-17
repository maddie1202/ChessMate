package com.example.chessiegame.components;


import android.content.Context;

import java.util.HashMap;
import java.util.Map;

import com.example.chessiegame.R;

public class BoardMap {
    Map<Integer, Integer> imageMap;

    public BoardMap(Context c) {
        imageMap = new HashMap<>();
        for (int i = 1; i < 9; i++) {
            imageMap.put(i, R.drawable.wpawn); // wpawn drawable
        }

        for (int i = 9; i < 19; i++) {
            imageMap.put(i, R.drawable.wrook); // wrook
        }

        for (int i = 19; i < 29; i++) {
            imageMap.put(i, R.drawable.wknight); // wknight
        }

        for (int i = 29; i < 39; i++) {
            imageMap.put(i, R.drawable.wbishop); // wbishop
        }

        for (int i = 39; i < 48; i++) {
            imageMap.put(i, R.drawable.wqueen); // wqueen
        }

        imageMap.put(48, R.drawable.wking); // wking

        for (int i = -1; i > -9; i--) {
            imageMap.put(i, R.drawable.bpawn); // bpawn
        }

        for (int i = -9; i > -19; i--) {
            imageMap.put(i, R.drawable.brook); // brook
        }

        for (int i = -19; i > -29; i--) {
            imageMap.put(i, R.drawable.bknight); // bknight
        }

        for (int i = -29; i > -39; i--) {
            imageMap.put(i, R.drawable.bbishop); // bbishop
        }

        for (int i = -39; i > -48; i--) {
            imageMap.put(i, R.drawable.bqueen); // bqueen
        }

        imageMap.put(-48, R.drawable.bking); // bking

    }

    public int get(int key) {
        return imageMap.get(key);
    }

    public int size() { return imageMap.size(); }

    public boolean isEmpty() {
        return imageMap.isEmpty();
    }

}