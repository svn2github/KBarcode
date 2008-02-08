
add_subdirectory( pics )

include_directories( ${CMAKE_SOURCE_DIR} ${KDE4_INCLUDE_DIR} 
  ${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}  )
add_definitions( ${KDE4_DEFINITIONS} )

########### next target ###############


set(kbarcode_SRCS
  barcodecombo.cpp
  barcodedialog.cpp
  barcodedialogs.cpp
  barcodeitem.cpp
  barcodeprinterdlg.cpp
  barkode.cpp
  barkodeengine.cpp
  batchiface_skel.cpp
  batchprinter.cpp
  batchwizard.cpp
  commands.cpp
  configdialog.cpp
  confwizard.cpp
  csvfile.cpp
  csvimportdlg.cpp
  databasebrowser.cpp
  definition.cpp
  definitiondialog.cpp
  documentitem.cpp
  documentitemdlg.cpp
  dsrichtext.cpp
  dstextedit.cpp
  editoriface_skel.cpp
  encodingcombo.cpp
  encodingcombo.moc.cpp
  gnubarcode.cpp
  gnubarkodeengine.cpp
  imageitem.cpp
  kactionmap.cpp
  kbarcode.cpp
  kbarcodeiface_skel.cpp
  kbarcodesettings.cpp
  label.cpp
  labeleditor.cpp
  labelutils.cpp
  lineitem.cpp
  main.cpp
  mainwindow.cpp
  measurements.cpp
  mimesources.cpp
  multilineeditdlg.cpp
  mycanvasitem.cpp
  mycanvasview.cpp
  mydatatable.cpp
  newlabel.cpp
  pixmapbarcode.cpp
  previewdialog.cpp
  printersettings.cpp
  printlabeldlg.cpp
  propertywidget.cpp
  purepostscript.cpp
  rectitem.cpp
  rectsettingsdlg.cpp
  smalldialogs.cpp
  sqltables.cpp
  tbarcode2.cpp
  tcanvasitem.cpp
  tec.cpp
  textitem.cpp
  textlineedit.cpp
  textlineitem.cpp
  tokendialog.cpp
  tokenprovider.cpp
  xmlutils.cpp
  zplutils.cpp
  )

kde4_add_ui_files( kbarcode_SRCS 
  )

kde4_add_executable(kbarcode ${kbarcode_SRCS})

target_link_libraries(kbarcode ${KDE4_KDECORE_LIBS} ${KDE4_KDEUI_LIBS}
			       ${KDE4_KIO_LIBS} ${KDE4_KMETADATA}
			       ${KDE4_KJS_LIBS} )

install(TARGETS kbarcode DESTINATION ${BIN_INSTALL_DIR})


########### install files ###############

#install( FILES  kbarcode.desktop DESTINATION ${XDG_APPS_DIR} )

kde4_install_icons( ${ICON_INSTALL_DIR} )
