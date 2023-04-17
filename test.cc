#include<iostream>
#include<xratslam/XRatSlam.hh>

using namespace std;
using namespace xratslam;

int main()
{
    cout << "Hello World!";
    string configFile = "";
    XRatSlam slam(configFile);
    slam::start();
    return 0;
}
