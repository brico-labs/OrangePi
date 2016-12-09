source  := src
output  := out
sources := $(sort $(wildcard $(source)/*.md))
lang    := es-ES
target  := OrangePi

# pdf control, other fonts could be:
# 'Liberation Sans',  'Liberation Mono'
# 'Source Sans Pro', 'Source Code Pro'
# 'Arial'
mainfont := 'Ubuntu'
monofont := 'Ubuntu Mono'

pdf_opt := --smart --standalone --variable geometry:a4paper --variable lang=$(lang) \
           --number-sections --toc --from=markdown --to latex --latex-engine=xelatex \
           --variable colorlinks \
           --variable mainfont=$(mainfont) \
           --variable monofont=$(monofont) \
           --variable fontsize='12pt' \

## Recipes for targets

# These targets are not files
.PHONY: clean pdf latex mediawiki epub github

# all -- This target try to build every thing
all: pdf latex mediawiki epub github docx

# reset -- This target deletes every target and then tries to build everithing
reset: clean all

# pdf  -- buid pdf into output directory
pdf: $(output)/$(target).pdf $(sources)

$(output)/$(target).pdf: $(sources)
	pandoc $(pdf_opt) \
	--output=$(output)/$(target).pdf \
	$(sources)

# latex  -- buid latex file into output directory
latex: $(output)/$(target).tex $(sources)

$(output)/$(target).tex: $(sources)
	pandoc $(pdf_opt) \
	--output=$(output)/$(target).tex \
	$(sources)

# mediawiki  -- buid mediawiki file into output directory
mediawiki: $(output)/$(target).mw $(sources)
$(output)/$(target).mw: $(sources)
	pandoc --from markdown --to mediawiki \
	--output=$(output)/$(target).mw \
	$(sources)

#epub  -- buid epub file into output directory
epub: $(output)/$(target).epub $(sources)
$(output)/$(target).epub: $(sources)
	pandoc --from markdown --to epub \
	--output=$(output)/$(target).epub \
	$(sources)

#odt  -- buid odt file into output directory
odt: $(output)/$(target).odt $(sources)
$(output)/$(target).odt: $(sources)
	pandoc --from markdown --to odt \
	--output=$(output)/$(target).odt \
	$(sources)

#docx  -- buid docx file into output directory
docx: $(output)/$(target).docx $(sources)
$(output)/$(target).docx: $(sources)
	pandoc --from markdown --to docx \
	--output=$(output)/$(target).docx \
	$(sources)

# github   -- buid github README.md file into repo root directory
github: README.md $(sources)
README.md: $(sources)
	pandoc --from markdown --to markdown_github \
	--output=../README.md \
	$(sources)

clean:
	rm -f $(output)/*.pdf
	rm -f $(output)/*.tex
	rm -f $(output)/*.mw
	rm -f $(output)/*.odt
	rm -f $(output)/*.docx
	rm -r ../README.md
