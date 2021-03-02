(TeX-add-style-hook
 "C19-C LaTeX Notes"
 (lambda ()
   (TeX-add-to-alist 'LaTeX-provided-package-options
                     '(("placeins" "section") ("hyphenat" "none") ("geometry" "margin={1.25in, 1in}") ("inputenc" "utf8") ("babel" "english") ("tocbibind" "nottoc" "notlot" "notlof")))
   (add-to-list 'LaTeX-verbatim-macros-with-braces-local "path")
   (add-to-list 'LaTeX-verbatim-macros-with-braces-local "url")
   (add-to-list 'LaTeX-verbatim-macros-with-braces-local "nolinkurl")
   (add-to-list 'LaTeX-verbatim-macros-with-braces-local "hyperbaseurl")
   (add-to-list 'LaTeX-verbatim-macros-with-braces-local "hyperimage")
   (add-to-list 'LaTeX-verbatim-macros-with-braces-local "hyperref")
   (add-to-list 'LaTeX-verbatim-macros-with-braces-local "href")
   (add-to-list 'LaTeX-verbatim-macros-with-delims-local "path")
   (TeX-run-style-hooks
    "latex2e"
    "article"
    "art10"
    "graphicx"
    "amssymb"
    "amsmath"
    "indentfirst"
    "hyperref"
    "caption"
    "placeins"
    "titlesec"
    "tikz"
    "multicol"
    "hyphenat"
    "sectsty"
    "geometry"
    "lipsum"
    "fancyhdr"
    "inputenc"
    "babel"
    "float"
    "tocbibind"
    "mathtools"
    "enumitem"
    "chngcntr"
    "tocloft"
    "amsthm"
    "xcolor"
    "listings")
   (TeX-add-symbols
    '("fancyfootnotetext" 2)
    '("circled" 1)
    '("DeclareAutoPairedDelimiter" 3)
    "noexpand"
    "di"
    "oldcenter"
    "oldendcenter")
   (LaTeX-add-environments
    "Figure"
    "Table")
   (LaTeX-add-pagestyles
    "dingens")
   (LaTeX-add-lengths
    "tindent")
   (LaTeX-add-mathtools-DeclarePairedDelimiters
    '("csname" ""))
   (LaTeX-add-amsthm-newtheorems
    "theorem"
    "question"
    "corollary"
    "lemma"
    "proposition"
    "definition")
   (LaTeX-add-xcolor-definecolors
    "mGreen"
    "mGray"
    "mPurple"
    "backgroundColour"))
 :latex)

