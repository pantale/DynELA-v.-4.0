#
# DynELA Finite Element Code Makefile
#
# (c) by Olivier Pantalé 2020
#
# -------------------------------------------------------------------------------
# Beginning of the private area for the Makefile
# Local modifications must be made in this area.
# -------------------------------------------------------------------------------
#@!CODEFILE = Makefile-sample-abaqus
#@!BEGIN = PRIVATE

# Abaqus file to treat
FILE = Radial

#@!BEGIN = COMMON
# -------------------------------------------------------------------------------
# There's nothing to touch here next in the Makefile
# If there are any changes to be made, they must be made in the main Makefile
# or else these changes will be overwritten the next time the file is automatically updated.
# -------------------------------------------------------------------------------

# Default option for the Makefile
all:
	abaqus job=$(FILE) input=$(FILE) double=both interactive ask_delete=OFF
	abaqus python Extract.py
	@echo "Sleep for 2 seconds for files sync"
	@sleep 2
	@rm -f abaqus.rpy*
	replaceInFile "$(FILE)_*.plot" $(FILE)_ Abaqus-
	@rm -f sauvegarde_replaceInFile.tar.gz
	@rm -f $(FILE)*.abq $(FILE)*.com $(FILE)*.log $(FILE)*.mdl $(FILE)*.msg $(FILE)*.pac $(FILE)*.prt $(FILE)*.res $(FILE)*.sel $(FILE)*.stt $(FILE)*.dat $(FILE)*.sta

# Copy the SVG figures to the documentation directory and convert them to PDF
install:
	@for SVGFILE in $(OUTPUTS); do svgToPdf -file $$SVGFILE -dest $(OUTDIR) -prefix $(GENERIC)_ --silent; done

# Clean the files in the current directory
clean:
	@echo "Cleaning: $(PWD)"
	@rm -f *.odb *.plot

# Clean the subdirectory, i.e. run clean
subclean: clean

