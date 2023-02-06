FILES := $(wildcard build/*/*.gz) $(wildcard build/*/*/main*.gz) $(wildcard build/*.gz)

pre-package:
	gzip -f -r -k build

package:
	zip ./dist/filesystem.zip $(FILES) -x \*.map.gz -x \*LICENSE*.gz -x \*status.json.gz
	md5sum ./dist/filesystem.zip | cut -d " " -f1 > ./dist/filesystem.zip.md5

purgecss:
	purgecss --css build/static/css/*.css --content build/index.html build/static/js/*.js --output build/static/css

build:
	npm run build

clean:
	rm -Rf ./dist/*
	rm -Rf ./build
