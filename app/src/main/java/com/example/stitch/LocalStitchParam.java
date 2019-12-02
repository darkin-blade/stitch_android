package com.example.stitch;

import android.app.Activity;
import android.content.DialogInterface;
import android.graphics.drawable.ColorDrawable;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.Spinner;

import androidx.fragment.app.DialogFragment;
import androidx.fragment.app.FragmentManager;

public class LocalStitchParam extends DialogFragment {
    public View myView;

    Button button_1;
    Spinner featuresList;
    Spinner warpList;
    Spinner waveCorrectList;// 波形矫正


    @Override
    public void show(FragmentManager fragmentManager, String tag) {
        super.show(fragmentManager, tag);
        MainActivity.window_num = MainActivity.LOCAL_RECOGNIZE_PARAM;
    }

    @Override
    public void onDismiss(final DialogInterface dialog) {
        super.onDismiss(dialog);
        Activity activity = getActivity();
        if (activity instanceof DialogInterface.OnDismissListener) {
            ((DialogInterface.OnDismissListener) activity).onDismiss(dialog);
        }
    }

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setStyle(STYLE_NO_FRAME, android.R.style.Theme);// 关闭背景(点击外部不能取消)
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        myView = inflater.inflate(R.layout.stitch_param, container);
        getDialog().getWindow().setBackgroundDrawable(new ColorDrawable(0x00000000));// 背景透明

        initData();
        initUI();

        return myView;
    }

    public void initData() {
        MainActivity.window_num = MainActivity.MAIN;// TODO 修改窗口编号
    }

    public void initUI() {//
        // 按钮
        button_1 = myView.findViewById(R.id.button_1);

        // 下拉菜单
        featuresList = myView.findViewById(R.id.features_type);
        warpList = myView.findViewById(R.id.wrap_type);
        waveCorrectList = myView.findViewById(R.id.wave_correct);

        String[] tmpString = new String[]{"orb", "akaze"};
        ArrayAdapter<String> tmpAdapter = new ArrayAdapter<String>(getContext(), R.layout.spinner_1, tmpString);
        featuresList.setAdapter(tmpAdapter);

        tmpString = new String[]{"plane", "affine", "cylindrical", "spherical"};
        tmpAdapter = new ArrayAdapter<>(getContext(), R.layout.spinner_1, tmpString);
        warpList.setAdapter(tmpAdapter);

        tmpString = new String[]{"horiz", "vert"};
        tmpAdapter = new ArrayAdapter<>(getContext(), R.layout.spinner_1, tmpString);
        waveCorrectList.setAdapter(tmpAdapter);

        button_1.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                // 保存参数
                MainActivity.localStitch.featuresType = (String) featuresList.getSelectedItem();
                MainActivity.localStitch.warpType = (String) warpList.getSelectedItem();
                MainActivity.localStitch.waveCorrectType = (String) waveCorrectList.getSelectedItem();
                dismiss();
            }
        });
    }
}
