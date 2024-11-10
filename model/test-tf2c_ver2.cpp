//#include "deepracer/camera/camera_driver.h"
//#include "deepracer/lidar/lidar_driver.h"
#include <tensorflow/c/c_api.h>
#include <iostream>
#include <vector>

// TensorFlow model function
TF_Buffer* ReadBufferFromFile(const char* file);
void DeallocateBuffer(void* data, size_t length);
std::vector<float> RunModelInference(TF_Graph* graph, TF_Session* session, const std::vector<float>& lidar_data, const std::vector<float>& camera_data);

// TensorFlow model function
void DeallocateBuffer(void* data, size_t length) {
    free(data);
}

TF_Buffer* ReadBufferFromFile(const char* file) {
    FILE* f = fopen(file, "rb");
    if (f == nullptr) {
        std::cerr << "Failed to open file: " << file << std::endl;
        return nullptr;
    }
    fseek(f, 0, SEEK_END);
    long fsize = ftell(f);
    fseek(f, 0, SEEK_SET);

    void* data = malloc(fsize);
    fread(data, 1, fsize, f);
    fclose(f);

    TF_Buffer* buffer = TF_NewBuffer();
    buffer->data = data;
    buffer->length = fsize;
    buffer->data_deallocator = DeallocateBuffer;

    return buffer;
}

std::vector<float> RunModelInference(TF_Graph* graph, TF_Session* session, const std::vector<float>& lidar_data, const std::vector<float>& camera_data) {
    TF_Status* status = TF_NewStatus();

    // Create camera and lidar data (input tensor...)
    int lidar_data_size = lidar_data.size();
    int camera_data_size = camera_data.size();

    TF_Tensor* input_lidar_tensor = TF_NewTensor(TF_FLOAT, nullptr, 0, lidar_data.data(), lidar_data_size * sizeof(float), [](void* data, size_t len, void* arg){}, nullptr);
    TF_Tensor* input_camera_tensor = TF_NewTensor(TF_FLOAT, nullptr, 0, camera_data.data(), camera_data_size * sizeof(float), [](void* data, size_t len, void* arg){}, nullptr);

    // input name set(from tensor i/o)
    const char* input_names[] = {"main_level/agent/main/online/network_0/SECTOR_LIDAR/SECTOR_LIDAR", "main_level/agent/main/online/network_0/STEREO_CAMERAS/STEREO_CAMERAS"};
    TF_Output input_ops[] = {{TF_GraphOperationByName(graph, input_names[0]), 0}, {TF_GraphOperationByName(graph, input_names[1]), 0}};

    const char* output_name = "main_level/agent/main/online/network_1/ppo_head_0/policy";
    TF_Output output_op = {TF_GraphOperationByName(graph, output_name), 0};

    TF_Tensor* output_tensors[1];
    TF_SessionRun(session, nullptr, input_ops, input_tensors, 2, &output_op, output_tensors, 1, nullptr, 0, nullptr, status);

    if (TF_GetCode(status) != TF_OK) {
        std::cerr << "Error during model inference: " << TF_Message(status) << std::endl;
    }

    // output data -> vector
    float* data = static_cast<float*>(TF_TensorData(output_tensors[0]));
    int output_size = TF_TensorByteSize(output_tensors[0]) / sizeof(float);
    std::vector<float> output_data(data, data + output_size);

    TF_DeleteStatus(status);
    TF_DeleteTensor(input_lidar_tensor);
    TF_DeleteTensor(input_camera_tensor);
    TF_DeleteTensor(output_tensors[0]);

    return output_data;
}

int main() {
    // TensorFlow model load!!!!!!!
    TF_Status* status = TF_NewStatus();
    TF_Graph* graph = TF_NewGraph();
    TF_Buffer* graph_def = ReadBufferFromFile("model.pb");

    if (!graph_def) {
        std::cerr << "Failed to load model." << std::endl;
        return -1;
    }

    TF_ImportGraphDefOptions* import_opts = TF_NewImportGraphDefOptions();
    TF_GraphImportGraphDef(graph, graph_def, import_opts, status);
    if (TF_GetCode(status) != TF_OK) {
        std::cerr << "Error importing graph: " << TF_Message(status) << std::endl;
        return -1;
    }

    TF_SessionOptions* session_opts = TF_NewSessionOptions();
    TF_Session* session = TF_NewSession(graph, session_opts, status);
    if (TF_GetCode(status) != TF_OK) {
        std::cerr << "Error creating session: " << TF_Message(status) << std::endl;
        return -1;
    }

    // camera, lidar init @TODO : Check with KSH driver....
    CameraDriver cameraDriver(0, 1); //example
    LidarDriver lidarDriver("/dev/ttyUSB0", 115200); //example

    if (!cameraDriver.CreateCamera() || !lidarDriver.CreateLidar()) {
        std::cerr << "Failed to initialize camera or lidar." << std::endl;
        return -1;
    }

    while (true) {
        // Get camera and lidar datda
        std::vector<u_char> left_camera_data, right_camera_data;
        cameraDriver.GetCameraData(&left_camera_data, &right_camera_data);

        std::vector<float> lidar_data;
        lidarDriver.GetLidarData(&lidar_data);

        // Inference
        std::vector<float> output = RunModelInference(graph, session, lidar_data, left_camera_data);

        // Speed and direction
        float speed = output[0];
        float direction = output[1];

        std::cout << "Predicted Speed: " << speed << ", Direction: " << direction << std::endl;

        // TODO : SEND PART
        // SendData(speed, direction); //TODO : DEVELOP FUNCTION....
    }

    // Close session and de-mem space delete
    TF_CloseSession(session, status);
    TF_DeleteSession(session, status);
    TF_DeleteGraph(graph);
    TF_DeleteStatus(status);
    TF_DeleteBuffer(graph_def);

    return 0;
}
