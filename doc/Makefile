all: css userguide changelog pdf utlist utstring utarray

userguide:	txt/userguide.txt
	asciidoc --unsafe --out-file=html/userguide.html -a linkcss=1 -a theme=tdh $<

utlist: txt/utlist.txt
	asciidoc --unsafe --out-file=html/utlist.html -a linkcss=1 -a theme=tdh $<

utarray: txt/utarray.txt
	asciidoc --unsafe --out-file=html/utarray.html -a linkcss=1 -a theme=tdh $<

utstring: txt/utstring.txt
	asciidoc --unsafe --out-file=html/utstring.html -a linkcss=1 -a theme=tdh $<

changelog:	txt/ChangeLog.txt
	asciidoc --out-file=html/ChangeLog.html txt/ChangeLog.txt 

stage:
	cp html/userguide.html ${PAGEROOT}/uthash
	cp html/userguide.pdf ${PAGEROOT}/uthash
	cp html/utlist.html ${PAGEROOT}/uthash
	cp html/utarray.html ${PAGEROOT}/uthash
	cp html/utstring.html ${PAGEROOT}/uthash
	cp html/ChangeLog.html ${PAGEROOT}/uthash
	cp html/license.html ${PAGEROOT}/uthash
	cp html/index.html ${PAGEROOT}/uthash
	cp html/styles.css ${PAGEROOT}/uthash

.PHONY: pdf

pdf: txt/userguide.txt
	a2x --format=pdf $<
	mv txt/userguide.pdf pdf/
	cd html && ln -sf ../pdf/userguide.pdf userguide.pdf
	rm -f txt/userguide.xml

css: html/toc.css
	if [ ! -d ~/.asciidoc/themes/tdh ]; then mkdir -p ~/.asciidoc/themes/tdh; fi
	cp html/toc.css ~/.asciidoc/themes/tdh

docbook: txt/userguide.txt
	asciidoc -b docbook --out-file=/tmp/userguide.xml $<
	xmlto -o html html-nochunks /tmp/userguide.xml 
