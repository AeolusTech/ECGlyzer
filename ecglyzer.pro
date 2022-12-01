TEMPLATE = subdirs

CONFIG+=ordered

SUBDIRS = \
    code \
    googletest \
    tests \
    tests2



tests2.depends = googletest
