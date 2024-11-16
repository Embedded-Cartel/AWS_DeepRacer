import tensorflow as tf
import numpy as np

sess = None
lidar_input_tensor = None
camera_input_tensor = None
output_tensor = None

def initialize_model(model_path):
    global sess, lidar_input_tensor, camera_input_tensor, output_tensor
    lidar_input_tensor_name = "main_level/agent/main/online/network_0/SECTOR_LIDAR/SECTOR_LIDAR:0"
    camera_input_tensor_name = "main_level/agent/main/online/network_0/STEREO_CAMERAS/STEREO_CAMERAS:0"
    output_tensor_name = "main_level/agent/main/online/network_1/ppo_head_0/policy:0"

    # Model Load -> main loop -> initialize_model(YOUR MODEL)
    with tf.io.gfile.GFile(model_path, "rb") as f:
        graph_def = tf.compat.v1.GraphDef()
        graph_def.ParseFromString(f.read())

    # New graph create and load model....
    graph = tf.Graph()
    with graph.as_default():
        tf.import_graph_def(graph_def, name="")

    sess = tf.compat.v1.Session(graph=graph) #session

    # intput and output tensor GET! ( USE -> print-iotensor.py :D )
    lidar_input_tensor = graph.get_tensor_by_name(lidar_input_tensor_name)
    camera_input_tensor = graph.get_tensor_by_name(camera_input_tensor_name)
    output_tensor = graph.get_tensor_by_name(output_tensor_name)

def predict(lidar_data, camera_data):
    global sess, lidar_input_tensor, camera_input_tensor, output_tensor

    # intput data type return and Set the type
    lidar_input_data = np.array(lidar_data, dtype=np.float32).reshape(1, -1)
    camera_input_data = np.array(camera_data, dtype=np.float32).reshape(1, 120, 160, 2)
    
    feed_dict = {
        lidar_input_tensor: lidar_input_data,
        camera_input_tensor: camera_input_data
    }# feed dictionary.....

    # Model run and return result
    result = sess.run(output_tensor, feed_dict=feed_dict)

    print("Raw inference result:", result)

    # return the result(conver to list)
    return result.flatten().tolist()

if __name__ == "__main__":
    initialize_model("/home/ubuntu/workspace-tensor/model.pb")

    # Random input data...... (EXAMPLE -> @TODO : Change here)
    lidar_data = np.random.rand(8)
    camera_data = np.random.rand(120, 160, 2)

    # Predit
    result = predict(lidar_data, camera_data)
    print("Inference result:", result)
