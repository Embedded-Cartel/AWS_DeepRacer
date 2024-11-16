import tensorflow as tf

def print_graph_def_node_names(pb_model_path):
    # Load the frozen graph (.pb)
    with tf.io.gfile.GFile(pb_model_path, "rb") as f:
        graph_def = tf.compat.v1.GraphDef()
        graph_def.ParseFromString(f.read())
    
    # List all the nodes in the graph
    for node in graph_def.node:
        print(f"Node name: {node.name} | Operation: {node.op}")

# Call the function to print all node names
print_graph_def_node_names('model.pb')

