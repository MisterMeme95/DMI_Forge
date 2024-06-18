#!/bin/bash

# Define the color types and their corresponding folders
declare -A color_types
color_types=( ["indexed"]="indexed_images" ["rgb"]="rgb_images" ["grayscale"]="grayscale_images" ["gray_alpha"]="gray_alpha_images" ["rgba"]="rgba_images" )

# Define the bit depths to be tested
bit_depths=(1 2 4 8 16)

# Define the base directory for input and output
base_dir="."
output_dir_prefix="transformed_"
echo "UPDATED!"
# Create a log file for errors
log_file="transformation_errors.log"
echo "Transformation Errors Log" > $log_file

# Loop through each color type
for color_type in "${!color_types[@]}"; do

  input_folder="${base_dir}/${color_types[$color_type]}"
  output_folder="${base_dir}/${output_dir_prefix}${color_type}"

  echo "${input_folder}"
  echo "${output_folder}"
  # Create the output folder if it doesn't exist
  mkdir -p "$output_folder"

  # Loop through each bit depth
  for bit_depth in "${bit_depths[@]}"; do
    echo "Processing ${color_type} images with bit depth ${bit_depth}..."

    # Loop through each image in the input folder
    for input_file in "$input_folder"/*; do
      echo "${input_file}"
      if [[ -f "$input_file" ]]; then
        output_file="${output_folder}/$(basename "$input_file")"

        # Run the png_transform command
        png_transform -i "$input_file" -o "$output_file" -b "$bit_depth" -c "$color_type"

        # Check if the command succeeded
        if [[ $? -ne 0 ]]; then
          echo "Failed to transform $input_file with bit depth $bit_depth and color type $color_type" >> $log_file
        fi
      fi
    done
  done
done

echo "Transformation process completed. Check $log_file for any errors."
