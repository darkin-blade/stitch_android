package com.example.stitch;

import androidx.appcompat.app.AppCompatActivity;
import androidx.core.app.ActivityCompat;

import android.content.Context;
import android.content.pm.PackageManager;
import android.graphics.Bitmap;
import android.graphics.Color;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.Spinner;
import android.widget.TextView;
import android.widget.Toast;

import org.opencv.android.OpenCVLoader;
import org.opencv.core.Mat;

public class MainActivity extends AppCompatActivity {
    static public int window_num;
    static public String appPath = null;// app路径

    static public final int MAIN = 0;

    Spinner featuresTypeList;
    Button button_1;

    // 初始化opencv java
    static {
        if (!OpenCVLoader.initDebug()) {
            infoLog("opencv init failed");
        } else {
            infoLog("opencv init succeed");
        }
    }

    // TODO
    static {
        System.loadLibrary("native-lib");// TODO
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main_activity);

        initApp();
        initUI();

        infoLog("create finish");
    }

    public void initApp() {// 检查权限
        String permission = "android.permission.WRITE_EXTERNAL_STORAGE";
        int check_result = ActivityCompat.checkSelfPermission(this, permission);// `允许`返回0,`拒绝`返回-1
        if (check_result != PackageManager.PERMISSION_GRANTED) {// 没有`写`权限
            ActivityCompat.requestPermissions(this, new String[]{permission}, 1);// 获取`写`权限
        }

        // 初始化路径字符串
        appPath = getExternalFilesDir("").getAbsolutePath();
    }

    public void initUI() {
        featuresTypeList = findViewById(R.id.features_type);
        button_1 = findViewById(R.id.button_1);

        String[] featuresTypeArray = new String[]{"orb", "akaze"};
        ArrayAdapter<String> featuresTypeAdapter = new ArrayAdapter<String>(this, R.layout.spinner_1, featuresTypeArray);
        featuresTypeList.setAdapter(featuresTypeAdapter);

        button_1.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                simpleTest();
            }
        });
    }

    public void simpleTest() {
        String featuresType = (String) featuresTypeList.getSelectedItem();
        infoLog("features type: " + featuresType);
    }

    public void midTest() {
        infoLog("mid test");

        Mat matBGR = new Mat();
        // 查找特征点
//        findPoint(appPath + "/img0.png", matBGR.getNativeObjAddr());
        // 匹配特征点
//        matchPoint(new String[]{appPath + "/mac1.jpg", appPath + "/mac2.jpg"}, matBGR.getNativeObjAddr());
    }

    static public void infoLog(String log) {
        Log.i("fuck", log);
    }

    static public void infoToast(Context context, String log) {
        Toast toast =  Toast.makeText(context, log, Toast.LENGTH_SHORT);
        View view = toast.getView();
        TextView textView = view.findViewById(android.R.id.message);
        textView.setTextColor(Color.rgb(0x00, 0x00, 0x00));
        toast.show();
    }

    // native 方法

    public native void stitch_e(String featuresType);

    // 不用的 native 方法
    public native void path2Bmp(String imgPath, Bitmap imgSend);
    public native void sendString(String[] imgPaths);
    public native void findPoint(String imgPath, long result);// 查找特征点
    public native void matchPoint(String[] imgPaths, long result);// 匹配特征点
}
