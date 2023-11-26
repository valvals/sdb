TEMPLATE = subdirs

SUBDIRS += \
app\
libs\
ipc\
tests\

app.depends = libs
tests.depends = libs

