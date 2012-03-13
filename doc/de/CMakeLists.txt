include(${CMAKE_SOURCE_DIR}/cmake/UseLATEX.cmake)

ADD_LATEX_DOCUMENT(DDV_Kurzregeln.tex    DEFAULT_PDF MANGLE_TARGET_NAMES)
ADD_LATEX_DOCUMENT(FreeDoko_Konzept.tex  DEFAULT_PDF MANGLE_TARGET_NAMES)
ADD_LATEX_DOCUMENT(Kurzregeln.tex        DEFAULT_PDF MANGLE_TARGET_NAMES)
ADD_LATEX_DOCUMENT(DDV_Turnierregeln.tex DEFAULT_PDF MANGLE_TARGET_NAMES)
ADD_LATEX_DOCUMENT(FreeDoko_Regeln.tex   DEFAULT_PDF MANGLE_TARGET_NAMES)

install(FILES 
DDV_Kurzregeln.pdf
#DDV_Turnierregeln.pdf
#FreeDoko_Konzept.pdf
#FreeDoko_Regeln.pdf
#Kurzregeln.pdf
GPL.de
LiesMich.html
LiesMich.txt
subversion
SuSE
Windows
Windows.kompilieren
DESTINATION ${DOC_INSTALL_DIR}/de )
