# import tensorflow as tf
# import numpy as np

# # 전역 변수 세션 및 텐서
# sess = None
# lidar_input_tensor = None
# camera_input_tensor = None
# output_tensor = None

# def initialize_model(model_path):
#     print("fuck python init start")
#     global sess, lidar_input_tensor, camera_input_tensor, output_tensor
#     if sess is None:  # 세션이 이미 초기화되어 있지 않은 경우에만 초기화
#         lidar_input_tensor_name = "main_level/agent/main/online/network_0/SECTOR_LIDAR/SECTOR_LIDAR:0"
#         camera_input_tensor_name = "main_level/agent/main/online/network_0/STEREO_CAMERAS/STEREO_CAMERAS:0"
#         output_tensor_name = "main_level/agent/main/online/network_1/ppo_head_0/policy:0"

#         with tf.io.gfile.GFile(model_path, "rb") as f:
#             graph_def = tf.compat.v1.GraphDef()
#             graph_def.ParseFromString(f.read())

#         graph = tf.Graph()
#         with graph.as_default():
#             tf.import_graph_def(graph_def, name="")

#         sess = tf.compat.v1.Session(graph=graph)
#         lidar_input_tensor = graph.get_tensor_by_name(lidar_input_tensor_name)
#         camera_input_tensor = graph.get_tensor_by_name(camera_input_tensor_name)
#         output_tensor = graph.get_tensor_by_name(output_tensor_name)
#         print("-"*50)
#         print("init end")

# def predict(lidar_data, camera_data):
#     global sess, lidar_input_tensor, camera_input_tensor, output_tensor
#     lidar_input_data = np.array(lidar_data, dtype=np.float32).reshape(1, -1)
#     camera_input_data = np.array(camera_data, dtype=np.float32).reshape(1, 120, 160, 2)

#     feed_dict = {
#         lidar_input_tensor: lidar_input_data,
#         camera_input_tensor: camera_input_data
#     }

#     result = sess.run(output_tensor, feed_dict=feed_dict)
#     return result.flatten().tolist()

import tensorflow as tf
import numpy as np

# 전역 변수 세션 및 텐서
sess = None
lidar_input_tensor = None
camera_input_tensor = None
output_tensor = None

def initialize_model(model_path):
    print("fuck python init start")
    global sess, lidar_input_tensor, camera_input_tensor, output_tensor
    if sess is None:  # 세션이 이미 초기화되어 있지 않은 경우에만 초기화
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
        print("init end")

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
