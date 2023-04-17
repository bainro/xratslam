#include<iostream>
#include<xratslam/XRatSlam.hh>

using namespace std;
using namespace xratslam;

int main()
{
    cout << "Hello World!";
    string configFile = "";
    XRatSlam slam = XRatSlam("");
    slam::start();
    return 0;
}
