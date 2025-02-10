filelist=`find ../src -type f \( -iname \*.cpp -o -iname \*.h \)`
echo ${filelist} | tr ' ' '\n'

po_template_file=../locale/ru_RU/LC_MESSAGES/Editor.pot
if [ -f "$po_template_file" ]; then
	echo "updating $po_template_file ..."
else
	echo "initializing $po_template_file ..."
	po_dir=`dirname $po_template_file`
	[ ! -d $po_dir ] && mkdir -p $po_dir
	touch $po_template_file
fi

xgettext -j -C -n -kTranslate:2,2t -kTranslate:2,3,4t -kTranslateCtx:2,3c,3t -kTranslateCtx:2,3,5t,5t -kTranslateDefer:2,2t -kTranslateDefer:2,3,4t -kTranslateCtxDefer:2,3c,3t -kTranslateCtxDefer:2,3,5t,5t -o $po_template_file $filelist
sed -i 's|Language:|Language: ru|g' $po_template_file
sed -i 's|charset=CHARSET|charset=UTF-8|g' $po_template_file
sed -i 's|nplurals=INTEGER|nplurals=3|g' $po_template_file
sed -i 's|plural=EXPRESSION|plural=n%10==1 \&\& n%100!=11 ? 0 : n%10>=2 \&\& n%10<=4 \&\& (n%100<10 \|\| n%100>=20) ? 1 : 2|g' $po_template_file


po_file=../locale/ru_RU/LC_MESSAGES/Editor.po
if [ ! -f "$po_file" ]; then
	echo "initializing $po_file ..."
	msginit -i $po_template_file -o $po_file -l ru_RU
else
	echo "updating $po_file ..."
fi

msgattrib --set-obsolete --ignore-file=$po_template_file -o $po_file $po_file
msgattrib --no-obsolete -o $po_file $po_file
msgmerge -q -o $po_file $po_file $po_template_file

rm $po_template_file

echo "Done!"