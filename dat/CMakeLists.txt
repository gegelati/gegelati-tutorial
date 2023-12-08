MESSAGE("Download dat content.")
set(ASSET_LIST "DejaVuSans.ttf" "pendulum.png" "arrow.png")
set(ASSET_URL_HEAD "https://preesm.github.io/assets/downloads/")

foreach(ASSET IN LISTS ASSET_LIST)
	file(GLOB ASSET_FILE "${DAT_DIR}/${ASSET}")	
	if(ASSET_FILE MATCHES "$^")
	
		message("Downloading ${ASSET} library from ${ASSET_URL_HEAD}/${ASSET}")
			file(DOWNLOAD ${ASSET_URL_HEAD}/${ASSET} "${DAT_DIR}/${ASSET}")
	endif()
endforeach()