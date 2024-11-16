import tensorflow as tf
import numpy as np

# Global variables for session and tensors
sess = None
lidar_input_tensor = None
camera_input_tensor = None
output_tensor = None

"""
    initialize_model() - Loads and initializes the reinforcement learning model.
    @model_path: The file path to the TensorFlow model (.pb file) to be loaded.

    This function loads the pre-trained TensorFlow model from the specified path
    and initializes the necessary TensorFlow session and tensors. It imports the
    model graph and retrieves the input and output tensors used for inference.
    This function is designed to initialize the model only once by checking if 
    the session is already active.

    Context: Process context. The function should be called once during the setup phase.
"""
def initialize_model(model_path):
    print("[INFO] Python init start")
    global sess, lidar_input_tensor, camera_input_tensor, output_tensor
    if sess is None:  # Initialize only if session is not active
        lidar_input_tensor_name = "main_level/agent/main/online/network_0/SECTOR_LIDAR/SECTOR_LIDAR:0"
        camera_input_tensor_name = "main_level/agent/main/online/network_0/STEREO_CAMERAS/STEREO_CAMERAS:0"
        output_tensor_name = "main_level/agent/main/online/network_1/ppo_head_0/policy:0"

        with tf.io.gfile.GFile(model_path, "rb") as f:
            graph_def = tf.compat.v1.GraphDef()
            graph_def.ParseFromString(f.read())

        graph = tf.Graph()
        with graph.as_default():
            tf.import_graph_def(graph_def, name="")

        sess = tf.compat.v1.Session(graph=graph)
        lidar_input_tensor = graph.get_tensor_by_name(lidar_input_tensor_name)
        camera_input_tensor = graph.get_tensor_by_name(camera_input_tensor_name)
        output_tensor = graph.get_tensor_by_name(output_tensor_name)
        print("-"*50)
        print("[INFO] Python init END")

"""
    predict() - Run inference using lidar and camera data.
    @lidar_data: List of float values representing the processed lidar data input.
    @camera_data: List of float values representing the processed camera data input.

    This function performs inference using the initialized reinforcement learning model
    by feeding lidar and camera input data to the model's input tensors. The function 
    outputs a probability distribution for different driving actions based on the model's 
    policy output tensor. 

    The output probabilities correspond to model-determined control actions.

    Context: Process context. The model must be initialized via initialize_model().
    Return:
    * List[float] - A list of probabilities corresponding to the model's output for
                    different driving actions.
"""
def predict(lidar_data, camera_data):
    global sess, lidar_input_tensor, camera_input_tensor, output_tensor
    lidar_input_data = np.array(lidar_data, dtype=np.float32).reshape(1, -1)
    camera_input_data = np.array(camera_data, dtype=np.float32).reshape(1, 120, 160, 2)

    feed_dict = {
        lidar_input_tensor: lidar_input_data,
        camera_input_tensor: camera_input_data
    }

    # 모델 출력값 계산
    logits = sess.run(output_tensor, feed_dict=feed_dict)
    
    # 소프트맥스 적용
    probabilities = tf.nn.softmax(logits).numpy()  # 즉시 실행 모드에 맞춰 NumPy로 변환
    
    return probabilities.flatten().tolist()
