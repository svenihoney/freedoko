if(LATEX_COMPILER)
  ADD_LATEX_DOCUMENT(FreeDoko_concept.tex DEFAULT_PDF MANGLE_TARGET_NAMES) 
  ADD_LATEX_DOCUMENT(FreeDoko_rules.tex   DEFAULT_PDF MANGLE_TARGET_NAMES)
  ADD_LATEX_DOCUMENT(shortrules.tex       DEFAULT_PDF MANGLE_TARGET_NAMES)

#  install(FILES 
#    FreeDoko_concept.pdf FreeDoko_rules.pdf shortrules.pdf
#    DESTINATION ${DOC_INSTALL_DIR}/en
#    )
endif(LATEX_COMPILER)

install(FILES
  README.html README.txt
  Windows
  DESTINATION ${DOC_INSTALL_DIR}/en
  )
