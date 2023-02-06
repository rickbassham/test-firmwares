all: data/index.html.gz data/mui.css.gz data/mui.js.gz data/manifest.json.gz data_src/mui.css data/md5.js.gz data/uploadDone.html.gz

setup:
	npm i minify -g
	npm i bower -g
	npm i sass -g

data:
	@mkdir -p data

build:
	@mkdir -p build

# https://www.muicss.com/
# https://medium.com/swlh/are-you-using-svg-favicons-yet-a-guide-for-modern-browsers-836a6aace3df

data_src/mui.css:
	sass data_src/custom.sass > data_src/mui.css

data/index.html.gz: data
	minify ./data_src/index.html | gzip -c > data/index.html.gz

data/uploadDone.html.gz: data
	minify ./data_src/uploadDone.html | gzip -c > data/uploadDone.html.gz

data/mui.css.gz: data
	sass data_src/custom.sass | minify --css | gzip -c > data/mui.css.gz

data/mui.js.gz: data
	minify ./data_src/mui.js | gzip -c > data/mui.js.gz

data/md5.js.gz: data
	minify ./data_src/md5.js | gzip -c > data/md5.js.gz

data/manifest.json.gz: data
	cat ./data_src/manifest.json | gzip -c > data/manifest.json.gz

fsimage: build
	platformio run --target buildfs --environment pico_alt
	cd data && zip ../build/filesystem.zip *

firmware: build
	platformio run --environment pico_alt
	cp .pio/build/pico_alt/firmware.bin ./build/firmware.bin

upload-fsimage: fsimage
	platformio run --target uploadfs --environment pico_alt

clean:
	@-rm data_src/mui.css
	@-rm data/index.html.gz
	@-rm data/uploadDone.html.gz
	@-rm data/mui.css.gz
	@-rm data/mui.js.gz
	@-rm data/md5.js.gz
	@-rm data/manifest.json.gz
	@-rm build/filesystem.zip
	@platformio run --target clean --environment pico_alt

clean-all: clean
	@rm -rf data
	@rm -rf build
