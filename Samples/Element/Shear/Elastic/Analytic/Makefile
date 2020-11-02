# DynELA file to treat
FILE = Shear-Elastic

# Nothing to touch here after
TOP = $(DYNELA)/Samples

all:
	python AnalyticSolutions.py

clean:
	@echo "Cleaning: $(PWD)"
	@rm -f *.plot

subclean: clean
