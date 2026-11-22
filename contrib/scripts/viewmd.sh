#!/bin/env bash

# viewmd.sh
#
# Copyright (C) 2026  Staf Wagemakers Belgium
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


set -o errexit
set -o pipefail
set -o nounset

usage() {

   echo >&2 "Usage: $(basename "$0") -f makedown file [-u]"
   exit 1

}

justUpdate=0

while getopts "f:u" arg; do
   case $arg in

     f)
       mdFile=$OPTARG
       ;;
     u)
       justUpdate=1
       ;;
     *)
       usage
       ;;
   esac
done

echo >&2 "mdFile = \"$mdFile\""

if [ -z "$mdFile" ]; then
   usage
fi

if [ ! -r "$mdFile" ]; then
   echo >&2 "ERROR: $mdFile is not readable"
   exit 2
fi

mdFileStripped=$( echo "$mdFile" | tr '/' '_' )

tmpFile="/tmp/viewmd_${mdFileStripped}.html"

pandoc "$mdFile" > "$tmpFile"

if [ "$justUpdate" = "1" ]; then
   exit 0
fi


firefox "$tmpFile"
