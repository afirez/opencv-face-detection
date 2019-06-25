package io.danbogh.facedetection;

import android.graphics.Bitmap;
import android.graphics.Point;

/**
 * Created by danbogh on 2017/9/12.
 */
public class SmartCropper {

    /**
     * 输入图片扫描边框顶点
     *
     * @param srcBitmap 扫描图片
     * @return 返回顶点数组，以 左上，右上，右下，左下排序
     */
    public static Point[] scan(Bitmap srcBitmap) {
        if (srcBitmap == null) {
            throw new IllegalArgumentException("srcBitmap cannot be null");
        }
        Point[] outPoints = new Point[4];
        nativeScan(srcBitmap, outPoints);
        return outPoints;
    }

    /**
     * 裁剪图片
     *
     * @param srcBitmap  待裁剪图片
     * @param cropPoints 裁剪区域顶点，顶点坐标以图片大小为准.
     *                   length必须是4，点的顺序必须是：左上，右上，右下，左下
     * @return 返回裁剪后的图片
     */
    public static Bitmap crop(Bitmap srcBitmap, Point[] cropPoints) {
        if (srcBitmap == null) {
            throw new IllegalArgumentException("srcBitmap cannot be null");
        }
        if (cropPoints == null) {
            throw new IllegalArgumentException("cropPoints cannot be null");
        }
        if (cropPoints.length != 4) {
            throw new IllegalArgumentException("The length of cropPoints must be 4");
        }
        Point leftTop = cropPoints[0];
        Point rightTop = cropPoints[1];
        Point rightBottom = cropPoints[2];
        Point leftBottom = cropPoints[3];
        int cropWidth = (int) ((Utils.distance(leftTop, rightTop)
                + Utils.distance(leftBottom, rightBottom)) / 2);
        int cropHeight = (int) ((Utils.distance(leftTop, leftBottom)
                + Utils.distance(rightTop, rightBottom)) / 2);
        Bitmap cropBitmap = Bitmap.createBitmap(cropWidth, cropHeight, Bitmap.Config.ARGB_8888);
        nativeCrop(srcBitmap, cropPoints, cropBitmap);
        return cropBitmap;
    }

    private static native void nativeScan(Bitmap srcBitmap, Point[] outPoints);

    private static native void nativeCrop(Bitmap srcBitmap, Point[] points, Bitmap outBitmap);

    static {
        System.loadLibrary("smart_cropper");
    }
}
