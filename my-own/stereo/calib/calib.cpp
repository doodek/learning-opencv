
#include <opencv2/opencv.hpp>
//#include <opencv2/imgcodecs.hpp>
#include <iostream>
#include <vector>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <fstream>

using namespace std;

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

vector<cv::Point3f> createBoardModel(int h, int w, int squareSize)
{
    vector<cv::Point3f> model;

    for(int i = 0; i < h, i++)
    {
        for(int j = 0; j < w; j++)
        {
        model.push_back(
            cv::Point3f((float)(i * squareSize), (float)(j*squareSize), 0.f));
        }
    }
}




int main(int argc, char *argv[])
{
    string leftListFile = argv[1];
    string rightListFile = argv[2];

    int boardWidth = atoi(argv[3]);
    int boardHeight = atoi(argv[4]);
    int boardSquareSize = atoi(argv[5]);

    cout << "Reading the images..." << endl << endl;
    vector<cv::Mat> leftImages  = readImagesFromFile(leftListFile);
    vector<cv::Mat> rightImages = readImagesFromFile(rightListFile);
    cout << "Images read." << endl << endl;

    vector<cv::Point3f> boardModel = createBoardModel(boardHeight, boardWidth, boardSquareSize);
    


    return 0;
}

