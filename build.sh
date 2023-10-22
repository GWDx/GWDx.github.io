#!/bin/bash
set -e

# assert git status is clean
if [[ -n $(git status --porcelain) ]]; then
    echo "git status is not clean"
    exit 1
fi

# assert that we are on master
if [[ $(git rev-parse --abbrev-ref HEAD) != 'master' ]]
then
    echo 'not on master'
    exit 1
fi

git checkout build
git reset origin/build --hard
git merge master --no-edit

rm -r docs
hugo
# cp CNAME docs/CNAME

git add .
git commit --amend -m "build"
