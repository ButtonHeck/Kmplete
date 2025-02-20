if [ -z "$1" ]
  then
    echo "No domain supplied"
fi

echo "--------Compile PO for English--------"
sh ./po_compile_en.sh $1

echo "--------Compile PO for Russian--------"
sh ./po_compile_ru.sh $1
