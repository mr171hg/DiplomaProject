%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%
%% KPI Thesis Class.
%%
%% Class for writing bachelor/master/phd thesis at Computer Science department
%% at Faculty of Electrical Engineering and Informatics and Technical University
%% of Košice.
%%
%% department homepage: http://kpi.fei.tuke.sk
%% project homepage: https://git.kpi.fei.tuke.sk/tuke/thesis.in.latex
%%
%%%%%
\NeedsTeXFormat{LaTeX2e}
\ProvidesClass{kithesis}[2021/03/23 KPI Thesis Class, v2021.2]

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%
%% Using report class as base.
%%
%%%%%
\PassOptionsToClass{a4paper,12pt,oneside}{report}
\LoadClass{report}


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%
%% Additional packages, and their options.
%%
%%%%%

\RequirePackage{pdfpages}

% titlesec for modification of chapters and sections
\RequirePackage{titlesec}
\titleformat{\chapter}
    {\huge}{\textbf\thechapter}{20pt}{\huge\textbf}[{\titlerule[0.8pt]}]

% sets the document geometry
\RequirePackage[
    a4paper,
    %width=150mm,
    top=25mm,
    bottom=25mm,
    left=30mm,
    right=25mm,
    bindingoffset=6mm
]{geometry}

\RequirePackage{graphicx}
\DeclareGraphicsExtensions{.pdf,.png,.jpg,.eps,.svg}

\RequirePackage[utf8]{inputenc}
\RequirePackage[T1]{fontenc}

\RequirePackage[
    figure,
    table]{totalcount}

%% Numeric citations and sorting by citation order
\RequirePackage{csquotes}
\RequirePackage[
    bibencoding=auto,
    style=iso-numeric,      % numeric citations according for ISO 690
    autolang=other,
    sorting=none,           % sort by order of citation
    backend=biber           % different backend
]{biblatex}

% location of file with bibliography resources
\addbibresource{chapters/bibliography.bib}

% slovak translation of bibliography
\DefineBibliographyStrings{slovak}{%
    bibliography = {Literatúra}
}

