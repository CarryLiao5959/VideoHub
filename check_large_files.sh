git ls-files | while read file; do
  size=$(git cat-file -s :"$file")
  if [ $size -gt 104857600 ]; then
    echo "Large file in Git cache: $file ($size bytes)"
  fi
done