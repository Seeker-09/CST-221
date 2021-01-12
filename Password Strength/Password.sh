#!/bin/sh

#  Password.sh
#  CST-221
#
#  Created by David Mundt on 1/12/21.
#  

echo "Enter password"
read password

length="$(#password)"

if test $length -ge 8; then
echo "$password" | grep -q[0-9]

if test $? -eq 0; then
echo "$password" | grep -q[A-Z]

if test $? -eq 0; then
echo "$password" | grep -q[a-z]

if test $? -eq 0; then
echo "Strong password"

else
echo "Weak Password - Need lowercase"
fi

else
echo "Weak Password - need uppercase"
fi

else
echo "Weak Password - Need numbers"
fi

else
echo "Weak Password - less than 8 characters"
fi
