package com.example.stitch;

import android.graphics.Bitmap;
import android.graphics.drawable.ColorDrawable;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.ImageView;

import androidx.fragment.app.DialogFragment;
import androidx.fragment.app.FragmentManager;

import org.opencv.android.Utils;
import org.opencv.core.Mat;
import org.opencv.imgproc.Imgproc;

public class LocalRecognize extends DialogFragment {
    static public String featuresType = "orb";
    static public String warpType = "spherical";
    static public String waveCorrectType = "horiz";

    static public LocalRecognizeParam localRecognizeParam;

    // protected
    public View myView;

    Button button_1;
    Button button_2;

    @Override
    public void show(FragmentManager fragmentManager, String tag) {
        super.show(fragmentManager, tag);
        MainActivity.window_num = MainActivity.LOCAL_RECOGNIZE;
    }

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setStyle(STYLE_NO_FRAME, android.R.style.Theme);// 关闭背景(点击外部不能取消)
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        myView = inflater.inflate(R.layout.local_recognize, container);
        getDialog().getWindow().setBackgroundDrawable(new ColorDrawable(0x00000000));// 背景透明

        initData();
        initUI();

        return myView;
    }

    public void initData() {
        localRecognizeParam = new LocalRecognizeParam();
    }

    public void initUI() {
        button_1 = myView.findViewById(R.id.button_1);
        button_2 = myView.findViewById(R.id.button_2);

        button_1.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                simpleTest();
            }
        });

        button_2.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                localRecognizeParam.show(getFragmentManager(), "set params");
            }
        });
    }

    public void simpleTest() {
        String[] imgPaths = new String[2];

        for (int i = 0; i < 2; i ++) {
            imgPaths[i] = MainActivity.appPath + "/boat" + (i + 1) + ".jpg";
        }

        Mat matBGR = new Mat();
        int result = MainActivity.stitch_e(
                imgPaths,
                featuresType,
                warpType,
                waveCorrectType,
                matBGR.getNativeObjAddr()
        );

        if (result != 0) {
            MainActivity.infoLog("failed");
            return;
        } else {
            MainActivity.infoLog("mat size " + matBGR.cols() + ", " + matBGR.rows());
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
        ImageView imageView = myView.findViewById(R.id.sample_img);
        imageView.setImageBitmap(bitmap);
    }
}
