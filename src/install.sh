#!/bin/bash
#
# Copyright (C) 2019-2020 Rafael M. Siejakowski
# License information at the end of the file.
#

TARGET="/usr/bin"

if [[ $UID -eq 0 ]]
then
	EXECUTABLE="$1"
	if [[ "x$EXECUTABLE" == "x" ]]
	then
		echo "Error: no executable to install"
		exit
	fi
	if [[ -f "$EXECUTABLE" ]] && [[ -d "$TARGET" ]]
	then
		cp -u "$EXECUTABLE" "$TARGET"
		if [[ $? -eq 0 ]]
		then
			echo "Successfully installed $EXECUTABLE to $TARGET"
		else
			echo "Error: unable to copy $EXECUTABLE to $TARGET"
		fi
	else
		echo "Error: your filesystem does not appear to have the file $EXECUTABLE or the directory $TARGET"
	fi
else
	echo "You must be root to do this."
fi
#
# Copyright (C) 2019-2020 Rafael M. Siejakowski
# 
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License 
# version 2 as published by the Free Software Foundation; 
# later versions of the GNU General Public Licence do NOT apply.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 
# 02110-1301, USA.
