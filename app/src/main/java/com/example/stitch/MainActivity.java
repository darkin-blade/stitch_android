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
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.Toast;

import org.opencv.android.OpenCVLoader;
import org.opencv.android.Utils;
import org.opencv.core.Mat;
import org.opencv.imgproc.Imgproc;

import java.util.ArrayList;

public class MainActivity extends AppCompatActivity {
    static public int window_num;
    static public String appPath = null;// app路径
    public int isExit;

    static public final int MAIN = 0;
    static public final int LOCAL_RECOGNIZE = 1;// 本地选取图片
    static public final int TAKE_PICTURES = 2;// 拍照获取图片
    static public final int SELECT_IMG = 3;// 本地文件管理器
    static public final int SAVE_IMG = 4;// 保存合并的图片到本地

    // 初始化opencv java
    static {
        if (!OpenCVLoader.initDebug()) {
            infoLog("opencv init failed");
        }
    }

    // TODO
    static {
        System.loadLibrary("native-lib");// TODO
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        infoLog("on create");

        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        init();
    }

    public void init() {// 检查权限
        infoLog("init");

        String permission = "android.permission.WRITE_EXTERNAL_STORAGE";
        int check_result = ActivityCompat.checkSelfPermission(this, permission);// `允许`返回0,`拒绝`返回-1
        if (check_result != PackageManager.PERMISSION_GRANTED) {// 没有`写`权限
            ActivityCompat.requestPermissions(this, new String[]{permission}, 1);// 获取`写`权限
        }

        // 初始化路径字符串
        appPath = getExternalFilesDir("").getAbsolutePath();

        midTest();
    }

    public void simpleTest() {
        // 字符串测试
        TextView tv = findViewById(R.id.sample_text);
        tv.setText(stringFromJNI());

        // TODO 测试
        sendString(new String[]{"mac1.jpg", "mac2.jpg"});
    }

    public void midTest() {
        infoLog("mid test");

        Mat matBGR = new Mat();
        // 查找特征点
//        findPoint(appPath + "/img0.png", matBGR.getNativeObjAddr());
        // 匹配特征点
        matchPoint(new String[]{appPath + "/mac1.jpg", appPath + "/mac2.jpg"}, matBGR.getNativeObjAddr());
        Bitmap bitmap = Bitmap.createBitmap(matBGR.cols(), matBGR.rows(), Bitmap.Config.ARGB_8888);

        // BGR转RGB
        Mat matRGB = new Mat();
        Imgproc.cvtColor(matBGR, matRGB, Imgproc.COLOR_BGR2RGB);
        Utils.matToBitmap(matRGB, bitmap);


        // TODO 显示图片
        ImageView imageView = findViewById(R.id.sample_img);
        imageView.setImageBitmap(bitmap);
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

    // TODO native 方法
    public native String stringFromJNI();

    public native void path2Bmp(String imgPath, Bitmap imgSend);

    public native void sendString(String[] imgPaths);

    public native void findPoint(String imgPath, long result);// 查找特征点

    public native void matchPoint(String[] imgPaths, long result);// 匹配特征点
}
