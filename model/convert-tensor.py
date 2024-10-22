import tensorflow as tf

#### Enable eager execution for TensorFlow 1.x compatibility ####
tf.compat.v1.enable_eager_execution()

##### Load the graph definition from the model.pb file ####
with tf.io.gfile.GFile('model.pb', 'rb') as f:
    graph_def = tf.compat.v1.GraphDef()
    graph_def.ParseFromString(f.read())

# Define input and output tensor names
input_tensor_names = [
    'main_level/agent/main/online/network_0/SECTOR_LIDAR/SECTOR_LIDAR:0',
    'main_level/agent/main/online/network_0/STEREO_CAMERAS/STEREO_CAMERAS:0'
]
output_tensor_name = 'main_level/agent/main/online/network_1/ppo_head_0/policy:0'

#### Wrap the frozen graph to create a ConcreteFunction ####
def wrap_frozen_graph(graph_def, inputs, outputs):
    def _imports_graph_def():
        tf.compat.v1.import_graph_def(graph_def, name='')
    # Wrap the import in a tf.compat.v1.wrap_function
    wrapped_import = tf.compat.v1.wrap_function(_imports_graph_def, [])
    import_graph = wrapped_import.graph
    # Map input and output tensors
    return wrapped_import.prune(
        tf.nest.map_structure(import_graph.as_graph_element, inputs),
        tf.nest.map_structure(import_graph.as_graph_element, outputs))

# Create the ConcreteFunction
concrete_func = wrap_frozen_graph(
    graph_def=graph_def,
    inputs=input_tensor_names,
    outputs=output_tensor_name)

print("Input tensor details:")
for input_tensor in concrete_func.inputs:
    print("Name:", input_tensor.name)
    print("Shape:", input_tensor.shape)
    print("Dtype:", input_tensor.dtype)
    print("---")

print("Output tensor details:")
for output_tensor in concrete_func.outputs:
    print("Name:", output_tensor.name)
    print("Shape:", output_tensor.shape)
    print("Dtype:", output_tensor.dtype)
    print("---")

# Create TFLite converter from the ConcreteFunction
converter = tf.lite.TFLiteConverter.from_concrete_functions([concrete_func])

# Set optimization options -> TO BE DONE! (Not study....)
# converter.optimizations = [tf.lite.Optimize.DEFAULT]

tflite_model = converter.convert()

#### SAVE ####
with open('model.tflite', 'wb') as f:
    f.write(tflite_model)

print("!!!!!!!!! TensorFlow Lite convert DONE !!!!!!!!!")
