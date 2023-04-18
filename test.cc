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
    cout << slam.start() << endl;
    DataReaderVideo vid("./test.mp4");
    cout << 123 << endl;
    vid.setOdometryVisual(configFile);
    cout << 234 << endl;
    Image **img;
    Odometry **odo;
    vid.readNext(img, odo);
    cout << 345 << endl;
    cout << slam.feed(*img, *odo);
    cout << 456 << endl;
    return 0;
}
