
//#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/opencv_modules.hpp>
#include <opencv2/aruco/charuco.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <iostream>
#include <vector>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <fstream>

using namespace std;


struct boardDimensions{
    int cols;
    int rows;
    float squareSize;
    float markerLength;
};


vector<string> readFileToStringVect(string path)
{
    vector<string> list; 
    string entry;
    ifstream file(path);
    if(file.is_open())
    {
        while(!file.eof())
        {
            file >> entry;
            list.push_back(entry);
        }   
    }
    return list;
}

vector<cv::Mat> readImagesFromFile(string path)
{
    vector<cv::Mat> images;
    vector<string> list = readFileToStringVect(path);
    
    for(string entry : list)
    {
        images.push_back(cv::imread(path));   
        cout << "Read " << path << endl;
    }
} 

vector<cv::Point3f> createBoardModel(int h, int w, int squareSize)
{
    vector<cv::Point3f> model;

    for(int i = 0; i < h; i++)
    {
        for(int j = 0; j < w; j++)
        {
        model.push_back(
            cv::Point3f((float)(i * squareSize), (float)(j*squareSize), 0.f));
        }
    }
}


double calibrateCamera(vector<cv::Mat> images, boardDimensions * brd, int dictionaryId, cv::Mat cameraMatrix, cv::Mat distCoeffs)
{

    cv::Ptr<cv::aruco::Dictionary> dictionary = cv::aruco::getPredefinedDictionary(cv::aruco::PREDEFINED_DICTIONARY_NAME(dictionaryId));
    cv::Ptr<cv::aruco::DetectorParameters> detectorParams = cv::aruco::DetectorParameters::create();

    cv::Ptr<cv::aruco::CharucoBoard> charucoBoard = cv::aruco::CharucoBoard::create(brd->cols, brd->rows, brd->squareSize, brd->markerLength, dictionary);
    cv::Ptr<cv::aruco::Board> board = charucoBoard.staticCast<cv::aruco::Board>();

    //<image<markers<4points>>>
    vector<vector<vector<cv::Point2f>>> allCorners;
    vector<vector<int>> allIds;

    for(cv::Mat img : images)
    {
        
        vector<int> ids;
        vector<vector<cv::Point2f>> corners, rejected;
        cv::aruco::detectMarkers(img, dictionary, corners, ids, detectorParams, rejected);
        cv::aruco::refineDetectedMarkers(img, board, corners, ids, rejected);
        cv::Mat currentCharucoCorners, currentCharucoIds;

        if(ids.size() > 0) cv::aruco::interpolateCornersCharuco(corners, ids, img, charucoBoard, currentCharucoCorners, currentCharucoIds);

        allCorners.push_back(corners);
        allIds.push_back(ids);
    }

    vector<vector<cv::Point2f>>  allCornersConcatenated;
    vector<int> allIdsConcatenated;
    vector<int> markersPerFrame;

    markersPerFrame.reserve(allCorners.size());
    for(unsigned int i = 0; i < allCorners.size(); i++){
        markersPerFrame.push_back((int)allCorners[i].size());
        for(unsigned int j = 0; j < allCorners[i].size(); j++){
            allCornersConcatenated.push_back(allCorners[i][j]);
            allIdsConcatenated.push_back(allIds[i][j]);
        }
    }

    cv::Mat rvecs, tvecs;
    cv::Size imgSize = images[1].size();


    double arucoRepErr = cv::aruco::calibrateCameraAruco(
        allCornersConcatenated,
        allIdsConcatenated,
        markersPerFrame,
        board,
        imgSize,
        cameraMatrix,
        distCoeffs,
        cv::noArray(),
        cv::noArray()
    );

    int nFrames = (int) allCorners.size();
    vector<cv::Mat> allCharucoCorners;
    vector<cv::Mat> allCharucoIds;
    vector<cv::Mat> filteredImages;
    allCharucoCorners.reserve(nFrames);
    allCharucoIds.reserve(nFrames);

    for(int i = 0; i < nFrames; i++)
    {
        cv::Mat currentCharucoCorners, currentCharucoIds;
        cv::aruco::interpolateCornersCharuco(
            allCorners[i], 
            allIds[i], 
            images[i],
            charucoBoard,
            currentCharucoCorners,
            currentCharucoIds,
            cameraMatrix,
            distCoeffs
        );

        allCharucoIds.push_back(currentCharucoIds);
        allCharucoIds.push_back(currentCharucoCorners);
        filteredImages.push_back(images[i]);
    }

    //reprojection error
    double charucoRepErr = cv::aruco::calibrateCameraCharuco(
        allCharucoCorners,
        allCharucoIds,
        charucoBoard,
        imgSize,
        cameraMatrix,
        distCoeffs,
        rvecs,
        tvecs
    );

    cout << "Rep Error: " << charucoRepErr << endl;
    cout << "Aruco Rep Error: " << arucoRepErr << endl;

    return charucoRepErr;
}


int main(int argc, char *argv[])
{
    string leftListFile = argv[1];
    string rightListFile = argv[2];

    boardDimensions * brd;
    brd->cols = atoi(argv[3]);
    brd->rows = atoi(argv[4]);
    brd->squareSize = atoi(argv[5]);
    brd->markerLength = atoi(argv[6]);

    
    cout << "Reading the images..." << endl << endl;
    vector<cv::Mat> leftImages  = readImagesFromFile(leftListFile);
    vector<cv::Mat> rightImages = readImagesFromFile(rightListFile);
    cout << "Images read." << endl << endl;

    cv::Mat leftCameraMatrix;
    cv::Mat leftDistCoeffs;
    cv::Mat rightCameraMatrix;
    cv::Mat rightDistCoeffs;

    double leftReprojectionError = calibrateCamera(leftImages, brd, 1, leftCameraMatrix, leftDistCoeffs);
    double rightReprojectionError = calibrateCamera(rightImages, brd, 1, rightCameraMatrix, rightDistCoeffs);

    //vector<cv::Point3f> boardModel = createBoardModel(leftImages, brd, 1);
    

    return 0;
}

