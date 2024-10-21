import tensorflow as tf
import argparse

parser = argparse.ArgumentParser(description="Convert a TensorFlow SavedModel to TensorFlow Lite format.")
parser.add_argument('saved_model_dir', type=str, help='Path to the SavedModel directory')
parser.add_argument('output_file', type=str, help='Filename to save the converted TensorFlow Lite model')

args = parser.parse_args()

# path to the SavedModel directory
converter = tf.lite.TFLiteConverter.from_saved_model(args.saved_model_dir) 
tflite_model = converter.convert()

with open(args.output_file, 'wb') as f:
    f.write(tflite_model)

print(f'TensorFlow Lite model saved as {args.output_file}')
