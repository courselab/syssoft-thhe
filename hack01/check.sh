#!/bin/bash


if ! md5sum -c docrypt.md5  ; then
   echo "docrypt invalid"
   exit 1
fi

if !  md5sum -c libauth.so.md5  ; then
   echo "libauth.so invalid"
   exit 1
fi

echo "All checks passed."
