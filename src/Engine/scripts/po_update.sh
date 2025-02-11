if [ -z "$1" ]
  then
    echo "No domain supplied"
fi

echo "--------Update PO for English--------"
sh ./po_update_en.sh $1

echo "--------Update PO for Russian--------"
sh ./po_update_ru.sh $1
