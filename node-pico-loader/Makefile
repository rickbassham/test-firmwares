all: content/index.html.gz content/mui.css.gz content/mui.js.gz content/manifest.json.gz content/updates.json

setup:
	npm i minify -g

serve:
	docker compose up

content:
	@mkdir -p content

# https://www.muicss.com/
# https://medium.com/swlh/are-you-using-svg-favicons-yet-a-guide-for-modern-browsers-836a6aace3df

content/index.html.gz: content
	minify ./src/index.html | gzip -c > content/index.html.gz

content/mui.css.gz: content
	minify ./src/mui.css | gzip -c > content/mui.css.gz

content/mui.js.gz: content
	minify ./src/mui.js | gzip -c > content/mui.js.gz

content/manifest.json.gz: content
	cat ./src/manifest.json | gzip -c > content/manifest.json.gz

content/updates.json: content
	cat ./src/updates.json > content/updates.json

clean:
	@rm -rf content
