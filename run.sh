#!/bin/bash
set -e

if [[ $(git status -s) ]]
then
    echo 'dirty'
    exit 1;
fi

git checkout build
git reset --hard origin/build
git merge master --no-edit

hugo
cp CNAME docs/CNAME

git add .
git commit --amend -m "build"
