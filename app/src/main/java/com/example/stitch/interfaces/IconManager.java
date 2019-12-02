package com.example.stitch.interfaces;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;

public class IconManager {
    public Context myContext;

    public IconManager() {
        ;
    }

    public IconManager(Context context) {
        myContext = context;
    }

    public boolean isImg(String imgPath) {// 判断是否是图片
        Bitmap bitmap = null;
        BitmapFactory.Options options = new BitmapFactory.Options();
        options.inJustDecodeBounds = true;// TODO
        bitmap = BitmapFactory.decodeFile(imgPath, options);
        if (options.outHeight == 0 || options.outWidth == 0) {
            return false;
        } else {
            return true;
        }
    }

    public Bitmap LoadThumb(final String imgPath, final int width, final int height) {// 加载缩略图
        if (isImg(imgPath) == false) {
            return null;
        }

        BitmapFactory.Options options = new BitmapFactory.Options();
        options.inJustDecodeBounds = true;// TODO 此时decode的bitmap为null
        Bitmap bitmap = BitmapFactory.decodeFile(imgPath, options);
        options.inJustDecodeBounds = false;// TODO

        // 缩放
        int h_rate = options.outHeight / height;
        int w_rate = options.outWidth / width;
        int rate = 1;
        if (h_rate < w_rate) {
            rate = h_rate;
        } else {
            rate = w_rate;
        }
        options.inSampleSize = rate;
        bitmap = BitmapFactory.decodeFile(imgPath, options);

        return bitmap;
    }

}

