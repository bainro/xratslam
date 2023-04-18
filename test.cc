#include<iostream>
#include<xratslam/Image.hh>
#include<xratslam/Odometry.hh>
#include<xratslam/XRatSlam.hh>
#include<xratslam/DataReaderVideo.hh>

using namespace std;
using namespace xratslam;

int main()
{
    cout << "Hello World!" << endl;
    string configFile = "";
    XRatSlam slam(configFile);
    cout << "starting slam!" << endl;
    slam.start();
    DataReaderVideo vid("./test.mp4");
    vid.setOdometryVisual(configFile);
    Image *img;
    Odometry *odo;
    vid.readNext(&img, &odo);
    int count = 0;
    while (img != 0) {
        slam.feed(img, odo);
        count++;
        if (count%100 == 0) {
            cout << "Frame #" << count << " done!" << endl;
            Image *img_copy(&img);
            (*img_copy).show("OpenCV window", 10);
        }    
        vid.readNext(&img, &odo);
    }
    cout << "FINISHED!" << endl;
    return 0;
}
