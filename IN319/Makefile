CC=g++
CXXFLAGS=-O2 -L/usr/X11R6/lib  -lm  -lpthread -lX11
TMPFOLDER=tmp
BUILDFOLDER=_build


all: Projet


Projet: Projet.cpp
	@ if [ ! -d $(BUILDFOLDER) ]; then mkdir $(BUILDFOLDER); fi
	$(CC) -o $(BUILDFOLDER)/Projet $^ $(CXXFLAGS)

clean: 
	rm -rf $(TMPFOLDER)/*.o
	rm -rf $(BUILDFOLDER)/*
