#include "imagesaver.h"



int test_imagesaver()
{
    vector<Vec3f> testdata;
    for(int i=0; i<20; i++)
    {
        Vec3f data;
        data.x = 0.2;
        data.y = 0.2;
        data.z = 0.0;
        testdata.push_back(data);
    }
    for(int i=0; i<testdata.size(); i++)
    {
        cout << testdata[i] << endl;
    }
    Vec3f *data = &testdata[0];
    save_ppm_image("test_image1.ppm", 4, 5, data);
    cout << "try again";
    return 0;
}