#
# Makefile for creating PDF from Latex
#
#  DynELA Finite Element Code v.4.0
#  by Olivier PANTALE
#  Olivier.Pantale@enit.fr
#
# -------------------------------------------------------------------------------
# Beginning of the private area for the Makefile
# Local modifications must be made in this area.
# -------------------------------------------------------------------------------
#@!CODEFILE = Makefile-documentation
#@!BEGIN = PRIVATE

# Define the top file to create
TOP      = DynELA.pdf

# Define the location of all figures where SVG files can be found
FIGURES_FOLDERS = Figures
FIGURES         = $(wildcard Figures/*)

# Define how to create indexes
define makeIndexes
	-makeindex $(ROOT).0dx -o $(ROOT).0nd
endef

#@!BEGIN = COMMON
# -------------------------------------------------------------------------------
# There's nothing to touch here next in the Makefile
# If there are any changes to be made, they must be made in the main Makefile
# or else these changes will be overwritten the next time the file is automatically updated.
# -------------------------------------------------------------------------------

# Programs to use
TEX2PDF      = pdflatex -interaction=batchmode -synctex=1
BIBTEX       = -bibtex

# Internal variables
SOURCES      = $(wildcard *.tex)
MAINTOP      = $(TOP:.pdf=.tex)
ROOT         = $(TOP:.pdf=)

# Store current path
HERE = $(PWD)

# Default option for the Makefile
all: figures $(TOP)

# Convert all SVG figures in Figures folder to PDF
figures:
	@for FIGURES_FOLDER in $(FIGURES_FOLDERS); do cd $(HERE)/$$FIGURES_FOLDER; svgToPdf --silent; done

# Makes the top target
$(TOP) : $(SOURCES) $(FIGURES)
	# Remove old file
	-rm $(TOP)
	# Run pass 1
	$(TEX2PDF) $(MAINTOP)
	# Run bibtex skipping error
	$(BIBTEX) $(ROOT).aux
	# Run pass 2
	$(TEX2PDF) $(MAINTOP)
	# Run the indexes
	$(call makeIndexes)
	# Run pass 3
	$(TEX2PDF) $(MAINTOP)

# Clean the LateX temp files
clean: 
	rm -rf *.aux *.log *.nav *.out *.toc *.vrb *.bbl *.blg *.snm *.mtc* *.?dx *.maf *.ptc *.brf *.lof *.lot *.ilg *.?nd

# Clean the target to force compilation again
forceClean:
	rm $(TOP)

