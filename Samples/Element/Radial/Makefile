#
# DynELA Finite Element Code Makefile
#
# (c) by Olivier Pantalé 2020
#
# -------------------------------------------------------------------------------
# Beginning of the private area for the Makefile
# Local modifications must be made in this area.
# -------------------------------------------------------------------------------
#@!CODEFILE = Makefile-sample-subdir
#@!BEGIN = PRIVATE

# List of subdirectories to take into account
SUBDIRS = ElQua4NAx

#@!BEGIN = COMMON
# -------------------------------------------------------------------------------
# There's nothing to touch here next in the Makefile
# If there are any changes to be made, they must be made in the main Makefile
# or else these changes will be overwritten the next time the file is automatically updated.
# -------------------------------------------------------------------------------

# Store the current path
HERE = $(PWD)

# Default option for the Makefile
all:
	@for SUBDIR in $(SUBDIRS); do cd $(HERE)/$$SUBDIR; make; done

# Propagates the clean command to subdirectories
clean:
	@for SUBDIR in $(SUBDIRS); do cd $(HERE)/$$SUBDIR; make clean; done

# Propagates the subclean command to subdirectories
subclean:
	@for SUBDIR in $(SUBDIRS); do cd $(HERE)/$$SUBDIR; make subclean; done

