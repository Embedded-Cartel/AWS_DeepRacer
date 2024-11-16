#include <iostream>
#include <tensorflow/c/c_api.h>

void DeallocateBuffer(void* data, size_t length) {
    free(data);
// free memory allocated for buffer
}

/*** Function to read the protobuf file into a TF_Buffer ***/
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

/******************************************* MAIN START *******************************************/
int main() {    
    std::cout << "TensorFlow Version: " << TF_Version() << std::endl; // Print TensorFlow version
    
    TF_Status* status = TF_NewStatus(); // Initialize status
    
    TF_Graph* graph = TF_NewGraph(); // Create a new graph

    /*** Import the graph definition from the .pb file ***/
    const char* model_path = "model.pb"; // CHANGE THIS!!!! ["YOUR MODEL NAME.pb"]
    TF_Buffer* graph_def = ReadBufferFromFile(model_path);
    if (graph_def == nullptr) {
        std::cerr << "Failed to load graph_def" << std::endl;
        return -1;
    }

    /*** Import graph_def into the graph ***/
    TF_ImportGraphDefOptions* import_opts = TF_NewImportGraphDefOptions();
    TF_GraphImportGraphDef(graph, graph_def, import_opts, status);
    if (TF_GetCode(status) != TF_OK) {
        std::cerr << "Error importing graph_def: " << TF_Message(status) << std::endl;
        return -1;
    }
    TF_DeleteImportGraphDefOptions(import_opts);
    TF_DeleteBuffer(graph_def);

    std::cout << "Model loaded successfully!" << std::endl;

    /*** Create a session for the graph ***/
    TF_SessionOptions* session_opts = TF_NewSessionOptions();
    TF_Session* session = TF_NewSession(graph, session_opts, status);
    if (TF_GetCode(status) != TF_OK) {
        std::cerr << "Error creating session: " << TF_Message(status) << std::endl;
        return -1;
    }
    TF_DeleteSessionOptions(session_opts);

    // Clean up
    TF_CloseSession(session, status);
    TF_DeleteSession(session, status);
    TF_DeleteGraph(graph);
    TF_DeleteStatus(status);

    return 0;
}
