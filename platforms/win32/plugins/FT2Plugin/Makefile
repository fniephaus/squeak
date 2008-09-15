include ../../Makefile.plugin


# Change the freetype library directory, if necessary

FT2_DIR := ../../../freetype-2.3.7


# Special CFLAGS and OFLAGS
INCLUDES+= -I$(FT2_DIR)/include -I$(FT2_DIR)/include/freetype


$(PLUGINLIB): $(VMDIR) $(OBJDIR) $(LIBOBJ)
	$(DLLTOOL) -d $(WIN32DIR)/freetype.def -l $(OBJDIR)/$(PLUGINLIB)
	$(AR) $(OBJDIR)/$(PLUGINLIB) $(LIBOBJ)

