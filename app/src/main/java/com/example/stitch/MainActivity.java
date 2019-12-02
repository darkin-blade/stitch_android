package com.example.stitch;

import androidx.appcompat.app.AppCompatActivity;
import androidx.core.app.ActivityCompat;

import android.content.Context;
import android.content.DialogInterface;
import android.content.pm.PackageManager;
import android.graphics.Bitmap;
import android.graphics.Color;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.Toast;

import org.opencv.android.OpenCVLoader;
import org.opencv.android.Utils;
import org.opencv.core.Mat;
import org.opencv.imgproc.Imgproc;

public class MainActivity extends AppCompatActivity implements DialogInterface.OnDismissListener {
    static public int window_num;
    static public String appPath = null;// app路径

    static public final int MAIN = 0,
            LOCAL_RECOGNIZE = 1,
            LOCAL_RECOGNIZE_PARAM = 2;

    static public LocalRecognize localRecognize;// 本地识别

    // protected
    Button btnLocalRecognize;

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
        setContentView(R.layout.local_recognize);

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

        // 初始化 dialog
        // 初始化 组件
        localRecognize = new LocalRecognize();
    }

    public void initUI() {
        btnLocalRecognize = findViewById(R.id.local_recognize);

        btnLocalRecognize.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                localRecognize.show(getSupportFragmentManager(), "local recognize");
            }
        });
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
    static public native int stitch_e(
            String[] imgPaths,
            String featuresType,
            String wrapType,
            String waveCorrectType,
            long result
    );

    // 不用的 native 方法
    public native void path2Bmp(String imgPath, Bitmap imgSend);
    public native void sendString(String[] imgPaths);
    public native void findPoint(String imgPath, long result);// 查找特征点
    public native void matchPoint(String[] imgPaths, long result);// 匹配特征点

    @Override
    public void onDismiss(DialogInterface dialog) {
        ;// TODO
    }
}
