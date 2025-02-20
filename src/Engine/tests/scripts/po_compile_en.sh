if [ -z "$1" ]
  then
    echo "No domain supplied"
fi

cd ../locale/en_EN/LC_MESSAGES
msgfmt -o $1.mo $1.po