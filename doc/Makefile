ASCIIDOC = asciidoc

all : wisp-guide.html

wisp-guide.html : wisp-guide.txt
	$(ASCIIDOC) -o $@ $^

.PHONY : all clean
clean : 
	$(RM) wisp-guide.html
