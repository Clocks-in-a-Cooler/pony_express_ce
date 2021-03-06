# ----------------------------
# Makefile Options
# ----------------------------

NAME ?= PNYEXPRS
DESCRIPTION ?= "Pony Express CE"
COMPRESSED ?= NO
ARCHIVED ?= NO
HAS_PRINTF := NO

CFLAGS ?= -Wall -Wextra -Oz
CXXFLAGS ?= -Wall -Wextra -Oz

# ----------------------------

ifndef CEDEV
$(error CEDEV environment path variable is not set)
endif

include $(CEDEV)/meta/makefile.mk

# these don't seem to work

# $(OBJDIR)/font.c.src: $(SRCDIR)/font/oldie_font.inc $(SRCDIR)/font/font.c

# $(SRCDIR)/font/oldie_font.inc: $(SRCDIR)/font/oldie-13.fnt
# 	echo "converting font $<"
# 	convfont -o carray -f $< $@