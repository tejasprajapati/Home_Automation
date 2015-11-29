package com.example.bluetoothstm32;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.UUID;
 
import com.example.bluetoothstm32.MainActivity;
import com.example.bluetoothstm32.SetPreferenceActivity;
import com.example.bluetoothstm32.R;
 
import android.app.Activity;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.Intent;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.os.Handler;
import android.preference.PreferenceManager;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;
 
public class MainActivity extends Activity {
	  private static final String TAG = "bluetoothSTM32";
	   
	  Button btnOn, btnOff;
	  TextView txtArduino;
	  Handler h;
	  private String address; 	// MAC-address
	   
	  private static final int REQUEST_ENABLE_BT = 1;
	  final int RECIEVE_MESSAGE = 1;		// Status for Handler
	  private BluetoothAdapter btAdapter = null;
	  private BluetoothSocket btSocket = null;
	  private StringBuilder sb = new StringBuilder();
	  
	  private ConnectedThread mConnectedThread;
	   
	  // SPP UUID sevice 
	  private static final UUID MY_UUID = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB");
	 
   
	  /** Called when the activity is first created. */
	  @Override
	  public void onCreate(Bundle savedInstanceState) {
	    super.onCreate(savedInstanceState);
	 
	    setContentView(R.layout.activity_main);
	    
	    address = (String) getResources().getText(R.string.default_MAC);
	 
	    btnOn = (Button) findViewById(R.id.btnOn);					// button ON
	    btnOff = (Button) findViewById(R.id.btnOff);				// button OFF
	    txtArduino = (TextView) findViewById(R.id.txtArduino);		// for print text recieved from Arduino
	    
	    loadPref();
	    
	    h = new Handler() {
	    	public void handleMessage(android.os.Message msg) {
	    		switch (msg.what) {
	            case RECIEVE_MESSAGE:													// if message is recieved
	            	byte[] readBuf = (byte[]) msg.obj;
	            	String strIncom = new String(readBuf, 0, msg.arg1);
	            	sb.append(strIncom);												// append string
	            	int endOfLineIndex = sb.indexOf("\r\n");							// determine end of line
	            	if (endOfLineIndex > 0) { 											// id end of line,
	            		String sbprint = sb.substring(0, endOfLineIndex);
	                    sb.delete(0, sb.length());										// slear sb
	                	txtArduino.setText("Ответ от Arduino: " + sbprint); 	        // update TextView
	                	btnOff.setEnabled(true);
	                	btnOn.setEnabled(true); 
	                }
	            	//Log.d(TAG, "...String:"+ sb.toString() +  "Byte:" + msg.arg1 + "...");
	            	break;
	    		}
	        };
		};
	     
	    btAdapter = BluetoothAdapter.getDefaultAdapter();		// Bluetooth adapter
	    checkBTState();
	 
	    btnOn.setOnClickListener(new OnClickListener() {
	      public void onClick(View v) {
	    	btnOn.setEnabled(false);
	    	mConnectedThread.write("1");	// Send string via Bluetooth
	        //Toast.makeText(getBaseContext(), "Try LED On", Toast.LENGTH_SHORT).show();
	      }
	    });
	 
	    btnOff.setOnClickListener(new OnClickListener() {
	      public void onClick(View v) {
	    	btnOff.setEnabled(false);  
	    	mConnectedThread.write("0");	// Send string via Bluetooth
	        //Toast.makeText(getBaseContext(), "Try LED Off", Toast.LENGTH_SHORT).show();
	      }
	    });
	  }
	   
	  @Override
	  public void onResume() {
	    super.onResume();
	 
	    Log.d(TAG, "...onResume - try connect...");
	   
	    
	    if(!BluetoothAdapter.checkBluetoothAddress(address)){
    		//errorExit("Fatal Error", "Incorrect MAC-address");
    		Toast.makeText(getBaseContext(), "Incorrect MAC-address", Toast.LENGTH_SHORT).show();
    	}
    	else{
		    // Set up a pointer to the remote node using it's address.
		    BluetoothDevice device = btAdapter.getRemoteDevice(address);
		   
		    // Two things are needed to make a connection:
		    //   A MAC address, which we got above.
		    //   A Service ID or UUID.  In this case we are using the
		    //     UUID for SPP.
		    try {
		      btSocket = device.createRfcommSocketToServiceRecord(MY_UUID);
		    } catch (IOException e) {
		      errorExit("Fatal Error", "In onResume() and socket create failed: " + e.getMessage() + ".");
		    }
		   
		    // Discovery is resource intensive.  Make sure it isn't going on
		    // when you attempt to connect and pass your message.
		    btAdapter.cancelDiscovery();
		   
		    // Establish the connection.  This will block until it connects.
		    Log.d(TAG, "...Connecting...");
		    try {
		      btSocket.connect();
		      Log.d(TAG, "...Connection is OK...");
		    } catch (IOException e) {
		      try {
		        btSocket.close();
		      } catch (IOException e2) {
		        errorExit("Fatal Error", "In onResume() and unable to close socket during connection failure" + e2.getMessage() + ".");
		      }
		    }
		     
		    // Create a data stream so we can talk to server.
		    Log.d(TAG, "...Create Socket...");
		   
		    mConnectedThread = new ConnectedThread(btSocket);
		    mConnectedThread.start();
    	}
	  }
	 
