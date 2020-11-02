#
# DynELA Finite Element Code Makefile
#
# (c) by Olivier Pantalé 2020
#
# -------------------------------------------------------------------------------
# Beginning of the private area for the Makefile
# Local modifications must be made in this area.
# -------------------------------------------------------------------------------
#@!CODEFILE = Makefile-sample-dynela
#@!BEGIN = PRIVATE

# DynELA file to treat
FILE = Tensile

# Pictures for documentation
GENERIC = Tensile
OUTPUTS = plasticStrain.svg stress_11.svg stress_12.svg stress_22.svg temperature.svg vonMises.svg
OUTDIR = $(DYNELA)/Documentation/Figures/Samples/Element

#@!BEGIN = COMMON
# -------------------------------------------------------------------------------
# There's nothing to touch here next in the Makefile
# If there are any changes to be made, they must be made in the main Makefile
# or else these changes will be overwritten the next time the file is automatically updated.
# -------------------------------------------------------------------------------

# Defines the TOP file
TOP = $(DYNELA)/Samples

# Default option for the Makefile
all:
	./runBench.py --run
	cp _bench.png $(TOP)/$(FILE).png

# Makes a new reference result
reference:
	./runBench.py --reference

# Copy the SVG figures to the documentation directory and convert them to PDF
install:
	@for SVGFILE in $(OUTPUTS); do svgToPdf -file $$SVGFILE -dest $(OUTDIR) -prefix $(GENERIC)_ --silent; done

# Clean the files in the current directory
clean:
	@echo "Cleaning: $(PWD)"
	@rm -f *.vtk *.ref *.svg *.plot *.log _bench.png

# Clean the subdirectory
subclean: clean
	@cd Abaqus; make subclean

