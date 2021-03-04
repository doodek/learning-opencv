#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>  //some could be obsolete.
#include <iostream>
#include <string>
int main(int argc, char *argv[])
{
    
    if(argc != 2)
    {
        std::cout << "usage: ./show <cam-id>" << std::endl;
        return -1;
    }
    
    int camID = argv[1][0] - '0';
   
    cv::VideoCapture cap(camID);

    if(!cap.isOpened())
    {
        std::cerr << "Cannot open camera " << camID << ". Aborting. " << std::endl;
        return -2;
    }

    std::stringstream ss;
    ss << "cam " << camID;
    std::string windowName = ss.str();
    cv::namedWindow(windowName);
    
    cv::Mat frame;

    cv::Mat editedFrame;

    while(1)
    {
        cap >> frame;

        cv::Sobel(frame, editedFrame, CV_8U, 1, 1 );


        try
        {
            cv::imshow(windowName, editedFrame);
            if(cv::waitKey(5) >= 0)
            {
                break;
            }
        }
        catch(std::exception& e)
        {
            std::cerr <<  e.what() << std::endl;
            std::cerr << "Press any key to leave. " << std::endl;
        }                      
    }           

    return 0;
} 
