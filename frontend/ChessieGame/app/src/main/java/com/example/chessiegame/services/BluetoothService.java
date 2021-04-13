package com.example.chessiegame.services;

import android.app.Service;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
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
    //private BluetoothDevice btDevice;
    private ResultReceiver btReceiver = null;
    BluetoothSocket btSocket;
    private final UUID btUUID = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB");
    private BTThread btThread;
    private boolean started = false;
    private boolean startPolling;

    private int gameID;
    private int gameResult;
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
        if (intent == null) {
            return Service.START_STICKY;
        }

        if (!started) { // first time starting the service, initialize everything
            started = true;
            //btDevice = intent.getExtras().getParcelable("btDevice"); // not null
            btReceiver = intent.getExtras().getParcelable("btReceiver");
            gameID = intent.getIntExtra("gameID", 0);
            gameResult = -1;
            sequenceNum = intent.getIntExtra("sequenceNum", 0); // sequenceNum to wait for
            startPolling = true; // start scanning db for an opponent entry
            AIMove = "";
            AIBoard = new int[size][size];
            validMoves = new HashSet<int[][]>();

            Log.d("Bluetooth Service", "Service Started");

            // initialize IO Streams to null
            btOutputStream = null;
            btInputStream = null;

            /*try {
                btSocket = btDevice.createRfcommSocketToServiceRecord(btUUID);
                btSocket.connect();
            } catch (IOException e) {
                Log.d("Bluetooth Service", "Bluetooth Connection Failed");
                e.printStackTrace();
            }*/

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
                btThread.write(data);*/

                startPolling = true; // player made a move, start polling for opponent's move
                Log.d("Bluetooth Service", "Target sequence num received: " + sequenceNum);

            }
        }

        return Service.START_STICKY;
    }

    @Override
    public void onDestroy() {
        Log.d("Bluetooth Service", "Service Destroyed");
    }

    public class BTThread extends Thread {
        ResultReceiver btReceiver;
        RequestQueue queue = Volley.newRequestQueue(getApplicationContext());
        private boolean newInfo;

        public BTThread(ResultReceiver btReceiver) {
            this.btReceiver = btReceiver;
            newInfo = false;

            // Bluetooth socket initialization
            /*try {
                btInputStream = btSocket.getInputStream();
                btOutputStream = btSocket.getOutputStream();
            } catch (IOException e) {
                e.printStackTrace();
            }*/
        }

        public void run() {
            byte[] buffer = new byte[12000];  // buffer store for the stream
            int bytes; // bytes returned from read()

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

                while (!newInfo && startPolling) { // wait on new information from the db
                    pollDatabaseForResult(gameID);
                    Log.d("Bluetooth Service", "Polling database");
                    wait(250); // sleep between polls
                }

                if (startPolling) { // we have info and are still polling
                    newInfo = false; // reset newInfo for the next move
                    startPolling = false; // stop polling until player makes a move
                    if (gameResult == 1) {
                        sendPlayerWon();
                    } else {
                        //getValidMoves(); // now get the possible valid player moves
                        // send AI move and Set containing valid player moves to the chess screen
                        Bundle AIData = new Bundle();
                        AIData.putSerializable("AIMove", AIBoard);
                        AIData.putSerializable("validMoves", validMoves);
                        AIData.putInt("result", gameResult);
                        btReceiver.send(1, AIData);
                        Log.d("Bluetooth Service", "Done Polling and received result");
                        sequenceNum += 2; // always scan for EVEN sequence numbers
                    }
                }
                /*    }
                } catch (IOException e) {
                    e.printStackTrace();
                    break;
                }*/
            }
        }

        public void wait(int ms) {
            try {
                Thread.sleep(ms);
            } catch(InterruptedException ex) {
                Thread.currentThread().interrupt();
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
            String url = "http://ec2-user@ec2-54-153-82-188.us-west-1.compute.amazonaws.com:3000/getlatestboard/" + gameID;

            StringRequest stringRequest = new StringRequest(Request.Method.GET, url,
                    response -> {
                        try {
                            JSONObject json = new JSONObject(response);
                            if (json != null) { // in case the game has nothing in it
                                int num = (int) json.get("sequenceNumber");
                                Log.d("Bluetooth Service", "Service polled sequenceNum: " + num);
                                if (num == sequenceNum) { // there is new information in the db
                                    AIMove = json.get("placements").toString(); // the AI's move
                                    AIBoard = parseBoard(AIMove);
                                    newInfo = true;
                                }
                            } else {
                                Log.d("BluetoothService", "Poll database didn't work");
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
            String url = "http://ec2-user@ec2-54-153-82-188.us-west-1.compute.amazonaws.com:3000/getgameresult/" + gameID;

            StringRequest stringRequest = new StringRequest(Request.Method.GET, url,
                    response -> {
                        try {
                            JSONArray arr = new JSONArray(response);
                            if (arr.length() > 0) { // non-empty result
                                int result = (int) arr.getJSONObject(0).get("result");
                                if (result == 0) { // opponent won
                                    gameResult = 0;
                                    pollDatabaseForMove(gameID);
                                } else if (result == 1) { // player won
                                    gameResult = 1;
                                    newInfo = true; // no need to look for opponent data if player won
                                } else { // we keep playing, look for a new move
                                    gameResult = -1;
                                    pollDatabaseForMove(gameID);
                                }
                            }
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
         * Queries the database for all the possible valid player moves,
         * sends all valid moves and AI move to the chess screen,
         * and finally clears the valid player moves table for the next entry
         */
        public void getValidMoves() {
            String url = "http://ec2-user@ec2-54-153-82-188.us-west-1.compute.amazonaws.com:3000/getallmoves";

            StringRequest stringRequest = new StringRequest(Request.Method.GET, url,
                    response -> {
                        try {
                            JSONArray arr = new JSONArray(response);
                            if (arr.length() > 0) {
                                String allMoves = (String) arr.getJSONObject(0).get("placements");
                                String[] moveBoards = allMoves.split(",");
                                for (String moveBoard : moveBoards) {
                                    Log.d("Bluetooth Service", moveBoard);
                                    validMoves.add(parseBoard(moveBoard));
                                }
                            }

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
         * Signal that the player won the game
         */
        public void sendPlayerWon() { // when gameResult == 1, AIBoard and validMoves are ignored
            // send AI move and Set containing valid player moves to the chess screen
            Bundle AIData = new Bundle();
            AIData.putSerializable("AIMove", AIBoard);
            AIData.putSerializable("validMoves", validMoves);
            AIData.putInt("result", gameResult);
            btReceiver.send(1, AIData);
        }

        /**
         * Uses regex to parse a String representing piece placements into a 2D array of piece IDs
         */
        public int[][] parseBoard(String b) {
            int[][] layout = new int[size][size];
            String[] boardString = b.split("\\s+");

            for (int j = 0; j < size; j++) {
                for (int k = 0; k < size; k++) {
                    layout[j][k] = Integer.parseInt(boardString[(7 - j) * size + k]);
                }
            }

            return layout;
        }
    }
}
