import tensorflow as tf

with tf.io.gfile.GFile('model.pb', 'rb') as f:
    graph_def = tf.compat.v1.GraphDef()
    graph_def.ParseFromString(f.read())

with tf.Graph().as_default() as graph:
    tf.import_graph_def(graph_def, name='')
    for op in graph.get_operations():
        print(op.name)

