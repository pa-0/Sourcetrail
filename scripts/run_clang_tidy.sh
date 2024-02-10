#!/bin/env bash

JSON_DIR=../build/Release/CLang/compile_commands.json
SOURCE_DIR=/home/ahussein/Documents/Sourcetrail/src

unset IFS
files_list=()
while IFS=  read -r -d $'\0'; do
    files_list+=("$REPLY")
done < <(find $SOURCE_DIR/src -not -path "*test*" -type f -name "*.cpp" -print0)

files_count="${#files_list[@]}"
echo "$files_count"

for ((i=0;i<$files_count;++i)); do
  file_name="${files_list[$i]}"
  echo "[$i/$files_count] $file_name"
  clang-tidy -p $JSON_DIR --checks="-*,clang-analyzer-core*" $file_name
done
