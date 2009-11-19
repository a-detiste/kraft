
include_directories(${KDE4_INCLUDES} ${KDE4_INCLUDE_DIR} ${QT_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR})

add_subdirectory(pics)

########### next target ###############

set( kraftcat_LIB_SRCS
    catalogtemplate.cpp
    katalog.cpp
    katalogman.cpp
    brunskatalog.cpp
    brunsrecord.cpp
    defaultprovider.cpp
    doctext.cpp
    doctype.cpp
    kataloglistview.cpp
    katalogview.cpp
    brunskataloglistview.cpp
    brunskatalogview.cpp
    filterheader.cpp
    catalogchapteredit.cpp
    kraftdb.cpp
    geld.cpp
    einheit.cpp
    attribute.cpp
    numbercycle.cpp
    extendedcombo.cpp
    )


kde4_add_ui_files(kraftcat_LIB_SRCS flostab.ui)

kde4_add_kcfg_files(kraftcat_LIB_SRCS katalogsettings.kcfgc kraftsettings.kcfgc)

kde4_add_library(kraftcat SHARED ${kraftcat_LIB_SRCS})

target_link_libraries(kraftcat ${KDE4_KDECORE_LIBS}  ${KDE4_KIO_LIBS} ${QT_QTSQL_LIBRARY} ${QT_QT3SUPPORT_LIBRARY} ${KDE4_KDEUI_LIBS} kde3support)

set_target_properties(kraftcat PROPERTIES VERSION 1.0.0 SOVERSION 1)
install(TARGETS kraftcat ${INSTALL_TARGETS_DEFAULT_ARGS})

########### next target ###############

LINK_DIRECTORIES( lib )
set(kplant_SRCS brunsviewmain.cpp brunsviewer.cpp)

kde4_add_executable(kplant ${kplant_SRCS} )

target_link_libraries(kplant kraftcat ${KDE4_KDECORE_LIBS} ${KDE4_KIO_LIBS} ${QT_QT3SUPPORT_LIBRARY}  ${KDE4_KDE3SUPPORT_LIBS})

install(TARGETS kplant ${INSTALL_TARGETS_DEFAULT_ARGS})

########### next target ###############

set(kraft_SRCS
    templkatalog.cpp
    docposition.cpp
    matkatalog.cpp
    zeitcalcpart.cpp
    fixcalcpart.cpp
    floskeltemplate.cpp
    floskel.cpp
    calcpart.cpp
    kraftview.cpp
    kraftdoc.cpp
    main.cpp
    unitmanager.cpp
    stockmaterial.cpp
    stockmaterialman.cpp
    materialcalcpart.cpp
    flostempldialog.cpp
    stdsatzman.cpp
    materialtempldialog.cpp
    zeitcalcdialog.cpp
    fixcalcdialog.cpp
    mateditor.cpp
    matdatatable.cpp
    matcalcdialog.h
    matcalcdialog.cpp
    templatesaverbase.cpp
    templatesaverdb.cpp
    materialsaverbase.cpp
    materialsaverdb.cpp
    prefsdialog.cpp
    templkatalogview.cpp
    templkataloglistview.cpp
    positionviewwidget.cpp
    documentsaverbase.cpp
    documentsaverdb.cpp
    documentman.cpp
    docdigest.cpp
    docdigestview.cpp
    doctypeedit.cpp
    portal.cpp
    portalview.cpp
    archiveman.cpp
    reportgenerator.cpp
    htmlview.cpp
    docpostcard.cpp
    catalogselection.cpp
    addressselection.cpp
    kraftdocheaderedit.cpp
    kraftdocfooteredit.cpp
    inserttempldialog.cpp
    archdocposition.cpp
    archdoc.cpp
    materialkataloglistview.cpp
    materialkatalogview.cpp
    materialselectdialog.cpp
    kraftdocedit.cpp
    kraftdocpositionsedit.cpp
    portalhtmlview.cpp
    templtopositiondialogbase.cpp
    insertplantdialog.cpp
    headerselection.cpp
    textselection.cpp
    newdocassistant.cpp
    docassistant.cpp
    texteditdialog.cpp
    templateprovider.cpp
    headertemplateprovider.cpp
    footertemplateprovider.cpp
    catalogtemplateprovider.cpp
    addresstemplateprovider.cpp
    texttemplate.cpp
    doclocaledialog.cpp
    tagman.cpp
    positiontagdialog.cpp
    tagtemplatesdialog.cpp
    importitemdialog.cpp
    importfilter.cpp
    kraftview_ro.cpp
    taxeditdialog.cpp
    numbercycledialog.cpp )

kde4_add_kcfg_files(kraft_SRCS katalogsettings.kcfgc kraftsettings.kcfgc)

