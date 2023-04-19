#include<iostream>
#include<xratslam/Image.hh>
#include<xratslam/Odometry.hh>
#include<xratslam/XRatSlam.hh>
#include<xratslam/DataReaderVideo.hh>

using namespace std;
using namespace xratslam;

int main()
{
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
    while (true) {
        slam.feed(img, odo);
        vid.readNext(&img, &odo);
        if (img == 0) break;
        
        count++;
        if (count%1000 == 0) {
            cout << "Frame #" << count << " done!" << endl;
            // Image *img_copy(img);
            // (*img_copy).show("OpenCV window", 10);
            cout << "odometry time: " << odo.dTime << endl;
            (*img).show("OpenCV window", 10);
        }    
    }
    cout << "FINISHED!" << endl;
    return 0;
}
