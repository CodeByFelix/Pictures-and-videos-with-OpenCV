#include <opencv2/opencv.hpp>
#include <ctime>
#include <iostream>
#include <string>
#include <sstream>


std::string getFileName (char m)
{
    std::time_t currentTime = std::time(0);
	
	std::string timeString = std::ctime(&currentTime);
	
	std::cout << "Current Time is: " << timeString << std::endl;
	
	std::tm* localTime = std::localtime (&currentTime);
	
	int year = localTime->tm_year + 1900;
	int month = localTime->tm_mon + 1;
	int day = localTime->tm_mday;
	int hour = localTime->tm_hour;
	int minute = localTime->tm_min;
	int seconds = localTime->tm_sec;

    std::ostringstream oss;
    oss << (m == 'v'? "VID" : "IMG") << year << (month < 10? "0": "") << month 
        << (day < 10? "0" : "") << day 
        << (hour < 10? "0" : "") << hour 
        << (minute < 10? "0" : "") << minute 
        << (seconds < 10? "0" : "") << seconds 
        << (m == 'v'? ".mp4" : ".jpg");

    return oss.str();

}

int main ()
{
    cv::VideoCapture cam(0);

    if (!cam.isOpened())
    {
        std::cout << "Error: Unable to open webcam!!!" << std::endl;
        system("pause");
        return -1;
    }
    
    cv::Mat frame;
    bool recording = false;
    cv::VideoWriter videoWriter;

    while (true)
    {

        cam >> frame;
        cv::flip(frame, frame, 1);

        cv::imshow("Webcam", frame);

        char w = cv::waitKey(1);
        //std::cout << "Key Pressed is: " << w << std::endl;
        if (w == 'q')
            break;
        if (w == 's')
        {
            //start recording
            if(!recording)
            {
                std::string& fileName = getFileName('v');
                int frameWidth = cam.get(cv::CAP_PROP_FRAME_WIDTH);
                int frameHeight = cam.get(cv::CAP_PROP_FRAME_HEIGHT);
                videoWriter.open(fileName, cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), 10, cv::Size(frameWidth, frameHeight));
                if(!videoWriter.isOpened())
                {
                    std::cout << "Could not open Video Writer" << std::endl;
                    system("pause");
                    videoWriter.release();
                    //return -1;
                }
                else 
                {
                    recording = true;
                    std::cout << "\n\n\nVideo Recording started \n\n\n";
                }
                    
                
  
            }
                  
        }
        else if (w == 'b')
        {
            //stop recording
            videoWriter.release();
            recording = false;
        }
        else if (w == 'c')
        {
            std::string& fileName = getFileName('i');
            if (cv::imwrite(fileName, frame))
                std::cout << "\nImage saved successfully!\n";
            else
                std::cout << "\nError!!! \nFailed to save image.\n";
        }

        if (recording)
        {
            videoWriter.write(frame);
            std::cout << "\nRecording\n";
        }
    }

    cam.release();
    
    cv::destroyAllWindows();


    return 0;

}