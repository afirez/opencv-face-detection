package io.danbogh.facedetection;

import android.graphics.Point;

/**
 * Created by danbogh on 2017/9/13.
 */

public class Utils {
    public static double distance(Point p1, Point p2) {
        return distance(p1.x, p1.y, p2.x, p2.y);
    }

    public static double distance(int x1, int y1, int x2, int y2) {
        return Math.sqrt(Math.pow(x1 - x2, 2) + Math.pow(y1 - y2, 2));
    }
}