kde4_add_ui_files(kraft_SRCS
    zeitpartui.ui
    fixpartui.ui
    matpartui.ui
    inserttmplbase.ui
    materialdialog.ui
    insertplantbase.ui
    docheader.ui
    positionwidget.ui
    docfooter.ui
    texteditbase.ui
    positionwidget.ui
    doctypeeditbase.ui
    doctypedetailseditbase.ui
    taxeditbase.ui
    importtodocbase.ui
    numbercycleseditbase.ui

    )


kde4_add_executable(kraft ${kraft_SRCS})

target_link_libraries(kraft kraftcat ${KDE4_KDECORE_LIBS} ${KDEPIMLIBS_KABC_LIBS} ${KDE4_KIO_LIBS} ${QT_QTSQL_LIBRARY} ${QT_QT3SUPPORT_LIBRARY} ${KDE4_KDEUI_LIBS} kde3support khtml ctemplate pthread)

install(TARGETS kraft ${INSTALL_TARGETS_DEFAULT_ARGS})



########### install files ###############

install(FILES  kraft.desktop DESTINATION ${XDG_APPS_INSTALL_DIR})
install(FILES  kraftsettings.kcfg katalogsettings.kcfg DESTINATION ${KCFG_INSTALL_DIR})
install(FILES  docoverview.css docoverview_ro.css catalogview.css DESTINATION ${DATA_INSTALL_DIR}/kraft)
install(FILES  kraftui.rc katalogview.rc DESTINATION ${DATA_INSTALL_DIR}/kraft)

kde4_install_icons(${ICON_INSTALL_DIR})



#original Makefile.am contents follow:

