#!/bin/sh

git pull
git add .
git commit -am $0
git push -u origin master