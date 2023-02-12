all : lec01.pdf lec02.pdf lec03.pdf lec04.pdf lec05.pdf lec06.pdf

%.pdf : %/*.md
	pandoc -f markdown -t beamer --pdf-engine=lualatex -V fontsize=9pt -H setup.tex --slide-level 2 -s $^ -o $@

clean:
	rm -f *.out *.pdf