#
#
#
######## kdevelop will overwrite this part!!! (begin)##########
#
#lib_LTLIBRARIES = libbruns.la
#
#libkraftcat_la_SOURCES = catalogtemplate.cpp katalog.cpp katalogman.cpp  brunskatalog.cpp \
#brunsrecord.cpp defaultprovider.cpp doctext.cpp doctype.cpp \
#kataloglistview.cpp flostab.ui katalogview.cpp brunskataloglistview.cpp brunskatalogview.cpp \
#filterheader.cpp catalogchapteredit.cpp  kraftdb.cpp \
#geld.cpp einheit.cpp katalogsettings.kcfgc kraftsettings.kcfgc attribute.cpp
#
#
#libkraftcat_la_LDFLAGS = $(KDE_RPATH) $(all_libraries) -version-info 1:0:0 -no-undefined
#libkraftcat_la_LIBADD = $(LIB_KIO) $(all_libraries)
#
#bin_PROGRAMS = kraft kplant
#
### INCLUDES were found outside kdevelop specific part
#
#kraft_SOURCES = templkatalog.cpp docposition.cpp matkatalog.cpp zeitcalcpart.cpp \
#	fixcalcpart.cpp floskeltemplate.cpp floskel.cpp calcpart.cpp kraftview.cpp \
#	kraftdoc.cpp main.cpp unitmanager.cpp stockmaterial.cpp \
#	stockmaterialman.cpp materialcalcpart.cpp flostempldialog.cpp \
#	stdsatzman.cpp zeitpartui.ui fixpartui.ui matpartui.ui inserttmplbase.ui \
#	materialdialog.ui materialtempldialog.cpp insertplantbase.ui \
#	zeitcalcdialog.cpp fixcalcdialog.cpp mateditor.cpp matdatatable.cpp matcalcdialog.h \
#	matcalcdialog.cpp templatesaverbase.cpp templatesaverdb.cpp  \
#	materialsaverbase.cpp materialsaverdb.cpp \
#	prefsdialog.cpp templkatalogview.cpp templkataloglistview.cpp  \
#	docheader.ui positionwidget.ui positionviewwidget.cpp \
#	documentsaverbase.cpp documentsaverdb.cpp docfooter.ui documentman.cpp docdigest.cpp \
#	docdigestview.cpp portal.cpp portalview.cpp archiveman.cpp reportgenerator.cpp \
#	htmlview.cpp docpostcard.cpp catalogselection.cpp \
#	addressselection.cpp kraftdocheaderedit.cpp kraftdocfooteredit.cpp \
#	inserttempldialog.cpp archdocposition.cpp archdoc.cpp \
#	materialkataloglistview.cpp materialkatalogview.cpp materialselectdialog.cpp \
#        kraftdocedit.cpp kraftdocpositionsedit.cpp portalhtmlview.cpp \
#	templtopositiondialogbase.cpp insertplantdialog.cpp headerselection.cpp \
#	textselection.cpp newdocassistant.cpp \
#	docassistant.cpp texteditdialog.cpp texteditbase.ui templateprovider.cpp \
#	headertemplateprovider.cpp footertemplateprovider.cpp catalogtemplateprovider.cpp \
#	addresstemplateprovider.cpp texttemplate.cpp doclocaledialog.cpp extendedcombo.cpp \
#	tagman.cpp positiontagdialog.cpp tagtemplatesdialog.cpp
#
#kraft_LDADD   =  libbruns.la $(LIB_KIO) $(LIB_KABC) -lkhtml -lctemplate -lpthread
#
#kplant_SOURCES = brunsviewer.cpp brunsviewmain.cpp
#kplant_LDADD = libbruns.la $(LIB_KIO)
#
#EXTRA_DIST = kraftdoc.h kraftview.h kraftui.rc katalogview.rc kraft.desktop lo32-app-kange.png lo16-app-kange.png calcpart.cpp calcpart.h floskel.cpp floskel.h kraftglobals.h einheit.cpp einheit.h floskeltemplate.cpp floskeltemplate.h fixcalcpart.cpp fixcalcpart.h zeitcalcpart.cpp zeitcalcpart.h kraftdb.cpp kraftdb.h katalog.cpp templkatalog.cpp matkatalog.cpp matkatalog.h katalog.h templkatalog.h kataloglistview.cpp kataloglistview.h katalogview.h dbids.h unitmanager.h stockmaterial.h stockmaterialman.h materialcalcpart.h flostempldialog.h geld.h katalogman.h stdsatzman.h zeitcalcdialog.h fixcalcdialog.h mateditor.cpp mateditor.h matdatatable.cpp matdatatable.h matcalcdialog.h matcalcdialog.cpp templatesaverbase.cpp templatesaverbase.h templatesaverdb.cpp templatesaverdb.h 
#
#KDE_ICON = kraft
#
#xdg_apps_DATA = kraft.desktop
#
#kde_kcfg_DATA = kraftsettings.kcfg katalogsettings.kcfg
#
## dependencies on the header from ui files: (strange bug, only for not unsermake)
#docposition.o: positionwidget.h
#kraftview.o: docfooter.h docheader.h
#inserttempldialog.o: inserttmplbase.h
#texteditdialog.o: texteditbase.h
#insertplantdialog.o: insertplantbase.h
#
######## kdevelop will overwrite this part!!! (end)############
## These paths are KDE specific. Use them:
## kde_appsdir         Where your application's menu entry (.desktop) should go to.
## kde_icondir         Where your icon should go to - better use KDE_ICON.
## kde_sounddir        Where your sounds should go to.
## kde_htmldir         Where your docs should go to. (contains lang subdirs)
## kde_datadir         Where you install application data. (Use a subdir)
## kde_locale          Where translation files should go to. (contains lang subdirs)
## kde_cgidir          Where cgi-bin executables should go to.
## kde_confdir         Where config files should go to (system-wide ones with default values).
## kde_mimedir         Where mimetypes .desktop files should go to.
## kde_servicesdir     Where services .desktop files should go to.
## kde_servicetypesdir Where servicetypes .desktop files should go to.
## kde_toolbardir      Where general toolbar icons should go to (deprecated, use KDE_ICON).
## kde_wallpaperdir    Where general wallpapers should go to.
## kde_templatesdir    Where templates for the "New" menu (Konqueror/KDesktop) should go to.
## kde_bindir          Where executables should go to. Use bin_PROGRAMS or bin_SCRIPTS.
## kde_libdir          Where shared libraries should go to. Use lib_LTLIBRARIES.
## kde_moduledir       Where modules (e.g. parts) should go to. Use kde_module_LTLIBRARIES.
## kde_styledir        Where Qt/KDE widget styles should go to (new in KDE 3).
## kde_designerdir     Where Qt Designer plugins should go to (new in KDE 3).
#
## set the include path for X, qt and KDE
#INCLUDES= $(all_includes)
#
#METASOURCES = AUTO
#
#SUBDIRS = pics
#
## the library search path. 
#kraft_LDFLAGS = $(all_libraries) $(KDE_RPATH)
#
#rcdir = $(kde_datadir)/kraft
#rc_DATA = kraftui.rc katalogview.rc
#
#cssdir = $(kde_datadir)/kraft
#css_DATA = docoverview.css catalogview.css
#
#messages: rc.cpp
#	LIST=`find . -name \*.h -o -name \*.hh -o -name \*.H -o -name \*.hxx -o -name \*.hpp -o -name \*.cpp -o -name \*.cc -o -name \*.cxx -o -name \*.ecpp -o -name \*.C`; \
#	if test -n "$$LIST"; then \
#	 $(XGETTEXT) $$LIST -o $(podir)/kraft.pot; \
#	fi
#
## noinst_HEADERS = positionviewwidget.h