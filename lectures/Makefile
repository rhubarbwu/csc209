SCP_ADDRESS=rupert@cs.toronto.edu:public_html/209/

all : lec01.pdf lec02.pdf lec03.pdf lec04.pdf lec05.pdf lec06.pdf lec07.pdf lec08.pdf lec09.pdf

%.up : %.pdf
	scp $< ${SCP_ADDRESS}

up : lec01.pdf lec02.pdf lec03.pdf lec04.pdf lec05.pdf lec06.pdf lec07.pdf lec08.pdf lec09.pdf
	scp $^ ${SCP_ADDRESS}

%.pdf : %/*.md
	pandoc -f markdown -t beamer --pdf-engine=lualatex -V fontsize=9pt -H setup.tex --slide-level 2 -s $^ -o $@
	# gs -sDEVICE=pdfwrite -dCompatibilityLevel=1.4 -dPDFSETTINGS=/prepress -dNOPAUSE -dQUIET -dBATCH -sOutputFile=c$@ $@

clean:
	rm -f *.out *.pdf
