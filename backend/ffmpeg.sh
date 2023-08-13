#!/bin/bash

# Check for correct number of arguments
if [ "$#" -ne 2 ]; then
    echo "Usage: $0 <movie_folder_name> <segment_time>"
    exit 2
fi

# Initialize variables
movie_folder_name=$1
segment_time=$2

dir_path="/home/engage/github_projects/socket/backend/static/$movie_folder_name"
mp4_file=$(find "$dir_path" -type f -name "*.mp4" | head -n 1)

# Check for the existence of the mp4 file
if [ -z "$mp4_file" ]; then
    echo "No mp4 file found in the directory static/$movie_folder_name"
    exit 1
fi

# Define the output paths
out_m3u8="$dir_path/output.m3u8"
out_segment="$dir_path/segment%03d.ts"

# Remove existing segments and playlist, if they exist
rm "$dir_path"/*.ts 2> /dev/null
rm "$out_m3u8" 2> /dev/null

# Capture the start time of segmentation
start_time=$(date +%s)

# FFmpeg command setup
src=(-i "$mp4_file")
codec=(-codec copy)
codev=(-c:v libx264 -force_key_frames "expr:gte(t,n_forced*$segment_time)")
codea=(-c:a aac)
map=(-map 0)
form=(-f segment)
path=(-segment_list "$out_m3u8")
max_time_0=(-segment_time 1)
max_time=(-segment_time "$segment_time")

ffmpeg_opt0=("${src[@]}" "${codev[@]}" "${codea[@]}" "${map[@]}" "${form[@]}" "${path[@]}" "${max_time_0[@]}")
ffmpeg_opt=("${src[@]}" "${codev[@]}" "${codea[@]}" "${map[@]}" "${form[@]}" "${path[@]}" "${max_time[@]}")

# Segment the first 10 seconds of the video into 1-second chunks
for i in $(seq 0 9); do
  ffmpeg "${ffmpeg_opt0[@]}" -ss $i -t 1 "$out_segment"
done

# Segment the rest of the video
ffmpeg -ss 10 "${ffmpeg_opt[@]}" "${out_segment}"

# Capture the end time of segmentation
end_time=$(date +%s)

# Compute segmentation duration and file details
duration=$((end_time - start_time))
num_segments=$(find "$dir_path" -name 'segment*.ts' -type f | wc -l)
total_size=$(du -csh "$dir_path"/segment*.ts | grep total | cut -f1)
average_size=$(du -sh "$dir_path"/segment*.ts | awk '{total += $1} END {print total/NR "K"}')

# Display information
echo "[Duration of each segment] $segment_time s"
echo "[Time taken to segment] $duration s"
echo "[Number of .ts files] $num_segments"
echo "[Total size of .ts files] $total_size"
echo "[Average size of .ts files] $average_size"