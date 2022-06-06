import py_imu_trajectory

test_file_path = "/media/menonsandu/Chest/Ubuntu/Downloads/dataset-room1_512_16/mav0/imu0/data.csv"
py_imu_trajectory.get_imu_ahrs_output(test_file_path, -1, 100, 1)