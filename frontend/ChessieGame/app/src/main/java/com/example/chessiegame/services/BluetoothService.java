package com.example.chessiegame.services;

import android.app.Service;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.Intent;
import android.os.Bundle;
import android.os.IBinder;
import android.os.ResultReceiver;
import android.os.SystemClock;
import android.util.Log;

import androidx.annotation.Nullable;

import com.android.volley.Request;
import com.android.volley.RequestQueue;
import com.android.volley.toolbox.StringRequest;
import com.android.volley.toolbox.Volley;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.HashSet;
import java.util.UUID;

public class BluetoothService extends Service {

    private OutputStream btOutputStream;
    private InputStream btInputStream;
    private BluetoothDevice btDevice;
    private ResultReceiver btReceiver;
    BluetoothSocket btSocket;
    private final UUID btUUID = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB");
    private BTThread btThread;
    private boolean started = false;

    private int gameID;
    private int boardID;
    private int sequenceNum;
    private String AIMove;
    private int[][] AIBoard;
    private HashSet<int[][]> validMoves;
    private final int size = 8;

    public BluetoothService() {
    }

    @Nullable
    @Override
    public IBinder onBind(Intent intent) {
        return null;
    }

    @Override
    public void onCreate() {
        Log.d("Bluetooth Service", "Service Created");
        started = false;
    }

    @Override
    public int onStartCommand(Intent intent,
                               int flags,
                               int startId) {

        if (!started) { // first time starting the service, initialize everything
            started = true;
            btDevice = intent.getExtras().getParcelable("btDevice"); // not null
            btReceiver = intent.getExtras().getParcelable("btReceiver");
            gameID = intent.getIntExtra("gameID", 0);
            boardID = 0;
            sequenceNum = intent.getIntExtra("sequenceNum", 0);
            AIMove = "";
            AIBoard = new int[size][size];
            validMoves = new HashSet<int[][]>();

            Log.d("Bluetooth Service", "Service Started");

            // initialize IO Streams to null
            btOutputStream = null;
            btInputStream = null;
            btThread = null;

            try {
                btSocket = btDevice.createRfcommSocketToServiceRecord(btUUID);
                btSocket.connect();
            } catch (IOException e) {
                Log.d("Bluetooth Service", "Bluetooth Connection Failed");
                e.printStackTrace();
            }

            // start the thread without having to connect to btSocket
            btThread = new BTThread(btReceiver);
            btThread.start();

            /*if (btSocket.isConnected()) {
                Log.d("Bluetooth Service", "Bluetooth Socket is connected");
                btThread = new BTThread(btReceiver);
                btThread.start();
            } else {
                Log.d("Bluetooth Service", "Bluetooth Connection Failed");
            }*/

        } else { // data was sent to the running service, receive the target sequenceNum
            Log.d("Bluetooth Service", "Data sent to running service");
            if (btThread != null) {
                /*byte[] data = intent.getExtras().getByteArray("userMove");
                assert data != null;
                btThread.write(data);
                Log.d("Bluetooth Service", "User move sent to btThread");*/

                sequenceNum = intent.getIntExtra("sequenceNum", sequenceNum);
                Log.d("Bluetooth Service", "Target sequence num received: " + sequenceNum);

                // test send back message
                /*Bundle readData = new Bundle();
                readData.putByteArray("readData", data);
                Log.d("Bluetooth Service", "Sending move back to the chessscreen");
                btReceiver.send(1, readData);*/
            }
        }

        return Service.START_STICKY;
    }

    @Override
    public void onDestroy() {
        Log.d("Bluetooth Service", "Service Destroyed");
        if (btThread != null) {
            btThread.interrupt(); // interrupt the thread
        }
    }

    public class BTThread extends Thread {
        ResultReceiver btReceiver;
        RequestQueue queue = Volley.newRequestQueue(getApplicationContext());
        private boolean newInfo;

