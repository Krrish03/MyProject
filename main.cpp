#include "opencv2/highgui/highgui.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <windows.h>
#include <winable.h>
using namespace cv;
using namespace std;

void GenerateKey(int vk, BOOL bExtended) {

    KEYBDINPUT  kb = {0};
    INPUT       Input = {0};

    /* Generate a "key down" */
    if (bExtended) { kb.dwFlags  = KEYEVENTF_EXTENDEDKEY; }
    kb.wVk  = vk;
    Input.type  = INPUT_KEYBOARD;
    Input.ki  = kb;
    SendInput(1, &Input, sizeof(Input));

    /* Generate a "key up" */
    ZeroMemory(&kb, sizeof(KEYBDINPUT));
    ZeroMemory(&Input, sizeof(INPUT));
    kb.dwFlags  =  KEYEVENTF_KEYUP;
    if (bExtended) { kb.dwFlags |= KEYEVENTF_EXTENDEDKEY; }
    kb.wVk = vk;
    Input.type = INPUT_KEYBOARD;
    Input.ki = kb;
    SendInput(1, &Input, sizeof(Input));

    return;
}

int main(int argc, char* argv[])
{
    VideoCapture cap(0); // open the video camera no. 0

    if (!cap.isOpened())  // if not success, exit program
    {
        cout << "Cannot open the video cam" << endl;
        return -1;
    }

   double dWidth = cap.get(CV_CAP_PROP_FRAME_WIDTH); //get the width of frames of the video
   double dHeight = cap.get(CV_CAP_PROP_FRAME_HEIGHT); //get the height of frames of the video

    cout << "Frame size : " << dWidth << " x " << dHeight << endl;

    namedWindow("MyVideo",CV_WINDOW_AUTOSIZE); //create a window called "MyVideo"

    Point constPoint;
    while (1)
    {
        Mat frame;
        bool bSuccess;
        //for(register int falseRead = 0; falseRead<10;++falseRead)
        bSuccess = cap.read(frame); // read a new frame from video

        if (!bSuccess) //if not success, break loop
        {
             cout << "Cannot read a frame from video stream" << endl;
             break;
        }
        cvtColor(frame, frame, CV_BGR2GRAY);

        // Load nose cascade (.xml file)
        CascadeClassifier nose_cascade;
        nose_cascade.load( "C:/opencv/opencv/build/etc/haarcascades/Nariz.xml" );
        // Detect nose
        std::vector<Rect> nose;
        nose_cascade.detectMultiScale( frame, nose, 1.1, 3, 0|CV_HAAR_SCALE_IMAGE, Size(30, 30));

        // Draw circles on the detected nose
        Point center;
        for( int i = 0; i < (int)nose.size(); i++ )
        {
            center = Point( nose[i].x + nose[i].width*0.5, nose[i].y + nose[i].height*0.5 );
            //cout<<center<<"\n";
            int thr = 13;
            if(waitKey(60))
            if(abs(center.x - constPoint.x) < 25){
                if(center.x > constPoint.x && abs(center.x - constPoint.x) >= thr){
                    GenerateKey(VK_LEFT, TRUE);
                    cout<<"Left\n";
                }else if(center.x < constPoint.x  && abs(center.x - constPoint.x) >= thr){
                  GenerateKey(VK_RIGHT, TRUE);
                    cout<<"Right\n";
                }else if(center.y > constPoint.y && abs(center.y - constPoint.y) >= thr){
                   GenerateKey(VK_DOWN, TRUE);
                    cout<<"Down\n";
                }else if(center.y < constPoint.y && abs(center.y - constPoint.y) >= thr){
                    GenerateKey(VK_UP, TRUE);
                    cout<<"Up\n";
                }
            }
            ellipse( frame, center, Size( nose[i].width*0.1, nose[i].height*0.1), 0, 0, 360, Scalar( 255, 0, 255 ), 4, 8, 0 );
        }

        imshow("MyVideo", frame); //show the frame in "MyVideo" window
        if (waitKey(30)) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
       {
           constPoint = center;
            //cout << "Nose set" << endl;
       }


        if (waitKey(30) == 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
       {
            cout << "esc key is pressed by user" << endl;
            break;
       }
    }
    return 0;

}

