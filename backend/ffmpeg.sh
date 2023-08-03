#!/bin/bash

# 输入视频文件路径
input_file="/home/engage/github_projects/socket/backend/file/mp4-30min/armv8.mp4"
# 获取视频文件的目录路径
dir_path=$(dirname "$input_file")
# 输出的m3u8文件路径
output_playlist="$dir_path/output.m3u8"
# 输出的ts文件路径前缀
output_segment="$dir_path/segment%03d.ts"
# 每个切片的时间
segment_time=10

# 如果目标文件夹中存在.ts或.m3u8文件，则删除
if [ -f "$dir_path/*.ts" ] || [ -f "$output_playlist" ]; then
  rm "$dir_path/*.ts"
  rm "$output_playlist"
fi

# 获取视频文件大小
original_size=$(du -sh "$input_file" | cut -f1)

# 获取切分开始的时间
start_time=$(date +%s)

# 使用ffmpeg切分视频
ffmpeg -i "$input_file" -codec copy -map 0 -f segment -segment_list "$output_playlist" -segment_time ${segment_time} "${output_segment/\%03d/%03d}"

# 获取切分结束的时间
end_time=$(date +%s)

# 计算切分耗费的时间
duration=$((end_time - start_time))

# 获取切分后的ts文件数量
num_segments=$(find "$dir_path" -name 'segment*.ts' -type f | wc -l)

# 获取切分后的ts文件总大小
total_size=$(du -csh "$dir_path"/segment*.ts | grep total | cut -f1)
# 如果没有生成任何ts文件，平均大小设为0，否则计算平均大小
if [ "$num_segments" -eq 0 ]; then
  average_size="0"
else
  average_size=$(du -sh "$dir_path"/segment*.ts | awk '{total += $1} END {print total/NR "M"}')
fi

# 打印信息
echo "[every slide lasts] $segment_time s"
echo "[segment lasts] $duration s"
echo "[video size] $original_size"
echo "[.ts nums] $num_segments"
echo "[.ts total size] $total_size"
echo "[.ts average size] $average_size"
