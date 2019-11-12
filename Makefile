default:
	gcc -o skl skiplist_test.c -I.
	gcc -o sklr skiplist_with_rank_test.c -I.
clean:
	rm -rf skl sklr
