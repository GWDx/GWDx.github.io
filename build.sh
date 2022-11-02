#!/bin/bash
set -e

if [[ $(git status -s) ]]
then
    echo 'dirty'
    exit 1
fi

git checkout build
git reset origin/build --hard
git merge master --no-edit

rm -r docs
hugo
cp CNAME docs/CNAME

git add .
git commit --amend -m "build"
