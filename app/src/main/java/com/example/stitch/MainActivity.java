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
import android.widget.ImageView;
import android.widget.Spinner;
import android.widget.TextView;
import android.widget.Toast;

import org.opencv.android.OpenCVLoader;
import org.opencv.android.Utils;
import org.opencv.core.Mat;
import org.opencv.imgproc.Imgproc;

public class MainActivity extends AppCompatActivity {
    static public int window_num;
    static public String appPath = null;// app路径

    static public final int MAIN = 0;

    Spinner featuresList;
    Spinner warpList;
    Spinner waveCorrectList;// 波形矫正
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
        // 按钮
        button_1 = findViewById(R.id.button_1);

        // 下拉菜单
        featuresList = findViewById(R.id.features_type);
        warpList = findViewById(R.id.wrap_type);

        String[] tmpString = new String[]{"orb", "akaze"};
        ArrayAdapter<String> tmpAdapter = new ArrayAdapter<String>(this, R.layout.spinner_1, tmpString);
        featuresList.setAdapter(tmpAdapter);

        tmpString = new String[]{"plane", "affine", "cylindrical", "spherical"};
        tmpAdapter = new ArrayAdapter<>(this, R.layout.spinner_1, tmpString);
        warpList.setAdapter(tmpAdapter);

        button_1.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                simpleTest();
            }
        });
    }

    public void simpleTest() {
        String featuresType = (String) featuresList.getSelectedItem();
        String warpType = (String) warpList.getSelectedItem();
        String[] imgPaths = new String[2];

        for (int i = 0; i < 2; i ++) {
            imgPaths[i] = appPath + "/boat" + (i + 1) + ".jpg";
        }

        Mat matBGR = new Mat();
        int result = stitch_e(
                imgPaths,
                featuresType,
                warpType,
                matBGR.getNativeObjAddr()
        );

        if (result != 0) {
            infoLog("failed");
            return;
        } else {
            infoLog("mat size " + matBGR.cols() + ", " + matBGR.rows());
            if (matBGR.cols() * matBGR.rows() == 0) {
                return;
            }
        }

        Bitmap bitmap = Bitmap.createBitmap(matBGR.cols(), matBGR.rows(), Bitmap.Config.ARGB_8888);

        // BGR转RGB
        Mat matRGB = new Mat();
        Imgproc.cvtColor(matBGR, matRGB, Imgproc.COLOR_BGR2RGB);
        Utils.matToBitmap(matRGB, bitmap);

        // 显示图片
        ImageView imageView = findViewById(R.id.sample_img);
        imageView.setImageBitmap(bitmap);
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

    public native int stitch_e(
            String[] imgPaths,
            String featuresType,
            String wrapType,
            long result
    );

    // 不用的 native 方法
    public native void path2Bmp(String imgPath, Bitmap imgSend);
    public native void sendString(String[] imgPaths);
    public native void findPoint(String imgPath, long result);// 查找特征点
    public native void matchPoint(String[] imgPaths, long result);// 匹配特征点
}
