# import modules
import pandas as pd
import numpy as np
from scipy.integrate import cumtrapz
from numpy import sin,cos,pi
import matplotlib.pyplot as plt
from mpl_toolkits import mplot3d
plt.style.use('seaborn')

# Transform body frame accelerations into the inertial (Earth) frame
# Set up rotation matrices
def R_x(x):
    # body frame rotation about x axis
    return np.array([[1,      0,       0],
                     [0,cos(-x),-sin(-x)],
                     [0,sin(-x), cos(-x)]])
def R_y(y):
    # body frame rotation about y axis
    return np.array([[cos(-y),0,-sin(-y)],
                    [0,      1,        0],
                    [sin(-y), 0, cos(-y)]])
def R_z(z):
    # body frame rotation about z axis
    return np.array([[cos(-z),-sin(-z),0],
                     [sin(-z), cos(-z),0],
                     [0,      0,       1]])


if __name__ == '__main__':
    imu_data_path = "/home/menonsandu/Downloads/dataset-corridor1_512_16/mav0/imu0/data_1.csv"
    imu_pose_path = "/home/menonsandu/Downloads/dataset-corridor1_512_16/mav0/imu0/data_euler_trajectory.csv"
    imu_pose_quat_path = "/home/menonsandu/Downloads/dataset-corridor1_512_16/mav0/imu0/data_quat_trajectory.csv"
    sample_frequency = 200

    nrows = None
    df = pd.read_csv(imu_data_path, nrows=nrows)
    pose_df = pd.read_csv(imu_pose_path, nrows=nrows)
    pose_quat_df = pd.read_csv(imu_pose_quat_path, nrows=nrows)

    # df[df.columns[0]]*=1e-9 
    # add poses to df
    df = df.join(pose_df)
    df = df.join(pose_quat_df[['qw','qx','qy','qz']])

    accel = np.array([df['ax'], df['ay'], df['az']])

    # Set up arrays to hold Euler angles for rotation matrices
    pitch = df["pitch"]
    roll = df["roll"]
    yaw = df["yaw"]

    earth_accels = np.empty(accel.shape)
    # Perform frame transformations (body frame --> earth frame) 
    for i in range(df.shape[0]):
        # accel_earth = (RzRyRx)(accel_body)
        earth_accels[:,i] = R_z(yaw[i]) @ R_y(roll[i]) @ R_x(pitch[i]) @ accel[:,i]


    # Create new columns in dataframe for earth frame accelerations  
    df['EARTH ACCELERATION X'] = earth_accels[0,:]
    df['EARTH ACCELERATION Y'] = earth_accels[1,:]
    df['EARTH ACCELERATION Z'] = earth_accels[2,:]

    # TODO: Remove gravity
    
    dt = 1/sample_frequency
    initial_velocity = np.array([0,0,0])
    initial_position = np.array([0,0,0])
    # Integrate acceleration to get velocity
    x =cumtrapz(cumtrapz(df['EARTH ACCELERATION X'],dx=dt, initial=initial_velocity[0]),dx=dt, initial=initial_position[0])
    y =cumtrapz(cumtrapz(df['EARTH ACCELERATION Y'],dx=dt, initial=initial_velocity[1]),dx=dt, initial=initial_position[1])
    z =cumtrapz(cumtrapz(df['EARTH ACCELERATION Z'],dx=dt, initial=initial_velocity[2]),dx=dt, initial=initial_position[2])

    df['tx'] = x
    df['ty'] = y
    df['tz'] = z

    # Save the new dataframe to a csv file in the format timestamp [s], x [m], y [m], z [m], qx, qy, qz, qw
    new_df = df[[df.columns[0], 'tx', 'ty', 'tz', 'qx', 'qy', 'qz', 'qw']]
    new_df.to_csv(imu_data_path.replace(".csv","_trajectory.csv"), index=False, header=False)