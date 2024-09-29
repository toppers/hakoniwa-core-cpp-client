
for file_path in `find results -name "asset*.csv"`
do
    python check-time-diff.py ${file_path}
done