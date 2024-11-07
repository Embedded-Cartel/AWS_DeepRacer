#include <tensorflow/core/public/session.h>
#include <tensorflow/core/protobuf/meta_graph.pb.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <stdexcept>

#define DEBUG_MODE 1  // Set to 0 for normal operation

using namespace tensorflow;

void loadModel(Session*& session, const std::string& model_path) {
    SessionOptions session_options;
    Status status = NewSession(session_options, &session);
    if (!status.ok()) {
        throw std::runtime_error("Failed to create TensorFlow session.");
    }

    GraphDef graph_def;
    status = ReadBinaryProto(Env::Default(), model_path, &graph_def);
    if (!status.ok()) {
        throw std::runtime_error("Failed to load graph.");
    }

    status = session->Create(graph_def);
    if (!status.ok()) {
        throw std::runtime_error("Failed to create graph in session.");
    }
}

std::vector<float> inferModel(Session* session, const std::vector<float>& lidar_data, const std::vector<float>& camera_data) {
    Tensor input_lidar(DT_FLOAT, TensorShape({1, static_cast<int64_t>(lidar_data.size())}));
    Tensor input_camera(DT_FLOAT, TensorShape({1, static_cast<int64_t>(camera_data.size())}));

    std::copy(lidar_data.begin(), lidar_data.end(), input_lidar.flat<float>().data());
    std::copy(camera_data.begin(), camera_data.end(), input_camera.flat<float>().data());

    std::vector<std::pair<std::string, Tensor>> inputs = {
        {"main_level/agent/main/online/network_0/SECTOR_LIDAR/SECTOR_LIDAR", input_lidar},
        {"main_level/agent/main/online/network_0/STEREO_CAMERAS/STEREO_CAMERAS", input_camera}
    };

    std::vector<Tensor> outputs;
    Status status = session->Run(inputs, {"main_level/agent/main/online/network_1/ppo_head_0/policy"}, {}, &outputs);

    if (!status.ok()) {
        throw std::runtime_error("Model inference failed: " + status.ToString());
    }

    auto output_flat = outputs[0].flat<float>();
    return {output_flat.data(), output_flat.data() + output_flat.size()};
}

// Function to read data from example_data.txt
void readExampleData(const std::string& filename, std::vector<float>& lidar_data, std::vector<float>& camera_data) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open " + filename);
    }

    std::string line;
    bool is_lidar_data = true;

    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') {
            continue; //error....
        }

        std::istringstream iss(line);
        std::string value;

        if (is_lidar_data) {
            while (std::getline(iss, value, ',')) {
                lidar_data.push_back(std::stof(value));
            }
            is_lidar_data = false; // Next data is camera data
        } else {
            // Read camera data
            while (std::getline(iss, value, ',')) {
                camera_data.push_back(std::stof(value));
            }
        }
    }
    file.close();
}

// Function to simulate external communication (for normal operation)
void receiveData(std::vector<float>& lidar_data, std::vector<float>& camera_data) {
//@TODO!!!!!!!!!!!!
}

void sendData(float speed, float direction) {
//@TODO!!!!!!!!!!!!
}

int main() {
    // Load the model
    Session* session;
    try {
        loadModel(session, "model.pb");
        std::cout << "Model loaded successfully." << std::endl;
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return -1;
    }

#if DEBUG_MODE
    std::vector<float> lidar_data;
    std::vector<float> camera_data;

    try {
        readExampleData("example_data.txt", lidar_data, camera_data);
        std::cout << "Data read from example_data.txt." << std::endl;

        // Perform inference
        std::vector<float> output = inferModel(session, lidar_data, camera_data);

        // Print the output
        std::cout << "Inference Output:" << std::endl;
        std::cout << "Speed: " << output[0] << ", Direction: " << output[1] << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Error during debug mode: " << e.what() << std::endl;
        session->Close();
        delete session;
        return -1;
    }

#else
    // Normal operation: Receive data from external communication and perform inference
    while (true) {
        // Receive data from external source
        std::vector<float> lidar_data;
        std::vector<float> camera_data;

        receiveData(lidar_data, camera_data);
        
        try {
            std::vector<float> output = inferModel(session, lidar_data, camera_data);

            float speed = output[0];
            float direction = output[1];

            sendData(speed, direction);

        } catch (const std::exception& e) {
            std::cerr << "Error during inference: " << e.what() << std::endl;
            // Handle error
        }
    }
#endif

    // Close the session
    session->Close();
    delete session;

    return 0;
}
