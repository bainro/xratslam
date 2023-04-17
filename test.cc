#include<iostream>
#include<xratslam/XRatSlam.hh>

using namespace std;

int main()
{
    cout << "Hello World!";
    string configFile = "";
    XRatSlam slam = xratslam::XRatSlam("");
    slam::start();
    return 0;
}
