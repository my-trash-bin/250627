#!/bin/sh

set -e
cd "$(dirname "$0")"

git submodule update --init --recursive

if command -v cmd > /dev/null; then
  PWD="$(cmd //c cd | sed 's|\\|\\\\\\\\|g' | sed s/\"/\\\"/g)"
else
  PWD="$(pwd | sed 's|\\|\\\\\\\\|g' | sed s/\"/\\\"/g)"
fi

echo '[
  {
    "directory": "[[WORKSPACE]]",
    "file": "src/lib.cpp",
    "output": "/dev/null",
    "arguments": [
      "clang++",
      "src/lib.cpp",
      "-o",
      "/dev/null",
      "-I",
      "submodules/libcoro/include",
      "-std=c++20",
      "-c"
    ]
  }
]
' | sed "s\\[\\[WORKSPACE\\]\\]$PWDg" > compile_commands.json
