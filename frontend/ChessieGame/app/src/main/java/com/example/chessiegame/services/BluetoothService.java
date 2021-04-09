package com.example.chessiegame.services;

import android.app.IntentService;
import android.app.Service;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.Intent;
import android.os.Bundle;
import android.os.IBinder;
import android.os.ResultReceiver;
import android.os.SystemClock;
import android.util.Log;
import android.widget.Toast;

import androidx.annotation.Nullable;

import com.example.chessiegame.ChessScreen;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
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

            if (btSocket.isConnected()) {
                Log.d("Bluetooth Service", "Bluetooth Socket is connected");
                btThread = new BTThread(btReceiver);
                btThread.start();

            } else {
                Log.d("Bluetooth Service", "Bluetooth Connection Failed");
            }
        } else { // data was sent to the running service
            Log.d("Bluetooth Service", "Data sent to running service");
            if (btThread != null) {
                byte[] data = intent.getExtras().getByteArray("userMove");
                assert data != null;
                Log.d("Bluetooth Service", "Received user move: " + new String(data));
                btThread.write(data);
                Log.d("Bluetooth Service", "User move sent to btThread");

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
    }

    public class BTThread extends Thread {
        ResultReceiver btReceiver;

        public BTThread(ResultReceiver btReceiver) {
            this.btReceiver = btReceiver;

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

            while (true) {
                try {
                    // Read from the InputStream
                    bytes = btInputStream.available();
                    if (bytes > 0) {
                        buffer = new byte[12000]; // TODO: make this number bigger later
                        SystemClock.sleep(100); // pause and wait for rest of data.
                        bytes = btInputStream.available(); // # of bytes we can read
                        bytes = btInputStream.read(buffer, 0, bytes); // put data in buffer

                        // send the opponent data back to the chess screen activity
                        Bundle readData = new Bundle();
                        readData.putByteArray("readData", buffer);
                        btReceiver.send(1, readData);
                    }
                } catch (IOException e) {
                    e.printStackTrace();
                    break;
                }
            }
        }

        public void write(byte[] input) {
            try {
                btOutputStream.write(input);
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }
}