        public BTThread(ResultReceiver btReceiver) {
            this.btReceiver = btReceiver;
            newInfo = false;

            try {
                btInputStream = btSocket.getInputStream();
                btOutputStream = btSocket.getOutputStream();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }

        public void run() {
            byte[] buffer = new byte[12000];  // buffer store for the stream
            int bytes; // bytes returned from read()
            int currSequenceNum = 0;

            while (true) {
                /*try {
                    // Read from the InputStream
                    bytes = btInputStream.available();
                    if (bytes > 0) {
                        buffer = new byte[12000];
                        SystemClock.sleep(100); // pause and wait for rest of data.
                        bytes = btInputStream.available(); // # of bytes we can read
                        bytes = btInputStream.read(buffer, 0, bytes); // put data in buffer

                        // send the opponent data back to the chess screen activity
                        Bundle readData = new Bundle();
                        readData.putByteArray("readData", buffer);
                        btReceiver.send(1, readData);
                    } else { // alternate design*/

                while (!newInfo) { // wait on new information from the db
                    pollDatabaseForMove(gameID);
                    pollDatabaseForResult(gameID);
                    SystemClock.sleep(200); // sleep between polls
                }

                newInfo = false; // reset newInfo for the next move
                getValidMoves(); // now get the possible valid player moves
                sequenceNum += 2; // always scan for EVEN sequence numbers
                /*    }
                } catch (IOException e) {
                    e.printStackTrace();
                    break;
                }*/
            }
        }

        public void write(byte[] input) {
            try {
                btOutputStream.write(input);
            } catch (IOException e) {
                e.printStackTrace();
            }
        }

        /**
         * Queries the database for the most recent board in the game with ID = gameID
         */
        public void pollDatabaseForMove(int gameID) {
            // TODO: update url later, this does the same thing rn but slower
            String url = "http://ec2-user@ec2-54-153-82-188.us-west-1.compute.amazonaws.com:3000/getlatestboard/" + gameID;

            StringRequest stringRequest = new StringRequest(Request.Method.GET, url,
                    response -> {
                        try {
                            /*int maxNum = boardID;
                            JSONArray jsonArray = new JSONArray(response);
                            for (int i = 0; i < jsonArray.length(); i++) {
                                JSONObject j = jsonArray.getJSONObject(i);
                                // get the max sequenceNumber to find the most recent board
                                if ((int) j.get("boardID") > maxNum) {
                                    maxNum = (int) j.get("boardID");
                                    AIMove = (String) j.get("placements");
                                }
                            }

                            if (maxNum > boardID) {
                                boardID = maxNum;
                                AIBoard = parseMove(AIMove);
                                newInfo = true;
                            }*/

                            JSONArray arr = new JSONArray(response);
                            JSONObject res = arr.getJSONObject(0);
                            int num = (int) res.get("sequenceNumber");
                            if (num == sequenceNum) { // there is new information in the db
                                AIMove = res.get("placements").toString(); // the AI's move
                                AIBoard = parseMove(AIMove);
                                newInfo = true;
                            }
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

        /**
         * Queries the database for an updated result for game with gameID
         */
        public void pollDatabaseForResult(int gameID) {
            // TODO: query the database for a result != -1, update newInfo if updated result found
        }

        /**
         * Queries the database for all the possible valid player moves,
         * sends all valid moves and AI move to the chess screen,
         * and finally clears the valid player moves table for the next entry
         */
        public void getValidMoves() {
            // TODO: update url to get valid player moves
            String url = "http://ec2-user@ec2-54-153-82-188.us-west-1.compute.amazonaws.com:3000/getallmoves";

            StringRequest stringRequest = new StringRequest(Request.Method.GET, url,
                    response -> {
                        try {
                            JSONArray arr = new JSONArray(response);

                            for (int i = 0; i < arr.length(); i++) {
                                JSONObject elem = arr.getJSONObject(i);
                                validMoves.add(parseMove((String) elem.get("placements")));
                            }
                            // send AI move and Set containing valid player moves to the chess screen
                            Bundle AIData = new Bundle();
                            AIData.putSerializable("AIMove", AIBoard);
                            AIData.putSerializable("validMoves", validMoves);
                            // TODO: update this with the game result returned by AI
                            AIData.putInt("result", -1);
                            btReceiver.send(1, AIData);

                            // OPTIONAL: clear the validMoves table?
                        } catch (JSONException e) {
                            e.printStackTrace();
                        }
                    },
                    error -> {
                        Log.d("ChessScreen", "Error fetching valid player moves");
                    });

            // Add the request to the RequestQueue.
            queue.add(stringRequest);
        }

        /**
         * Turns the String representing the AI move into a 2d array of piece IDs
         */
        public int[][] parseMove(String s) {
            String[] boardString = s.split("\\s+");
            int[][] layout = new int[size][size];

            for (int i = 0; i < size; i++) {
                for (int j = 0; j < size; j++) {
                    layout[i][j] = Integer.parseInt(boardString[i * size + j]);
                }
            }

            return layout;
        }
    }
}
