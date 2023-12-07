#!/bin/bash

function log() {
	echo "$(date +%X) $1"
}

## Gegelati
if [ ! -d "gegelatilib-1.2.0" ]
then
	log "Download GEGELATI"
	curl -L -o gegelatilib-1.2.0.zip https://github.com/gegelati/gegelati/releases/download/v1.2.0/gegelatilib-1.2.0.zip
	unzip gegelatilib-1.2.0.zip
	rm gegelatilib-1.2.0.zip
fi
