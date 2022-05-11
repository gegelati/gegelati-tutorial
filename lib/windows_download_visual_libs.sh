#!/bin/bash

function log() {
	echo "$(date +%X) $1"
}

## Gegelati
if [ ! -d "gegelatilib-1.1.0" ] 
then
	log "Download GEGELATI"
	curl -L -o gegelatilib-1.1.0.zip https://github.com/gegelati/gegelati/releases/download/v1.1.0/gegelatilib-1.1.0.zip
	unzip gegelatilib-1.1.0.zip
	rm gegelatilib-1.1.0.zip
fi

## SDL2
if [ ! -d "SDL2-2.0.9" ] 
then
	log "Download SDL2 for visual"
	curl -o SDL2-devel-2.0.9-VC.zip https://www.libsdl.org/release/SDL2-devel-2.0.9-VC.zip
	unzip SDL2-devel-2.0.9-VC.zip
	cp SDL2-2.0.9/lib/x64/* SDL2-2.0.9/lib/
	rm SDL2-devel-2.0.9-VC.zip 
fi

## SDL2-ttf
if [ ! -d "SDL2_ttf-2.0.15" ] 
then
	log "Download SDL2_ttf for visual"
	curl -o SDL2_ttf-devel-2.0.15-VC.zip https://www.libsdl.org/projects/SDL_ttf/release/SDL2_ttf-devel-2.0.15-VC.zip
	unzip SDL2_ttf-devel-2.0.15-VC.zip
	cp SDL2_ttf-2.0.15/lib/x64/* SDL2_ttf-2.0.15/lib/
	rm SDL2_ttf-devel-2.0.15-VC.zip
fi

## SDL2-image
if [ ! -d "SDL2_image-2.0.5" ] 
then
	log "Download SDL_image for visual"
	curl -o SDL2_image-devel-2.0.5-VC.zip https://www.libsdl.org/projects/SDL_image/release/SDL2_image-devel-2.0.5-VC.zip
	unzip SDL2_image-devel-2.0.5-VC.zip
	cp SDL2_image-2.0.5/lib/x64/* SDL2_image-2.0.5/lib/
	rm SDL2_image-devel-2.0.5-VC.zip
fi