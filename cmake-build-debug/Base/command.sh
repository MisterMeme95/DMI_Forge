#!/bin/bash

# Define the input folder for indexed images
input_folder="indexed_images"

# Define the output folder for transformed images
output_folder="transformed_indexed"

# Define the bit depth and color type
target_bit_depth=8
target_color_type="rgba"

# Create a log file for errors
log_file="transformation_errors.log"
echo "Transformation Errors Log" > $log_file

# Create the output folder if it doesn't exist
mkdir -p "$output_folder"

echo "Processing images in ${input_folder} with bit depth ${target_bit_depth} and color type ${target_color_type}..."

# Loop through each image in the input folder
for input_file in "$input_folder"/*; do
  if [[ -f "$input_file" ]]; then
    # Change the file extension to .png for the output file
    output_file="${output_folder}/$(basename "$input_file" .dmi).png"

    # Run the png_transform command
    png_transform -i "$input_file" -o "$output_file" -b "$target_bit_depth" -c "$target_color_type"

    # Check if the command succeeded
    if [[ $? -ne 0 ]]; then
      echo "Failed to transform $input_file with bit depth $target_bit_depth and color type $target_color_type" >> $log_file
    fi
  fi
done

echo "Transformation process completed. Check $log_file for any errors."
