
################################################################################
##                                PROBABILITY                                 ##
################################################################################

# Project info
# ===============
PROJECT         := Probability
VERSION         := 0.0.1
STD_NAMESPACE   := probability
GIT_REMOTE_PATH := # Remote path for git repository
MAINTEINER_NAME := Renato Cordeiro Ferreira
MAINTEINER_MAIL := renato.cferreira@hotmail.com
COPYRIGHT       := Renato Cordeiro Ferreira
SYNOPSIS        := A fast implementation of probabilities
DESCRIPTION     := A fast implementation of probabilities using logarithms

# Program settings
# ==================
BENCHBIN        := bench

# Flags
# =======
CPPFLAGS        := # Precompiler Flags
CXXFLAGS        := -std=c++14 -Wall -Wextra -Wpedantic -Wshadow -O3
LDFLAGS         := # Linker flags

# Makeball list
# ===============
include conf/googletest.mk
include conf/googlebenchmark.mk