	  @Override
	  public void onPause() {
	    super.onPause();
	 
	    Log.d(TAG, "...In onPause()...");
	  
	    try     {
	      btSocket.close();
	    } catch (IOException e2) {
	      errorExit("Fatal Error", "In onPause() and failed to close socket." + e2.getMessage() + ".");
	    }
	  }
	   
	  @Override
	  protected void onActivityResult(int requestCode, int resultCode, Intent data) {
		  loadPref();
	  }
	  
	  
	  private void checkBTState() {
	    // Check for Bluetooth support and then check to make sure it is turned on
	    // Emulator doesn't support Bluetooth and will return null
	    if(btAdapter==null) { 
	      errorExit("Fatal Error", "Bluetooth is not support");
	    } else {
	      if (btAdapter.isEnabled()) {
	        Log.d(TAG, "...Bluetooth is ON...");
	      } else {
	        //Prompt user to turn on Bluetooth
	        Intent enableBtIntent = new Intent(btAdapter.ACTION_REQUEST_ENABLE);
	        startActivityForResult(enableBtIntent, REQUEST_ENABLE_BT);
	      }
	    }
	  }
	 
	  private void errorExit(String title, String message){
	    Toast.makeText(getBaseContext(), title + " - " + message, Toast.LENGTH_LONG).show();
	    finish();
	  }
	 
	  private class ConnectedThread extends Thread {
		    private final BluetoothSocket mmSocket;
		    private final InputStream mmInStream;
		    private final OutputStream mmOutStream;
		 
		    public ConnectedThread(BluetoothSocket socket) {
		        mmSocket = socket;
		        InputStream tmpIn = null;
		        OutputStream tmpOut = null;
		 
		        // Get the input and output streams, using temp objects because
		        // member streams are final
		        try {
		            tmpIn = socket.getInputStream();
		            tmpOut = socket.getOutputStream();
		        } catch (IOException e) { }
		 
		        mmInStream = tmpIn;
		        mmOutStream = tmpOut;
		    }
		 
		    public void run() {
		        byte[] buffer = new byte[256];  // buffer store for the stream
		        int bytes; // bytes returned from read()

		        // Keep listening to the InputStream until an exception occurs
		        while (true) {
		        	try {
		                // Read from the InputStream
		                bytes = mmInStream.read(buffer);
	                    h.obtainMessage(RECIEVE_MESSAGE, bytes, -1, buffer).sendToTarget();
		            } catch (IOException e) {
		                break;
		            }
		        }
		    }
		 
		    /* Call this from the main activity to send data to the remote device */
		    public void write(String message) {
		    	Log.d(TAG, "...String to send: " + message + "...");
		    	byte[] msgBuffer = message.getBytes();
		    	try {
		            mmOutStream.write(msgBuffer);
		        } catch (IOException e) {
		            Log.d(TAG, "...Error send: " + e.getMessage() + "...");     
		          }
		    }
		 
		    /* Call this from the main activity to shutdown the connection */
		    public void cancel() {
		        try {
		            mmSocket.close();
		        } catch (IOException e) { }
		    }
		}
	  
	    @Override
	    public boolean onCreateOptionsMenu(Menu menu) {
	        getMenuInflater().inflate(R.menu.activity_main, menu);
	        return true;
	    }
	  
	    @Override
	    public boolean onOptionsItemSelected(MenuItem item) {  
			 Intent intent = new Intent();
		     intent.setClass(MainActivity.this, SetPreferenceActivity.class);
		     startActivityForResult(intent, 0); 
		  
		     return true;
		 }  
	  
	private void loadPref(){
	    	SharedPreferences mySharedPreferences = PreferenceManager.getDefaultSharedPreferences(this);  
	    	
	    	address = mySharedPreferences.getString("pref_MAC_address", address);
	    	//Toast.makeText(getBaseContext(), address, Toast.LENGTH_SHORT).show();
	}
}