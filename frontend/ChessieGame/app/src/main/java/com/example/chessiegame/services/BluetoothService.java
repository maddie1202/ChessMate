package com.example.chessiegame.services;

import android.app.IntentService;
import android.app.Service;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.Intent;
import android.os.Bundle;
import android.os.IBinder;
import android.os.SystemClock;
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
    private ChessScreen.BTReceiver btReceiver;
    BluetoothSocket btSocket;
    private final UUID btUUID = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB");
    private BTThread btThread;
    private boolean started;

    public BluetoothService() {
    }

    @Nullable
    @Override
    public IBinder onBind(Intent intent) {
        return null;
    }

    @Override
    public void onCreate() {
        Toast.makeText(this, "The new Service was Created", Toast.LENGTH_LONG).show();
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
            Toast.makeText(this, " Service Started", Toast.LENGTH_LONG).show();

            // initialize IO Streams to null
            btOutputStream = null;
            btInputStream = null;
            btThread = null;

            try {
                btSocket = btDevice.createRfcommSocketToServiceRecord(btUUID);
                btSocket.connect();
            } catch (IOException e) {
                e.printStackTrace();
            }

            if (btSocket.isConnected()) {
                btThread = new BTThread(btReceiver);
                btThread.start();
            } else {
                Toast.makeText(this, "Bluetooth Connection Failed", Toast.LENGTH_LONG).show();
            }
        } else { // data was sent to the running service
            if (btThread != null) {
                byte[] data = intent.getExtras().getByteArray("userMove");
                btThread.write(data);
            }
        }

        return Service.START_STICKY;
    }

    @Override
    public void onDestroy() {
        Toast.makeText(this, "Service Destroyed", Toast.LENGTH_LONG).show();

    }

    public class BTThread extends Thread {
        ChessScreen.BTReceiver btReceiver;

        public BTThread(ChessScreen.BTReceiver btReceiver) {
            this.btReceiver = btReceiver;

            try {
                btInputStream = btSocket.getInputStream();
                btOutputStream = btSocket.getOutputStream();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }

        public void run() {
            byte[] buffer = new byte[1024];  // buffer store for the stream
            int bytes; // bytes returned from read()

            while (true) {
                try {
                    // Read from the InputStream
                    bytes = btInputStream.available();
                    if (bytes > 0) {
                        buffer = new byte[1024];
                        SystemClock.sleep(100); //pause and wait for rest of data.
                        bytes = btInputStream.available(); // # of bytes we can read
                        bytes = btInputStream.read(buffer, 0, bytes); // put data in buffer

                        // send the opponent data back to the chess screen activity
                        // TODO: convert buffer (byte array) to a Bundle
                        btReceiver.send(1, null);
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
