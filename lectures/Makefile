all : lec01.pdf lec02.pdf lec03.pdf lec04.pdf lec05.pdf lec06.pdf lec07.pdf lec08.pdf lec09.pdf lec10.pdf lec11.pdf lec12.pdf

%.pdf : %
	pandoc -f markdown -t beamer --pdf-engine=lualatex -V fontsize=9pt -H setup.tex --slide-level 2 -s $^/*.md -o $@
	# gs -sDEVICE=pdfwrite -dCompatibilityLevel=1.4 -dPDFSETTINGS=/prepress -dNOPAUSE -dQUIET -dBATCH -sOutputFile=$@ $@

clean:
	rm -f *.out *.pdf
