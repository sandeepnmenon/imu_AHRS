import os
import py_imu_trajectory

def test_binding():
    assert py_imu_trajectory.test_binding() == "Hello binders!"

def test_ahrs():
    current_file_path = os.path.dirname(os.path.realpath(__file__))
    test_file_path = os.path.join(current_file_path, "test_data.csv")
    output = py_imu_trajectory.get_imu_ahrs_output(test_file_path, 10, 100, 1, False)
    assert len(output) == 10

if __name__ == "__main__":
    test_binding()
    test_ahrs()