#!/bin/bash

make

items=(
  "simple"
  "square"
  "polygon"
  "circle"
  "ellipse"
)

for item in "${items[@]}"
do
  echo "Running $item"
  ./bin/$item
done
