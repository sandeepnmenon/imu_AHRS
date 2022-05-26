#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <imu_trajectory_utils.hpp>
namespace py = pybind11;

string TestBinding() {
    return "Hello binders!";
}

PYBIND11_MODULE(py_imu_trajectory, module) {
    module.doc() = "pybind11 plugin for calculating trajectory of IMU using IMU readings.";

    module.def("test_binding", &TestBinding, "Test Binding Function");
    module.def("get_imu_ahrs_output", &get_imu_ahrs_output, 
    "get_imu_ahrs_output", 
     py::arg("file_path"), py::arg("num_rows"), py::arg("sampleFrequency"), py::arg("useEuler"));
}