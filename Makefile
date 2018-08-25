# Makefile for creating stuff on host.
# Basically for maintainer only.
# Use Arduino IDE for compiling for Arduino.

# The functions for generating keywords.txt requires KeywordsTxtGenerator,
# https://github.com/bengtmartensson/KeywordsTxtGenerator, to be installed in
# KEYWORD_TXT_GENERATOR_DIR.

ifneq ($(ARDUINO),)
$(error This Makefile is not for compiling target code, for that, use the Arduino IDE.)
endif

KEYWORD_TXT_GENERATOR_DIR = ../KeywordsTxtGenerator
DOXYGEN := doxygen
DOXYFILE := Doxyfile
XSLTPROC := xsltproc
TRANSFORMATION := $(KEYWORD_TXT_GENERATOR_DIR)/doxygen2keywords.xsl
BROWSER=firefox

# Get VERSION from the version in library.properties
VERSION=$(subst version=,,$(shell grep version= library.properties))
ORIGINURL=$(shell git remote get-url origin)

default: all

#VERSION_H := src/version/version.h
#
#version: $(VERSION_H)
#
#$(VERSION_H): library.properties Makefile
#	echo "// This file was automatically generated from $<; do not edit." > $@
#	echo "/**"                                                           >> $@
#	echo " * Version of the current library."                            >> $@
#	echo " * Taken from the version in $<."                              >> $@
#	echo " */"                                                           >> $@
#	echo "#define VERSION \"$(VERSION)\""                                >> $@

doc: api-doc/index.html
	$(BROWSER) $<

api-doc/index.html xml/index.xml: $(wildcard src/* src/*/*) README.md
	$(DOXYGEN) $(DOXYFILE) > /dev/null

gh-pages: api-doc/index.html
	rm -rf gh-pages
	git clone --depth 1 -b gh-pages $(ORIGINURL) gh-pages
	rm -rf gh-pages/*
	cp -rf api-doc/* gh-pages
	(cd gh-pages; git add . )
	(cd gh-pages; git commit -a -m "Update of API documentation for version $(VERSION)")
	@echo Now perform \"git push\" from gh-pages

tag:
	git checkout master
	git status
	git tag -a Version-$(VERSION) -m "Tagging Version-$(VERSION)"
	git push origin Version-$(VERSION)

clean:
	rm -rf *.a *.o api-doc xml gh-pages

# Remove all products. Do not use before commit.
spotless: clean
	rm -f keywords.txt $(VERSION_H)

keywords: keywords.txt

keywords.txt: xml/index.xml
	$(XSLTPROC) $(TRANSFORMATION) $< | uniq > $@

all: keywords api-doc/index.html

.PHONY: clean spotless all default
