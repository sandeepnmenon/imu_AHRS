# AHRS Cpp Library

This library wraps the official implementation of MadgwickAHRS and Mahony AHRS algorithm to get orientation of an object based on accelerometer and gyroscope readings


# References.
[1] https://x-io.co.uk/open-source-imu-and-ahrs-algorithms

[2] https://github.com/arduino-libraries/MadgwickAHRS

[3] https://github.com/PaulStoffregen/MahonyAHRS/tree/master/src

# TODOs
- [ ] Use AHRS base class to implement Madgwick and Mahony AHRS. Then use command line arguments to select which algorithm to use.
- [x] Add python binding.
- [ ] Realtime data input and visualization.
