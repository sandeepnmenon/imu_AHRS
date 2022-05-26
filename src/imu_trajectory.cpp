// #include <AHRS.hpp>
#include <MadgwickAHRS.hpp>
#include <MahonyAHRS.hpp>
#include <iostream>
#include <cmath>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <imu_trajectory_utils.hpp>

#define PI 3.14159265358979323846
using namespace std;


int main(int argc, char **argv)
{
    const string file_path = string(argv[1]);
    const int num_rows = stoi(argv[2]);
    const int sampleFrequency = stoi(argv[3]);
    const int useEuler = stoi(argv[4]);

    // Initialise the output data vector
    vector<vector<double>> output_data = get_imu_ahrs_output(file_path, num_rows, sampleFrequency, useEuler);

    // Write the angles and quaternion to a csv file
    if (useEuler == 1) {
        const string output_file_path = file_path.substr(0, file_path.find_last_of(".")) + "_euler_trajectory.csv";
        vector<string> header = {"timestamp", "roll", "pitch", "yaw"};
        write_IMU_to_csv(output_file_path, ",", output_data, header);
        cout<<"Output file path: "<<output_file_path<<endl;
    } else {
        const string output_file_path = file_path.substr(0, file_path.find_last_of(".")) + "_quat_trajectory.csv";
        vector<string> header = {"timestamp", "qw", "qx", "qy", "qz"};
        write_IMU_to_csv(output_file_path, ",", output_data, header);
        cout<<"Output file path: "<<output_file_path<<endl;
    }

    return 0;
}
