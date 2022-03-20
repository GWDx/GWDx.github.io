#!/bin/bash
set -e

[[ $(git status -s) ]] && exit 1

echo 'prepare'

git branch -D build
git branch build
git checkout build

hugo
cp CNAME docs/CNAME

git add .
git commit -m "build"
