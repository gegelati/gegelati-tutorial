#!/bin/bash

function log() {
	echo "$(date +%X) $1"
}

## dowload font
if [ ! -f "DejaVuSans.ttf" ] 
then
	curl -o DejaVuSans.ttf https://preesm.github.io/assets/downloads/DejaVuSans.ttf
fi

# download pendulum
if [ ! -f "pendulum.png" ] 
then
	curl -o pendulum.png https://preesm.github.io/assets/downloads/pendulum.png
fi

# download arrow
if [ ! -f "arrow.png" ] 
then
	curl -o arrow.png https://preesm.github.io/assets/downloads/arrow.png
fi

