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
FILE = BarNecking

# Pictures for documentation
GENERIC = BarNecking-3D
OUTPUTS = vonMisesCP.svg temperatureCP.svg radius.svg temperature.svg plasticStrain.svg vonMises.svg timeStep.svg kineticEnergy.svg
OUTDIR = $(DYNELA)/Documentation/Figures/Samples/Plasticity

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

