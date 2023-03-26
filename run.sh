#!/bin/bash

make

items=(
  "simple"
  "square"
  "polygon"
  "circle"
  "ellipse"
  "line"
)

for item in "${items[@]}"
do
  echo "Running $item"
  ./bin/$item
  cp $item.png ./dist/$item.png
  echo "$item" >> ./dist/items.txt
done
