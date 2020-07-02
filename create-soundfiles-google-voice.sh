#!/bin/bash

#Dieses Skript erstellt die Sounddateien mit Gools Text-to-speech Service.
#Dazu liest es das create-soundfiles.sh Skript aus.

#usage: create-soundfiles-google.voice.sh script
#Parameter:
#script: Pfad zum create-soundfiles.sh Skript.

#JSON-Datei mit den Zugangsdaten zu den Google Cloud Diensten, siehe https://cloud.google.com/text-to-speech/docs/quickstart-protocol?hl=de
export GOOGLE_APPLICATION_CREDENTIALS=""
if [ -z $GOOGLE_APPLICATION_CREDENTIALS ] ; then
    echo "Zuerst die Umgebungsvariable GOOGLE_APPLICATION_CREDENTIALS auf die JSON-Datei mit den Zugangsdaten zu den Google Cloud Diensten setzen!"
    echo "Siehe https://cloud.google.com/text-to-speech/docs/quickstart-protocol?hl=de"
    exit
fi


REQUEST_JSON_1="{\"input\":{\"text\":\""
REQUEST_JSON_2="\"},\"voice\":{\"languageCode\":\"de-DE\",\"name\":\"de-DE-Wavenet-C\",\"ssmlGender\":\"FEMALE\"},\"audioConfig\":{\"audioEncoding\":\"MP3\"}}"
TMP_FILE="create-soundfiles-gen.sh"
rm $TMP_FILE
IFS=" "

echo 'function sendRequest(){
curl -X POST \
-H "Authorization: Bearer "$(gcloud auth application-default print-access-token) \
-H "Content-Type: application/json; charset=utf-8" \
-d @request.json \
https://texttospeech.googleapis.com/v1/text:synthesize > synthesize-output-base64.json
}' >> $TMP_FILE

# Extrahiert aus einem String den x. Parameter unter berücksichtung von double quotes (").
# Param1: String
# Param2: Nummer des Parameters beginnend bei 1
function extractParam(){
POS=1
TEXT=
DQUOTES="false"
for (( i = 0; i < ${#1}; ++i)); do
    local result
    case "${1:$i:1}" in
        [[:space:]])
            if [ "$DQUOTES" = "false" ] ; then
                if [ $POS -eq $2 ] ; then
                    result=$TEXT
                    break
                fi
                POS=$[$POS+1]
                TEXT=
            else
                TEXT=${TEXT}${1:$i:1}
            fi

            ;;
        \")
            if [ "$DQUOTES" = "false" ] ; then
                DQUOTES="true"
            else
                DQUOTES="false"
            fi
            ;;
        [^[:space:]])
            TEXT=${TEXT}${1:$i:1}
            
            if [ $i -eq $((${#1} - 1)) ] ; then
                result=$TEXT

            fi
            ;;
    esac
done

echo "$result"
}

processing_track="FALSE"

while read line
do
    if [ "$processing_track" = "FALSE" ]; then
        if echo "$line" | grep -q "say"; then
            processing_track="TRUE"
            TEXT=$(extractParam "$line" 4)
            echo 'REQUEST_JSON='"'"${REQUEST_JSON_1}"'""\"$TEXT\"""'"${REQUEST_JSON_2}"'" >> $TMP_FILE
            echo 'echo $REQUEST_JSON | jq '"'"."'"' > request.json' >> $TMP_FILE
            echo 'sendRequest' >> $TMP_FILE
            echo 'cat synthesize-output-base64.json | jq -r '"'"'.audioContent'"'"'  > synthesize-output-base64.txt' >> $TMP_FILE
        else
            echo "$line" >> $TMP_FILE
        fi
    else
        if echo "$line" | grep -q "sox"; then
            #do nothing
            :
        elif echo "$line" | grep -q "lame"; then
            #do nothing
            set -- $line
            echo 'base64 synthesize-output-base64.txt --decode > '$5 >> $TMP_FILE
            echo 'echo "Processed: "'$5 >> $TMP_FILE
            processing_track="FALSE"
        else
            echo "$line" >> $TMP_FILE
        fi
    fi

done < "$1"

chmod +x $TMP_FILE
./$TMP_FILE
