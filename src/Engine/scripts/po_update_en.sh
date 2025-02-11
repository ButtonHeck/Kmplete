if [ -z "$1" ]
  then
    echo "No domain supplied"
fi

filelist=`find ../src ../include -type f \( -iname \*.cpp -o -iname \*.h \)`
echo ${filelist} | tr ' ' '\n'

po_template_file=../locale/en_EN/LC_MESSAGES/$1.pot
if [ -f "$po_template_file" ]; then
	echo "updating $po_template_file ..."
else
	echo "initializing $po_template_file ..."
	po_dir=`dirname $po_template_file`
	[ ! -d $po_dir ] && mkdir -p $po_dir
	touch $po_template_file
fi

xgettext -j -C -n -kTranslate:2,2t -kTranslate:2,3,4t -kTranslateCtx:2,3c,3t -kTranslateCtx:2,3,5t,5t -kTranslateDefer:2,2t -kTranslateDefer:2,3,4t -kTranslateCtxDefer:2,3c,3t -kTranslateCtxDefer:2,3,5t,5t -o $po_template_file $filelist
sed -i 's|Language:|Language: en|g' $po_template_file
sed -i 's|charset=CHARSET|charset=UTF-8|g' $po_template_file
sed -i 's|nplurals=INTEGER|nplurals=2|g' $po_template_file
sed -i 's|plural=EXPRESSION|plural=n != 1|g' $po_template_file


po_file=../locale/en_EN/LC_MESSAGES/$1.po
if [ ! -f "$po_file" ]; then
	echo "initializing $po_file ..."
	msginit -i $po_template_file -o $po_file -l en_EN
else
	echo "updating $po_file ..."
fi

msgattrib --set-obsolete --ignore-file=$po_template_file -o $po_file $po_file
msgattrib --no-obsolete -o $po_file $po_file
msgmerge -q -o $po_file $po_file $po_template_file

rm $po_template_file

echo "Done!"