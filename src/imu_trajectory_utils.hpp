#include <cmath>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>

using namespace std;

// Returns IMU readings of the form <timestamp, gx, gy, gz, ax, ay, az>
vector<vector<double>> read_IMU_from_csv(const string file_path, const string delimiter, const int num_rows);

// Write IMU readings to a csv file
void write_IMU_to_csv(const string file_path, const string delimiter, const vector<vector<double>> data, const vector<string> header);

// Convert radians to degrees
double convert_radians_to_degree(double angle);

// Returns IMU readings of the form <timestamp, qw, qx, qy, qz> or <timestamp, roll, pitch, yaw>
vector<vector<double>> get_imu_ahrs_output(string file_path, const int num_rows, const int sampleFrequency, const int useEuler, const bool verbose=false);
