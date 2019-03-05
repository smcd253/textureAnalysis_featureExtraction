#!/bin/sh
#sh gitHelper "[insert message here]"
git pull
git add .
git commit -am $0
git push -u origin master