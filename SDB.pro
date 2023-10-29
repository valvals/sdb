TEMPLATE = subdirs

SUBDIRS += \
app\
libs\
tests\

app.depends = libs
tests.depends = libs

