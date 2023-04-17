#include<iostream>
#include<xratslam/XRatSlam.hh>

using namespace std;
using namespace xratslam;

int main()
{
    cout << "Hello World!" << endl;
    string configFile = "";
    XRatSlam slam(configFile);
    cout << slam.start() << endl;
    return 0;
}
