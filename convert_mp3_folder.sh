#!/bin/bash

# Ins Verzeichnis kopieren, dass auch den mp3-Ordner enthält

FOLDER_INDEX_START=96
TARGET_DIR="${PWD}/CONVERTED_MP3_FOLDER"

if [ ! -d "$TARGET_DIR" ]; then
    mkdir "$TARGET_DIR"
    mkdir "$TARGET_DIR"/$(printf "%02d" $(($FOLDER_INDEX_START + 0)))
    mkdir "$TARGET_DIR"/$(printf "%02d" $(($FOLDER_INDEX_START + 1)))
    mkdir "$TARGET_DIR"/$(printf "%02d" $(($FOLDER_INDEX_START + 2)))
    mkdir "$TARGET_DIR"/$(printf "%02d" $(($FOLDER_INDEX_START + 3)))
fi
echo "Mapping:" > mapping.txt
for FILE in mp3/*;do
   if [ -f "$FILE" ]; then
    INDEX=$(echo ${FILE} | sed -n "s/^.*\([0-9]\{4\}\)\(.*\)$/\1/p")
    
    FOLDER=$(printf "%02d" $(($FOLDER_INDEX_START + 10#$INDEX / 254)))
    FILENAME=$(printf "%03d" $((10#$INDEX % 254 + 1))).mp3
    
    echo "Kopiere $FILE nach $FOLDER/$FILENAME" >> mapping.txt
    echo "Kopiere $FILE nach $FOLDER/$FILENAME"
    cp "$FILE" "${TARGET_DIR}/${FOLDER}/${FILENAME}"
    
   fi
done

