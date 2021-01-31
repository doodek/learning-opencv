#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/core/matx.hpp>

int main(void)
{   

    cv::Mat mat(3, 3, CV_64FC1);
    cv::Vec<int, 3> vec(0, 0, 0);
    
    cv::Mat vecmat  = cv::Mat(vec);
    cv::Mat out =  vecmat * mat; 
    

}
