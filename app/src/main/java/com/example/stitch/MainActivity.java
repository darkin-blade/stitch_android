package com.example.stitch;

import androidx.appcompat.app.AppCompatActivity;
import androidx.core.app.ActivityCompat;

import android.content.pm.PackageManager;
import android.graphics.Bitmap;
import android.os.Bundle;
import android.util.Size;
import android.widget.ImageView;
import android.widget.TextView;

import com.example.stitch.utils.ImgSize;

public class MainActivity extends AppCompatActivity {
    static public int window_num;
    static public String appPath = null;// app路径
    public int isExit;

    static public final int MAIN = 0;
    static public final int LOCAL_RECOGNIZE = 1;// 本地选取图片
    static public final int TAKE_PICTURES = 2;// 拍照获取图片
    static public final int SELECT_IMG = 3;// 本地文件管理器
    static public final int SAVE_IMG = 4;// 保存合并的图片到本地


    // TODO
    static {
        System.loadLibrary("native-lib");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        init();
    }

    public void init() {// 检查权限
        String permission = "android.permission.WRITE_EXTERNAL_STORAGE";
        int check_result = ActivityCompat.checkSelfPermission(this, permission);// `允许`返回0,`拒绝`返回-1
        if (check_result != PackageManager.PERMISSION_GRANTED) {// 没有`写`权限
            ActivityCompat.requestPermissions(this, new String[]{permission}, 1);// 获取`写`权限
        }

        // 初始化路径字符串
        appPath = getExternalFilesDir("").getAbsolutePath();

        // 字符串测试
        TextView tv = findViewById(R.id.sample_text);
        tv.setText(stringFromJNI());

        // TODO 获取图片大小
        ImgSize imgSize = new ImgSize(100, 101);
        getImgSize("", imgSize);

//        Bitmap bitmap = null;
//        ImageView imageView = findViewById(R.id.sample_img);
    }

    // TODO native 方法
    public native String stringFromJNI();

    public native int changeImg(Bitmap imgSend);

    public native void getImgSize(String imgPath, ImgSize imgSize);
}
