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
	cp *.html *.pdf *.png *.css ${TMP}

.PHONY: pdf

pdf: userguide.txt
	a2x --format=pdf $<

docbook: userguide.txt
	asciidoc -b docbook --out-file=/tmp/userguide.xml $<
	xmlto -o html html-nochunks /tmp/userguide.xml 
