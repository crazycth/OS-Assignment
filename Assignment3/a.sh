g++ Process.cpp -o out -std=c++2a
echo $1 > in
python3 generator.py >> in
./out < in