#include "opencv2/opencv.hpp"
 
using namespace cv;
using namespace std;
 
void blob (string path, float *x, float *y ) {
        Mat bgr_image = cv::imread("blob.jpg");
        Mat hsv_image;
        cvtColor(bgr_image, hsv_image, cv::COLOR_BGR2HSV);
        Mat lower_red_hue_range;
        Mat upper_red_hue_range;
        Mat red_hue_image;
        inRange(hsv_image, cv::Scalar(0, 100, 100), cv::Scalar(10, 255, 255), lower_red_hue_range);
        inRange(hsv_image, cv::Scalar(160, 100, 100), cv::Scalar(179, 255, 255), upper_red_hue_range);
 
        addWeighted(lower_red_hue_range, 1.0, upper_red_hue_range, 1.0, 0.0, red_hue_image);
        GaussianBlur(red_hue_image, red_hue_image, cv::Size(9, 9), 2, 2);
        threshold(red_hue_image, red_hue_image, 100, 255, THRESH_BINARY_INV);
 
        // Setup SimpleBlobDetector parameters.
        SimpleBlobDetector::Params params;
 
        // Change thresholds
        params.minThreshold = 0;
        params.maxThreshold = 255;
 
        vector<KeyPoint> keypoints;
        SimpleBlobDetector detector(params);
        detector.detect(red_hue_image, keypoints);
        imshow("keypoint1s", red_hue_image);
        Mat im_with_keypoints;
        drawKeypoints(bgr_image, keypoints, im_with_keypoints, Scalar(255,0,0), DrawMatchesFlags::DRAW_RICH_KEYPOINTS );
        imshow("keypoints", im_with_keypoints);
        waitKey(0);
 
        *x = keypoints[0].pt.x;
        *y = keypoints[0].pt.y;
}