\RequirePackage[
   unicode,
   %backref,
   pdfusetitle,
   %pdfusekeywords,
   pdftoolbar=true,
   pdfmenubar=true,
   pdfwindowui=true,
   bookmarksopenlevel={0},
   bookmarksnumbered,
   bookmarksopen,
   pdfhighlight={/P},
   colorlinks=false, % TODO draft vs publish ready?
   %hidelinks, % disable color links completly
   citecolor=magenta,
   baseurl={https://kpi.fei.tuke.sk/}
]{hyperref}

% PDF metadata based on macro values
\AtBeginDocument{
  \hypersetup{
    pdftitle = {\@title@en},
    pdfauthor = {\@author},
    pdfkeywords = {\@keywords@en},
    pdfsubject = {\@thesis}
  }
}

% popisky ku obrazkom a tabulkam
\RequirePackage[small]{caption}
\setlength{\captionmargin}{25pt}

% TODO These commande probably have no effect, because are overriden by babel
\def\figurename{Obrázok}
\def\tabname{Tabuľka}
% \def\refname{Zoznam použitej literatúry} % TODO replaced by biblatex stuff, should be \bibname either

%% Vlastná hlavička
\RequirePackage{fancyhdr}
\setlength{\headheight}{15pt}

% nastavenie hlavičky pre hlavnú časť práce
\pagestyle{fancy}
\renewcommand{\chaptermark}[1]{
    \markboth{\ifnum\value{chapter}>0 \chaptername\ \thechapter.\ \fi #1}{}
}
\fancyhf{}
\rhead{\itshape \nouppercase{\leftmark}}
\rfoot{\thepage}


% štýl pre prvú stranu kapitoly (plain s číslom strany v päte zarovnaný vpravo)
\fancypagestyle{chapterpage}{
    \fancyhf{}
    \renewcommand\headrulewidth{0pt}
    \rfoot{\thepage}
}


% zmena štýlu prvej stránke kapitoly
\renewcommand\chapter{\if@openright\cleardoublepage\else\clearpage\fi
                    \thispagestyle{chapterpage}%
                    \global\@topnum\z@
                    \@afterindentfalse
                    \secdef\@chapter\@schapter}

\renewcommand{\headrulewidth}{0.7pt}


% slovnik terminov a skratiek
\RequirePackage[
    toc,
    acronym,
    nonumberlist,
    noredefwarn
]{glossaries}
\loadglsentries{glossary}
\makeglossaries % prikaz na vytvorenie suboru .glo



%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%
%% Variable definitions and default values: these variables should be defined by
%% the user (somewhere in the preamble). For example, to put the abstract into
%% the thesis, the thesis writer should type the following somewhere in the
%% preamble (before the `\begin{document}` or `\frontmatter` commands are
%% called):
%%
%%      \abstract{This is my abstract.}
%%
%% See below (in the comments starting with 'DOCVAR: ') for a list of all
% variables
%% the thesis writer is expected to use.
%%
%%%%%

\def\br{\\}

% DOCVAR: thesisspec (The list of thesis specifications)
\newcommand{\@thesisspec}{
    \thispagestyle{empty}
    \vspace*{\fill}
    \begin{center}
    \iflanguage{slovak}{
        Tu vložte zadávací list pomocou príkazu\\
        {\tt{\textbackslash{}thesisspec\{cesta/k/suboru/so/zadavacim.listom\}}}\\
        v preambule dokumentu. 
        
        Kópiu zadávacieho listu skenujte čiernobielo (v odtieňoch sivej) na $200$ až $300$ DPI!
        Nezabudnite do jednej práce vložiť originál zadávacieho listu!
    }{
        Here insert the assignment statement using the command\\
        {\tt{\textbackslash{}thesisspec\{path/to/the/filw/with/assignment.statement\}}}\\
        in the preamble of the document.
        
        Copy of the assignment statement should be scanned black\&white (grayscale) using DPI from $200$ to $300$!
        Don't forget to insert the original assignment statement into one of the theses copies.
    }
    \end{center}
    \vspace*{\fill}
    \newpage
}
\newcommand{\thesisspec}[1]{
    \renewcommand{\@thesisspec}{
        \includepdf[pages={-}]{#1}
    }
}


% Command for author's name in the form of: [affil]{Name}{Surname}[affil]
% Second optional argument using https://texfaq.org/FAQ-twooptarg
\newcommand{\@author@prefix}{}
\newcommand{\@author@firstname}{}
\newcommand{\@author@lastname}{}
\newcommand{\@author@postfix}{}
\renewcommand\author[3][]{
    \renewcommand{\@author@prefix}{#1}
    \renewcommand{\@author@firstname}{#2}
    \renewcommand{\@author@lastname}{#3}
    \authorpostfix
}
\newcommand\authorpostfix[1][]{
    \renewcommand{\@author@postfix}{#1}
}


% DOCVAR: college (The name of the thesis writer's college)
\newcommand{\@college@en}{Technical University of Košice}
\newcommand{\@college@sk}{Technická univerzita v Košiciach}
\newcommand{\@college}{\@college@en}
\newcommand{\college}[2]{
    \renewcommand{\@college@en}{#1}
    \renewcommand{\@college@sk}{#2}
}

\newcommand{\@college@city}{Košice}

% DOCVAR: faculty (The name of the thesis writer's faculty)
\newcommand{\@faculty@en}{Faculty of Electrical Engineering and Informatics}
\newcommand{\@faculty@sk}{Fakulta elektrotechniky a informatiky}
\newcommand{\@faculty}{\@faculty@en}
\newcommand{\faculty}[2]{
    \renewcommand{\@faculty@en}{#1}
    \renewcommand{\@faculty@sk}{#2}
}


% DOCVAR: department (name of the thesis writer's department)
\newcommand{\@department@en}{Department of Computers and Informatics}
\newcommand{\@department@sk}{Katedra počítačov a informatiky}
\newcommand{\@department}{\@department@en}
\newcommand{\department}[2]{
    \renewcommand{\@department@en}{#1}
    \renewcommand{\@department@sk}{#2}
}

% DOCVAR: department's shortcut
\newcommand{\@departmentacr@en}{DCI}
\newcommand{\@departmentacr@sk}{KPI}
\newcommand{\@departmentacr}{\@departmentacr@en}
\newcommand{\departmentacr}[2]{
    \renewcommand{\@departmentacr@en}{#1}
    \renewcommand{\@departmentacr@sk}{#2}
}


% DOCVAR: supervisor (name of the thesis writer's supervisor)
\newcommand{\@supervisor}{}
\newcommand{\supervisor}[1]{\renewcommand{\@supervisor}{#1}}

% DOCVAR: consultant (name of the thesis writer's consultant)
\newcommand{\@consultant}{}
\newcommand{\consultant}[1]{\renewcommand{\@consultant}{#1}}

% DOCVAR: field of study
\newcommand{\@fieldofstudy}{9.2.1. Informatika}
\newcommand{\fieldofstudy}[1]{\renewcommand{\@fieldofstudy}{#1}}

% DOCVAR: study programme
\newcommand{\@studyprogramme}{Informatika}
\newcommand{\studyprogramme}[1]{\renewcommand{\@studyprogramme}{#1}}

% DOCVAR: thesis (type of the thesis)
\newcommand{\@thesis@en}{Bachelor thesis}
\newcommand{\@thesis@sk}{Bakalárska práca}
\newcommand{\@thesis}{\@thesis@en}
\newcommand{\thesis}[2]{
    \renewcommand{\@thesis@en}{#1}
    \renewcommand{\@thesis@sk}{#2}
}

% DOCVAR: declaration text
\newcommand{\@declaration@en}{I hereby declare that this thesis is my own work and effort. Where other sources of information have been used, they have been acknowledged.}
\newcommand{\@declaration@sk}{Vyhlasujem, že som záverečnú prácu vypracoval samostatne s~použitím uvedenej odbornej literatúry.}
\newcommand{\@declaration}{\@declaration@en}
\newcommand{\declaration}[1]{
    \renewcommand{\@declaration@en}{#1}
    \renewcommand{\@declaration@sk}{#1}
}

% DOCVAR: date (declaration date)
\newcommand{\@submissiondate}{}
\newcommand{\submissiondate}[3]{
    \newcommand{\@submissionday}{#1}
    \newcommand{\@submissionmonth}{#2}
    \newcommand{\@submissionyear}{#3}
}

% DOCVAR: abstract of the thesis
\renewcommand{\abstract}[2]{%
    \newcommand{\@abstract@en}{#1}%
    \newcommand{\@abstract@sk}{#2}%
}
\newcommand{\@abstract}{\@abstract@en}

% DOCVAR: thesis keywords
\newcommand{\keywords}[2]{%
    \newcommand{\@keywords@en}{#1}%
    \newcommand{\@keywords@sk}{#2}%
}
\newcommand{\@keywords}{\@keywords@en}

% DOCVAR: thesis title
\renewcommand{\title}[2]{%
    \newcommand{\@title@en}{#1}%
    \newcommand{\@title@sk}{#2}%
}
\renewcommand{\@title}{\@title@en}


% DOCVAR: thesis (type of the thesis)
\newcommand{\@acknowledgment}{Na tomto mieste by som sa rád poďakoval svojmu vedúcemu záverečnej práce za pripomienky a~odbornú pomoc.}
\newcommand{\acknowledgment}[1]{\renewcommand{\@acknowledgment}{#1}}

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%
%% Translations of strings used in the thesis
%%

% Default empty declarations
\newcommand{\declarationname}{No Language Selected}
\newcommand{\signaturename}{No Language Selected}
\newcommand{\acknowledgementname}{No Language Selected}
\newcommand{\lstlistingname}{No Language Selected}
\newcommand{\studyprogrammename}{No Language Selected}
\newcommand{\fieldofstudyname}{No Language Selected}
\newcommand{\departmentname}{No Language Selected}
\newcommand{\supervisorname}{No Language Selected}
\newcommand{\consultantname}{No Language Selected}
\newcommand{\keywordsname}{No Language Selected}
\newcommand{\secondabstractname}{No Language Selected}
\newcommand{\secondkeywordsname}{No Language Selected}
\newcommand{\bibcitationname}{No Language Selected}
\newcommand{\supervisorcitationname}{No Language Selected}
\newcommand{\acrlistname}{No Language Selected}
\newcommand{\appendixlistname}{No Language Selected}

\AtBeginDocument{
    \addto\captionsenglish{
        \renewcommand{\@title}{\@title@en}
        \renewcommand{\@abstract}{\@abstract@en}
        \renewcommand{\@keywords}{\@keywords@en}
        \renewcommand{\@college}{\@college@en}
        \renewcommand{\@department}{\@department@en}
        \renewcommand{\@departmentacr}{\@departmentacr@en}
        \renewcommand{\@faculty}{\@faculty@en}
        \renewcommand{\@thesis}{\@thesis@en}
        \renewcommand{\@declaration}{\@declaration@en}
        \renewcommand{\declarationname}{Declaration}
        \renewcommand{\signaturename}{Signature}
        \renewcommand{\acknowledgementname}{Acknowledgement}
        \renewcommand{\studyprogrammename}{Study Programme}
        \renewcommand{\fieldofstudyname}{Field of Study}
        \renewcommand{\departmentname}{Department}
        \renewcommand{\supervisorname}{Supervisor}
        \renewcommand{\consultantname}{Consultant}
        \renewcommand{\abstractname}{Abstract in English}
        \renewcommand{\keywordsname}{Keywords in English}
        \renewcommand{\secondabstractname}{Abstract in Slovak}
        \renewcommand{\secondkeywordsname}{Keywords in Slovak}
        \renewcommand{\bibcitationname}{Bibliographic Citation}
        \renewcommand{\supervisorcitationname}{Supervisor}
        \renewcommand{\lstlistingname}{Listing}
        \renewcommand{\acrlistname}{List of Abbreviations}
        \renewcommand{\appendixlistname}{List of Appendixes}
    }
    \addto\captionsslovak{
        \renewcommand{\@title}{\@title@sk}
        \renewcommand{\@abstract}{\@abstract@sk}
        \renewcommand{\@keywords}{\@keywords@sk}
        \renewcommand{\@college}{\@college@sk}
        \renewcommand{\@department}{\@department@sk}
        \renewcommand{\@departmentacr}{\@departmentacr@sk}
        \renewcommand{\@faculty}{\@faculty@sk}
        \renewcommand{\@thesis}{\@thesis@sk}
        \renewcommand{\@declaration}{\@declaration@sk}
        \renewcommand{\declarationname}{Čestné vyhlásenie}
        \renewcommand{\signaturename}{Vlastnoručný podpis}
        \renewcommand{\acknowledgementname}{Poďakovanie}
        \renewcommand{\studyprogrammename}{Študijný program}
        \renewcommand{\fieldofstudyname}{Študijný odbor}
        \renewcommand{\departmentname}{Školiace pracovisko}
        \renewcommand{\supervisorname}{Školiteľ}
        \renewcommand{\consultantname}{Konzultant}
        \renewcommand{\abstractname}{Abstrakt v SJ}
        \renewcommand{\keywordsname}{Kľúčové slová v SJ}
        \renewcommand{\secondabstractname}{Abstrakt v AJ}
        \renewcommand{\secondkeywordsname}{Kľúčové slová v AJ}
        \renewcommand{\bibcitationname}{Bibliografická citácia}
        \renewcommand{\supervisorcitationname}{Vedúci práce}
        \renewcommand{\lstlistingname}{Zdrojový kód}
        \renewcommand{\acrlistname}{Zoznam skratiek}
        \renewcommand{\appendixlistname}{Zoznam príloh}
        \renewcommand{\appendixname}{Príloha}
    }
}

% right page number aligment from 2nd page in table of content
\fancypagestyle{plain}{%
    \renewcommand{\footrulewidth}{0pt}
}

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%
%% Front matter
%%
%% - outside and inside front cover
%% - title leaf
%% Do not include the date of make!
%% Institution + department.
%% Names of referees. (optional)
%% Degree.
%% Date of submission and defense. (optional)
%% Place and date of publication and publishers (and other info by them).
%
\newcommand{\frontmatter}{
    \pagenumbering{roman}
    \frontpage
    \titlepage
    \abstractpage

    % thesis detail specification
    \@thesisspec

    \declarationpage
    \acknowledgmentpage

    \pagestyle{plain}
    \tableofcontents
    \newpage

    % show list of figures only if there are some
    \iftotalfigures%
        \listoffigures%
        \newpage%
    \fi


    % show list of tables only if there are some
    \iftotaltables%
        \listoftables%
        \newpage%
    \fi

	\renewcommand\lstlistlistingname{Zoznam zdrojových kódov}
	\lstlistoflistings{}
	\newpage	

	\printglossary[type=\acronymtype,title={\acrlistname}]
	   \newpage

    \pagestyle{fancy}
    \pagenumbering{arabic}
    \newpage
}


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%
%% Front page component
%%
%%%%%
\newcommand{\frontpage}{

    \thispagestyle{empty}

    \begin{center}
        {\Large \textbf{\@college}}

        {\Large \textbf{\@faculty}}

        \vfill

        {\LARGE \textbf{\@title} \par}

        \bigskip

        {\large \textbf{\@thesis}}
    \end{center}

    \vfill

    \begin{center}
        {\Large \textbf{\@submissionyear}} \hfill {\Large \textbf{\@author@prefix~\@author@firstname~\@author@lastname~\@author@postfix}}
    \end{center}

    \newpage
}


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%
%% Abstract component
%%
%%%%%

\newcommand{\abstractpage}{
    \def\br{}

    \thispagestyle{empty}
    \noindent
    \subsection*{\abstractname}

    \@abstract
    
    \subsection*{\keywordsname}

    \@keywords

    \iflanguage{slovak}{
        \subsection*{\secondabstractname}

        \begin{otherlanguage}{english}
            \@abstract
        \end{otherlanguage}
        
        \subsection*{\secondkeywordsname}

        \begin{otherlanguage}{english}
            \@keywords
        \end{otherlanguage}
    }{
        \subsection*{\secondabstractname}

        \begin{otherlanguage}{slovak}
            \@abstract
        \end{otherlanguage}
        
        \subsection*{\secondkeywordsname}

        \begin{otherlanguage}{slovak}
            \@keywords
        \end{otherlanguage}
    }

    \vfill

    \subsection*{\bibcitationname}

    \MakeUppercase\@author@lastname, \@author@firstname. {\itshape \@title}. \@college@city: \@college, \@faculty, \@submissionyear. \pageref{theend}s. \supervisorcitationname: \@supervisor

    \newpage

    \def\br{\\}
}


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%
%% Declaration component
%%
%%%%%
\newcommand\declarationpage{
    \thispagestyle{empty}

    \vglue0pt
    \vfill
    \subsection*{\declarationname}

    \noindent\@declaration

    \bigskip

    \noindent \@college@city,~\@submissionday.\@submissionmonth.\@submissionyear \hfill
    \begin{tabular}[t]{c}
        \hbox to 50mm {\dotfill} \\ \textit{\small \signaturename}
    \end{tabular}
    \newpage
}


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%
%% Declaration component
%%
%%%%%
\newcommand\acknowledgmentpage{%
    \thispagestyle{empty}%
    \vglue0pt\vfill%

    \subsection*{\acknowledgementname}%

    \@acknowledgment

    \newpage
}


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%
%% Title page component
%%
%%%%%
\renewcommand{\titlepage}{
    \thispagestyle{empty}

    \begin{center}
        {\Large \textbf{\@college}}

        {\Large \textbf{\@faculty}}

        \vfill

        {\LARGE \textbf{\@title} \par}

        \bigskip

        {\large \textbf{\@thesis}}
    \end{center}

    \vfill

    \noindent
    \begin{tabular}{@{}p{.26\textwidth}p{.7\textwidth}}
        \studyprogrammename:& \@studyprogramme \\
        \fieldofstudyname:& \@fieldofstudy \\
        \departmentname:& \@department~(\@departmentacr) \\
        \supervisorname:& \@supervisor \\
        \consultantname:& \@consultant \\
    \end{tabular}

    \vspace{2\baselineskip}

    \begin{center}
        \centering
        {\Large \textbf{\@college@city{} \@submissionyear}} \hfill {\Large \textbf{\@author@prefix~\@author@firstname~\@author@lastname~\@author@postfix}}
    \end{center}

    \newpage
}


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%
%% Fonts
%%
%%%%%

% Font for some math characters to work with TeX Gyre Pagella font

\RequirePackage{textcomp}

% Font used for thesis is TeX Gyre Pagella.
% Note: You might need to install it first.

\RequirePackage{tgpagella}

% Use TXTT as a typewriter font. It supports bold and matches Pagella in style.
\renewcommand*\ttdefault{txtt}

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%
%% Line spacing and paragraph indentation
%%
%%%%%

\linespread{1.3}
\setlength{\parindent}{1.5em}

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

\newcommand{\blindtext}{Lorem ipsum dolor sit amet, consectetuer adipiscing elit. Etiam lobortis facilisissem. Nullam nec mi et neque pharetra sollicitudin. Praesent imperdiet mi necante. Donec ullamcorper, felis non sodales commodo, lectus velit ultrices augue, a dignissim nibh lectus placerat pede. Vivamus nunc nunc, molestie ut, ultriciesvel, semper in, velit. Ut porttitor. Praesent in sapien. Lorem ipsum dolor sit amet, consectetuer adipiscing elit. Duis fringilla tristique neque. Sed interdum libero utmetus. Pellentesque placerat. Nam rutrum augue a leo. Morbi sed elit sit ametante lobortis sollicitudin. Praesent blandit blandit mauris. Praesent lectus tellus, aliquet aliquam, luctus a, egestas a, turpis. Mauris lacinia lorem sit amet ipsum. Nunc quis urna dictum turpis accumsan semper.}
