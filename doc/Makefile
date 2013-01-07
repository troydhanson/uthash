all: css userguide changelog pdf utlist utstring utarray

userguide:	userguide.txt
	asciidoc --unsafe --out-file=userguide.html $<

utlist: utlist.txt
	asciidoc --unsafe --out-file=utlist.html $<

utarray: utarray.txt
	asciidoc --unsafe --out-file=utarray.html $<

utstring: utstring.txt
	asciidoc --unsafe --out-file=utstring.html $<

changelog:	ChangeLog.txt
	asciidoc --out-file=ChangeLog.html ChangeLog.txt 

TMP=/tmp/uthash-gh-pages
stage:
	mkdir -p ${TMP}
	cp userguide.html ${TMP}/uthash
	cp userguide.pdf ${TMP}/uthash
	cp utlist.html ${TMP}/uthash
	cp utarray.html ${TMP}/uthash
	cp utstring.html ${TMP}/uthash
	cp ChangeLog.html ${TMP}/uthash
	cp license.html ${TMP}/uthash
	cp index.html ${TMP}/uthash
	cp styles.css ${TMP}/uthash

.PHONY: pdf

pdf: userguide.txt
	a2x --format=pdf $<

docbook: userguide.txt
	asciidoc -b docbook --out-file=/tmp/userguide.xml $<
	xmlto -o html html-nochunks /tmp/userguide.xml 
