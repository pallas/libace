default: all

PROJECT := lace
PREFIX ?= /usr/local

HEADERS := $(wildcard *.h)

.PHONY: install
install:
	install -D -m 0644 -t $(PREFIX)/include/$(PROJECT) $(HEADERS)

#
