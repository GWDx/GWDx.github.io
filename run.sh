#!/bin/bash
set -e

[[ $(git status -s) ]] && exit 1

echo 'prepare'

git checkout build
git reset --hard master

hugo
cp CNAME docs/CNAME

git add .
git commit -m "build"
