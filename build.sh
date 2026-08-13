#!/bin/bash
# Build notebook.pdf. Self-contained: it points at the TeX Live tree under ~/texlive, so it needs
# neither a system TeX nor sudo. Run from anywhere; it cd's to the repository root itself.
#
# If ~/texlive is missing (a fresh machine), get a TeX with pdflatex + makeindex + inconsolata:
#   brew install --cask basictex     (then: sudo tlmgr install inconsolata needspace tocloft)
# or on Overleaf just upload notebook.tex - every package used here is in the default image.
set -e
export TEXLIVE_HOME="$HOME/texlive"
if [ -d "$TEXLIVE_HOME" ]; then
    export PATH="$TEXLIVE_HOME/2026basic/bin/universal-darwin:$PATH"
    export TEXMFHOME="$TEXLIVE_HOME/texmfhome"
    export TEXMFVAR="$TEXLIVE_HOME/texmfvar"
    export TEXMFCONFIG="$TEXLIVE_HOME/texmfconfig"
fi

cd "$(dirname "$0")"
node export_notebook.js "$@"
