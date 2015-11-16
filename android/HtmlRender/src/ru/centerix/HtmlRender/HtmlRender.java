package ru.centerix.HtmlRender;

import android.app.Activity;
import android.os.Bundle;
import android.widget.TextView;
import android.content.Context;
import android.telephony.TelephonyManager;
import android.provider.Settings;
import android.provider.Settings.Secure;
import java.util.UUID;
import android.net.wifi.WifiManager;
import android.os.Build;
import android.accounts.Account;
import android.accounts.AccountManager;
import android.net.Uri;
import android.database.Cursor;

import java.io.File;
import java.io.FileInputStream;
import java.io.InputStream;

import java.io.IOException;

import android.graphics.BitmapFactory;

import android.widget.RelativeLayout;
import android.widget.ImageView;
import android.graphics.Bitmap;
import android.view.View;



public class HtmlRender extends Activity{
	TextView textView1;
	RelativeLayout rlayout;
	ImageView imageView;

	static {
		System.loadLibrary("nativehtmlrender");
	}

	
    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState){
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);
        textView1 = (TextView) findViewById(R.id.textView1);
	rlayout = (RelativeLayout) findViewById(R.id.imageView1);

	imageView = new ImageView(this);

	//NativeTest();

	byte[] bytes = NativeHtmlRender("/sdcard/index.html");

	//Bitmap bitmap = getBitmap("/sdcard/image.jpg");
	Bitmap bitmap = BitmapFactory.decodeByteArray(bytes, 0, bytes.length);

	
	textView1.setText("Hello world!");

	imageView.setImageBitmap(bitmap);
	rlayout.addView(imageView);
    }


	private Bitmap getBitmap(String path){
       		//Uri uri = Uri.parse(path);
    		File file = new File(path);

    		InputStream in = null;
    		try {
    		    final int IMAGE_MAX_SIZE = 1200000; // 1.2MP
    		    in = new FileInputStream(file);

    		    // Decode image size
    		    BitmapFactory.Options o = new BitmapFactory.Options();
    		    o.inJustDecodeBounds = true;
    		    BitmapFactory.decodeStream(in, null, o);
    		    in.close();

    		    Bitmap b = null;
    		    in = new FileInputStream(file);

	        b = BitmapFactory.decodeStream(in);

    		    in.close();

    		    return b;
    		} catch (IOException e) {
    		  //  Log.e(TAG, e.getMessage(),e);
    		    return null;
    		}

	}


/*
    	private Bitmap getBitmap(String path) {

    		//Uri uri = Uri.parse(path);
    		File file = new File(path);

    		InputStream in = null;
    		try {
    		    final int IMAGE_MAX_SIZE = 1200000; // 1.2MP
    		    in = new FileInputStream(file);

    		    // Decode image size
    		    BitmapFactory.Options o = new BitmapFactory.Options();
    		    o.inJustDecodeBounds = true;
    		    BitmapFactory.decodeStream(in, null, o);
    		    in.close();

    		    int scale = 1;
    		    while ((o.outWidth * o.outHeight) * (1 / Math.pow(scale, 2)) >
    		          IMAGE_MAX_SIZE) {
    		       scale++;
    		    }
    		  //  Log.d(TAG, "scale = " + scale + ", orig-width: " + o.outWidth + ", orig-height: " + o.outHeight);
    		    imgt=o.outWidth + " " + o.outHeight;
    		    img_scale=o.outWidth;

    		    Bitmap b = null;
    		    in = new FileInputStream(file);
    		    if(scale > 1){
    		        scale--;
    		        // scale to max possible inSampleSize that still yields an image
    		        // larger than target
    		        o = new BitmapFactory.Options();
    		        o.inSampleSize = scale;
    		        b = BitmapFactory.decodeStream(in, null, o);

    		        // resize to desired dimensions
    		        int height = b.getHeight();
    		        int width = b.getWidth();
    		   //     Log.d(TAG, "1th scale operation dimenions - width: " + width + ", height: " + height);

    		        double y = Math.sqrt(IMAGE_MAX_SIZE
    		                / (((double) width) / height));
    		        double x = (y / height) * width;
    		        //if(y / height < x / width) img_scale= y / height; else img_scale= x / width;
    		        //img_scale = y / o.outHeight;

    		        Bitmap scaledBitmap = Bitmap.createScaledBitmap(b, (int) x, (int) y, true);
    		        b.recycle();
    		        b = scaledBitmap;

    		        System.gc();
    		    } else {
    		        b = BitmapFactory.decodeStream(in);
    		    }
    		    in.close();

    		    img_scale = b.getWidth() / img_scale;
    		    imgt = imgt + " " + b.getWidth() + " " + b.getHeight() + " " + img_scale;
    		    ivbsx=b.getWidth(); ivbsy=b.getHeight();
    		 //   Log.d(TAG, "bitmap size - width: " +b.getWidth() + ", height: " + b.getHeight());
    		    return b;
    		} catch (IOException e) {
    		  //  Log.e(TAG, e.getMessage(),e);
    		    return null;
    		}
    	}
  */

	private native byte[] NativeHtmlRender(String html);
	public native int NativeTest();

}
