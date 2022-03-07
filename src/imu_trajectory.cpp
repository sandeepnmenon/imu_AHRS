// #include <AHRS.hpp>
#include <MadgwickAHRS.hpp>
#include <MahonyAHRS.hpp>
#include <iostream>
#include <cmath>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>

#define PI 3.14159265358979323846
using namespace std;

// Returns IMU readings of the form <timestamp, gx, gy, gz, ax, ay, az>
vector<vector<double>> read_IMU_from_csv(const string file_path, const string delimiter, const int num_rows);

// Write IMU readings to a csv file
void write_IMU_to_csv(const string file_path, const string delimiter, const vector<vector<double>> data, const vector<string> header);

double convert_radians_to_degree(double angle);

int main(int argc, char **argv)
{
    const string file_path = string(argv[1]);
    const int num_rows = stoi(argv[2]);
    const int sampleFrequency = stoi(argv[3]);

    // Vector to store timestamp, accelerometer and gyroscope data
    vector<vector<double>> data = read_IMU_from_csv(file_path, ",", num_rows);
    double ax, ay, az;
    double gx, gy, gz;
    double timestamp;

    // Initialise the filter
    // TODO: Use factory pattern to initialise the filter
    Madgwick filter;
    // Mahony filter;
    
    filter.begin(sampleFrequency);

    // Initialise the angles
    double roll = 0.0;
    double pitch = 0.0;
    double yaw = 0.0;
    vector<vector<double>> angles_list;

    // Initialise the quaternion
    vector<vector<double>> quaternion_list;

    // Initialise the output data vector
    vector<vector<double>> output_data;

    // Iterate through data and get the angles and quarternion
    for (unsigned int i = 0; i < data.size(); i++) {
        // Get the accelerometer and gyroscope data
        gx = convert_radians_to_degree(data[i][1]);
        gy = convert_radians_to_degree(data[i][2]);
        gz = convert_radians_to_degree(data[i][3]);
        ax = data[i][4];
        ay = data[i][5];
        az = data[i][6];

        // Get the timestamp
        timestamp = data[i][0]*1e-9;

        // Update the filter
        filter.updateIMU(gx, gy, gz, ax, ay, az);

        // Get the angles in degrees
        roll = filter.getRoll();
        pitch = filter.getPitch();
        yaw = filter.getYaw();

        cout<<"Roll: "<<roll<<" Pitch: "<<pitch<<" Yaw: "<<yaw<<endl;
        
        // Get the quaternion and timestamp
        vector<double> quat = filter.getQuarternion();
        
        // cout<<"Quaternion: "<<quat[0]<<" "<<quat[1]<<" "<<quat[2]<<" "<<quat[3]<<endl;

        // Store the angles and quaternion
        vector<double> angles = {roll, pitch, yaw};
        angles_list.push_back(angles);
        quaternion_list.push_back(quat);

        output_data.push_back({timestamp, quat[0], quat[1], quat[2], quat[3]});
    }

    // Write the angles and quaternion to a csv file
    const string output_file_path = file_path.substr(0, file_path.find_last_of(".")) + "_trajectory.csv";
    const vector<string> header = {"timestamp", "qw", "qx", "qy", "qz"};
    write_IMU_to_csv(output_file_path, ",", output_data, header);
    cout<<"Output file path: "<<output_file_path<<endl;

    return 0;
}


vector<vector<double>> read_IMU_from_csv(const string file_path, const string delimiter = ",", const int num_rows = -1) {
    vector<vector<double>> data;
    ifstream file(file_path);
    string line;
    int count = 0;
    while (getline(file, line)) {
        stringstream ss(line);
        // skip header
        if (line.find("timestamp") != string::npos) {
            continue;
        }
        vector<double> record;
        string field;
        while (getline(ss, field, delimiter[0])) {            
            record.push_back(stod(field));
        }
        data.push_back(record);
        count++;
        if (num_rows > 0 && count >= num_rows) {
            break;
        }
    }

    return data;
}

void write_IMU_to_csv(const string file_path, const string delimiter, const vector<vector<double>> data, const vector<string> header) {
    ofstream file(file_path);
    for (unsigned int i = 0; i < header.size(); i++) {
        file << header[i] << delimiter;
    }
    file << endl;
    for (unsigned int i = 0; i < data.size(); i++) {
        for (unsigned int j = 0; j < data[i].size(); j++) {
            file << data[i][j] << delimiter;
        }
        file << endl;
    } 
}

double convert_radians_to_degree(double degrees)
{
    return degrees * 180.0 / PI;
}