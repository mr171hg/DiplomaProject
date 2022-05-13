rem prikazy ktore korektne prelozia Sablonu 2 
rem KEMT FEI TUKE, M.D. 2021-02-21
rem vysledkom je subor thesis.pdf

pdflatex thesis.tex
biber thesis
makeindex -s thesis.ist -t thesis.glg -o thesis.gls thesis.glo
makeindex -s thesis.ist -t tthesis.alg -o thesis.acr thesis.acn
pdflatex thesis.tex
pdflatex thesis.tex
