#!/bin/sh

#
# man2format.sh
#
# script to convert a manpage to a text file
#
# Copyright (C) 2007, 2020, 2026 Staf Wagemakers Belgie/Belgium
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License along
# with this program; if not, write to the Free Software Foundation, Inc.,
# 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
#
#

usage () {

	echo >&2
	echo >&2  "Usage: $(basename "$0") [ -f txt|html|md ] manpage.man"
	echo >&2

	exit 1

}

file=""
format="txt"
verbose=""


while getopts "hvf:" OPTION; do
  case $OPTION in
    f)
      format=$OPTARG
      ;;
    v)
      verbose=1
      ;;
    h)
      usage
      ;;
    ?)
      usage
      ;;
  esac
done

shift $((OPTIND - 1))

file=$1

if [ "$verbose" ]; then


    echo >&2 "... format set to ${format}"
    echo >&2 "... file set to ${file}"

fi

if [ "$file" = "" ]; then

    echo >&2 "ERROR: Sorry I need a filename"
    usage

fi

case "$format" in
    txt)
        # Code generate the text manpage with GNU groff
        # Using man dirrectly now
        # groff -t -e -mandoc -Tascii $1 | col -bx
        man "./${file}" | col -bx
        ;;
    html)
        groff -t -e -mandoc -Thtml "${file}"
        ;;
    markdown)
        pandoc -f man -t markdown "${file}"
        ;;
    *)
	    echo >&2
        echo >&2 "Unknown format ${format}"
        usage
        ;;
esac

