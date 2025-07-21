echo "" > test.c;
set fs (ls test_*.c);
for f in $fs;
    echo "#include \"$f\"" >> test.c;
end;
echo -e "" >> test.c
cat base_test.c >> test.c;
echo -e "" >> test.c;
echo "int main() {" >> test.c;
for f in $fs;
    echo "Parsing $f";
    set funcs (rg 'int test_' $f);
    for func in $funcs;
        set pfunc (echo $func | awk '{match($0, /test_[^(]+/); print substr($0, RSTART, RLENGTH)}');
        echo "Found $pfunc in $f";
        echo "  test($pfunc);" >> test.c
    end;
end;
echo "
  printf(\"Success: %d, Failures: %d\n\", success, failures);

  return failures;
}
" >> test.c;
