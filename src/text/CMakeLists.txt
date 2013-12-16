configure_file(gpl.string gpl.dos.string COPYONLY)
set_property(SOURCE gpl.dos.string PROPERTY GENERATED)
configure_file(help.string help.dos.string COPYONLY)
set_property(SOURCE help.dos.string PROPERTY GENERATED)

ADD_CUSTOM_TARGET(texts DEPENDS gpl.dos.string help.dos.string)
