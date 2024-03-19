#!/bin/bash
# TODO - get a tag as a parameter and push it as well

message=""

while getopts m: flag
do
    case "${flag}" in
        m) message=${OPTARG};;
    esac
done

echo $message

git add *.c *.h *.sh makefile
if [$message = ""]
then
    git commit
else
    git commit -m "$message"
fi
git push origin
git status
